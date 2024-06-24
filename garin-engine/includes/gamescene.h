#pragma once
#define JPH_OBJECT_STREAM

#include <GarinNatives.h>
#include "WindowsCompiler.h"
#include "UIAdministrator.h"
#include "DebugGame.h"

#include "EditorGUI.h"
#include "EditorConfigs.h"

#include <GarinBehaviours.h>
#include <GarinMaths.h>
#include <GarinGraphics.h>
#include <GarinEvents.h>
#include <GarinNatives.h>

#include <TerrainGenerator.h>

#include <Jolt/Jolt.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

JPH_SUPPRESS_WARNINGS

using namespace JPH;
using namespace JPH::literals;
using namespace std;

class Drawer;
class Camera;
class Entity;
class EditorConfigs;
class GameBehaviourRunner;
class InspectorUI;
class MainBarUI;
class SceneUI;
class FilesUI;
class AssetsUI;
class GameSettingsUI;
class HierarchyUI;
class MenuUI;
class UINotification;

static void TraceImpl(const char *inFMT, ...)
{
    // Format the message
    va_list list;
    va_start(list, inFMT);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), inFMT, list);
    va_end(list);

    // Print to the TTY
    cout << buffer << endl;
}

#ifdef JPH_ENABLE_ASSERTS

// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char *inExpression, const char *inMessage, const char *inFile, uint inLine)
{
    // Print to the TTY
    cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << endl;

    // Breakpoint
    return true;
};

#endif // JPH_ENABLE_ASSERTS

// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers
{
    static constexpr ObjectLayer NON_MOVING = 0;
    static constexpr ObjectLayer MOVING = 1;
    static constexpr ObjectLayer NUM_LAYERS = 2;
};

/// Class that determines if two object layers can collide
class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
{
public:
    virtual bool ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override
    {
        switch (inObject1)
        {
        case Layers::NON_MOVING:
            return inObject2 == Layers::MOVING; // Non moving only collides with moving
        case Layers::MOVING:
            return true; // Moving collides with everything
        default:
            JPH_ASSERT(false);
            return false;
        }
    }
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
    static constexpr BroadPhaseLayer NON_MOVING(0);
    static constexpr BroadPhaseLayer MOVING(1);
    static constexpr uint NUM_LAYERS(2);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
{
public:
    BPLayerInterfaceImpl()
    {
        // Create a mapping table from object to broad phase layer
        mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
        mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
    }

    virtual uint GetNumBroadPhaseLayers() const override
    {
        return BroadPhaseLayers::NUM_LAYERS;
    }

    virtual BroadPhaseLayer GetBroadPhaseLayer(ObjectLayer inLayer) const override
    {
        JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
        return mObjectToBroadPhase[inLayer];
    }

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    virtual const char *GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
    {
        switch ((BroadPhaseLayer::Type)inLayer)
        {
        case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:
            return "NON_MOVING";
        case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:
            return "MOVING";
        default:
            JPH_ASSERT(false);
            return "INVALID";
        }
    }
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
    BroadPhaseLayer mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/// Class that determines if an object layer can collide with a broadphase layer
class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
{
public:
    virtual bool ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override
    {
        switch (inLayer1)
        {
        case Layers::NON_MOVING:
            return inLayer2 == BroadPhaseLayers::MOVING;
        case Layers::MOVING:
            return true;
        default:
            JPH_ASSERT(false);
            return false;
        }
    }
};

// An example contact listener
class MyContactListener : public ContactListener
{
public:
    // See: ContactListener
    virtual ValidateResult OnContactValidate(const Body &inBody1, const Body &inBody2, RVec3Arg inBaseOffset, const CollideShapeResult &inCollisionResult) override
    {
        cout << "Contact validate callback" << endl;

        // Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
        return ValidateResult::AcceptAllContactsForThisBodyPair;
    }

    virtual void OnContactAdded(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
    {
        cout << "A contact was added" << endl;
    }

    virtual void OnContactPersisted(const Body &inBody1, const Body &inBody2, const ContactManifold &inManifold, ContactSettings &ioSettings) override
    {
        cout << "A contact was persisted" << endl;
    }

    virtual void OnContactRemoved(const SubShapeIDPair &inSubShapePair) override
    {
        cout << "A contact was removed" << endl;
    }
};

// An example activation listener
class MyBodyActivationListener : public BodyActivationListener
{
public:
    virtual void OnBodyActivated(const BodyID &inBodyID, uint64 inBodyUserData) override
    {
        cout << "A body got activated" << endl;
    }

    virtual void OnBodyDeactivated(const BodyID &inBodyID, uint64 inBodyUserData) override
    {
        cout << "A body went to sleep" << endl;
    }
};

class gamescene : public scenes
{
public:
    Drawer *drawcube;
    Camera *camera;

    Entity *select_obj;
    EditorConfigs *configs = new EditorConfigs();
    GameBehaviourRunner *runner;

    InspectorUI *inspector;
    MainBarUI *mainbar;
    SceneUI *sceneui;
    FilesUI *filesui;
    AssetsUI *assetsui;
    GameSettingsUI *settingsui;
    HierarchyUI *hierarchyui;
    MenuUI *menuui;
    UINotification *notify;

    PhysicsSystem *mPhysicsSystem;
    TempAllocatorImpl *mTempAllocator;
    JobSystemThreadPool *mJobSystem;

    static constexpr uint cMaxBodies = 1024;
    static constexpr uint cNumBodyMutexes = 0;
    static constexpr uint cMaxBodyPairs = 1024;
    static constexpr uint cMaxContactConstraints = 1024;

    BPLayerInterfaceImpl mBroadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl mObjectVsBroadPhaseLayerFilter;
    ObjectLayerPairFilterImpl mObjectVsObjectLayerFilter;
    BodyInterface *mBodyInterface;
    BodyID mSphereID;
    Body *floor;

    void embraceTheDarkness();

    void on_start() override;
    void on_update(float delta_time) override;
    void on_draw() override;
    void draw_ui() override;
    void on_destroy() override;
    void set_object_select(Entity *obj);

    std::string demangle(const char *name);
};