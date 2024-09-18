#include "../includes/InputSystemUI.h"

// Variables estáticas para almacenar las teclas añadidas
std::unordered_map<int, GLuint> key_bindings; // Para almacenar los códigos de las teclas y sus IDs

// Variable para capturar la tecla presionada
bool capturing_key = false;
int key_to_capture = -1; // ID de la tecla que estamos capturando

void InputSystemUI::draw()
{
    if (is_open)
    {
        ImGui::Begin("Input System", &is_open);

        // Título principal con diseño más limpio
        ImGui::Text("Configuración del Sistema de Entrada");
        ImGui::Separator();

        // Sección de asignación de teclas
        ImGui::Text("Asignación de Teclas:");

        // Mostrar teclas añadidas
        int id = 0;
        for (auto &[key_id, key_code] : key_bindings)
        {
            ImGui::PushID(key_id); // Usar un ID único para cada tecla

            // Mostrar el nombre o código de la tecla asignada
            ImGui::Text("Tecla %d:", key_id + 1);
            ImGui::SameLine();

            // Mostrar el nombre de la tecla actual
            std::string key_name = key_code != 0 ? glfwGetKeyName(key_code, 0) : "N/A";
            ImGui::Text("%s", key_name.c_str());
            ImGui::SameLine();

            // Botón para capturar una tecla (para asignarla mediante el teclado)
            if (ImGui::Button("Asignar tecla"))
            {
                capturing_key = true;
                key_to_capture = key_id; // Capturamos el ID de la tecla que queremos modificar
            }

            // Botón para eliminar la tecla
            ImGui::SameLine();
            if (ImGui::Button("Eliminar Tecla"))
            {
                key_bindings.erase(key_id);
            }

            ImGui::PopID();
            id++;
        }

        if (capturing_key && key_to_capture != -1)
        {
            ImGui::Text("Presiona una tecla para asignar...");
            for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; key++)
            {
                if (glfwGetKey(Gfx::get_game_window(), key) == GLFW_PRESS)
                {
                    key_bindings[key_to_capture] = key;
                    capturing_key = false;
                    key_to_capture = -1;
                    break;
                }
            }
        }

        ImGui::Separator();

        if (ImGui::Button("Añadir Tecla"))
        {
            int new_key_id = key_bindings.size();
            key_bindings[new_key_id] = 0;
        }

        ImGui::End();
    }
}