#include "../includes/AComponent.h"

// template <typename T>
// void AComponent::update(Entity *owner, int id)
// {
//     if (owner->hasComponent<T>())
//     {
//         ImGui::PushID(id);
//         if (ImGui::Button("X"))
//         {
//             // if (std::is_same<T, SpriteComponent>::value)
//             // {
//             //     std::cout << "El componente base no puede ser eliminado" << std::endl;
//             // }
//             // else
//             // {
//             //     owner->removeComponent<T>();
//             // }
//         }
//         else
//         {
//             draw(owner);
//             ImGui::Separator();
//         }
//         ImGui::PopID();
//     }
// }

// string AComponent::RemoveDir(string fullPath)
// {
//     std::string targetFolder = "assets/";

//     size_t pos = fullPath.find(targetFolder);
//     if (pos != std::string::npos)
//     {
//         std::string extractedPath = fullPath.substr(pos + targetFolder.length());

//         std::replace(extractedPath.begin(), extractedPath.end(), '/', '\\');

//         return extractedPath;
//     }

//     return "";
// }