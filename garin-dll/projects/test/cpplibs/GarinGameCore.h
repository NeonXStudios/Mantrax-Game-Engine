#include <dll.h>
#include <GarinComponents.h>
#include <GarinGraphics.h>
#include <GarinBehaviours.h>
#include "GameCoreManager.h"

#define GCLASSDLL(CM) (engine->register_class(#CM, []() -> std::unique_ptr<GameBehaviour> { return std::make_unique<CM>(); }))

#define REGISTER_CLASS void REGISTER_COMPONENTS(GameBehaviourFactory *engine)
extern "C" GARIN_API void REGISTER_COMPONENTS(GameBehaviourFactory *engine);

#define ASSIGNAPIS() (GameCoreManager::current_scene = api, GameCoreManager::engine_data = graphics)
#define ASSIGN_APIS void REGISTER_APIS(scenes *api, Graphics *graphics)
extern "C" GARIN_API void REGISTER_APIS(scenes *api, Graphics *graphics);
