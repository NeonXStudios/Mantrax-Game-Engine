#include <dll.h>
#include <GarinComponents.h>
#include <GarinGraphics.h>
#include <GarinBehaviours.h>
#include <EngineWrapper.h>

#define GCLASSDLL(CM) (engine->register_class(#CM, []() -> std::unique_ptr<GameBehaviour> { return std::make_unique<CM>(); }))

#define REGISTER_CLASS void REGISTER_COMPONENTS(GameBehaviourFactory *engine)
extern "C" GARINLIBS_API void REGISTER_COMPONENTS(GameBehaviourFactory *engine);

#define ASSIGNAPIS() (SceneManager::instance = scene_api, Gfx::window = window_api)
#define ASSIGN_APIS void REGISTER_APIS(SceneManager *scene_api, GLFWwindow *window_api)
extern "C" GARINLIBS_API void REGISTER_APIS(SceneManager *scene_api, GLFWwindow *window_api);