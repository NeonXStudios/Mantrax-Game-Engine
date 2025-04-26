#include <MaterialMaker.h>
#include <EditorGUI.h>

void MaterialMaker::on_draw()
{
    ImGui::SetNextWindowSize(ImVec2(300, 400));
    ImGui::Begin("Material Maker");
        EditorGUI::Text("Main Texture");
        EditorGUI::InputText("-", "Assets/Package/Main.png");
    ImGui::End();
}