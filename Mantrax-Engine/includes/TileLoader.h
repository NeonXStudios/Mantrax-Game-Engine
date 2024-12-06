#pragma once
#include <glad/glad.h>
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <vector>

const int tileSize = 16;
const int mapWidth = 16;
const int mapHeight = 16;

struct TileData
{
    GLuint textureID;
    ImVec2 uv0;
    ImVec2 uv1;
    int tileX, tileY;

    bool operator==(const TileData &other) const
    {
        return textureID == other.textureID && tileX == other.tileX && tileY == other.tileY;
    }
};

class TileLoader
{
public:
    GLuint texture = 0;
    std::vector<std::pair<int, int>> selectedTiles;
    TileData lastSelectedTileData;
    std::vector<TileData> selectedTextures;

    int tileSize = 16;
    int mapWidth = 16;
    int mapHeight = 16;

    int tilemap[16][16]; // Matriz de tiles

    TileLoader()
    {
        // Inicializar el mapa con un valor de ejemplo
        resetTilemap();
    }

    void resetTilemap()
    {
        // Inicializar el tilemap con algunos valores por defecto
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                tilemap[y][x] = (x + y) % 4; // Ejemplo simple para ilustrar
            }
        }
    }

    GLuint LoadTexture(const char *filePath, int &tilesetWidth, int &tilesetHeight)
    {
        int channels;
        unsigned char *data = stbi_load(filePath, &tilesetWidth, &tilesetHeight, &channels, 4);
        if (!data)
        {
            std::cerr << "Error al cargar el archivo de tileset: " << filePath << "\n";
            return 0;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tilesetWidth, tilesetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
        return texture;
    }

    bool load_tile(const char *filePath)
    {
        int tilesetWidth, tilesetHeight;
        texture = LoadTexture(filePath, tilesetWidth, tilesetHeight);
        return texture != 0;
    }

    void handleTileSelection(int x, int y, bool shiftPressed)
    {
        if (!shiftPressed)
        {
            selectedTiles.clear();
        }

        auto tile = std::make_pair(x, y);
        auto it = std::find(selectedTiles.begin(), selectedTiles.end(), tile);
        if (it == selectedTiles.end())
        {
            selectedTiles.push_back(tile);
        }
        else
        {
            selectedTiles.erase(it);
        }

        saveSelectedTileData(x, y);
    }

    void saveSelectedTileData(int tileX, int tileY)
    {
        lastSelectedTileData.tileX = tileX;
        lastSelectedTileData.tileY = tileY;
        lastSelectedTileData.textureID = texture;

        lastSelectedTileData.uv0 = ImVec2(tileX * tileSize / (float)(mapWidth * tileSize),
                                          tileY * tileSize / (float)(mapHeight * tileSize));
        lastSelectedTileData.uv1 = ImVec2((tileX + 1) * tileSize / (float)(mapWidth * tileSize),
                                          (tileY + 1) * tileSize / (float)(mapHeight * tileSize));
    }

    void draw_tile()
    {
        ImGui::Begin("Tilemap Viewer");

        // Panel para modificar tileSize, mapWidth, mapHeight
        ImGui::InputInt("Tile Size", &tileSize, 1, 10, ImGuiInputTextFlags_CharsDecimal);
        ImGui::InputInt("Map Width", &mapWidth, 1, 10, ImGuiInputTextFlags_CharsDecimal);
        ImGui::InputInt("Map Height", &mapHeight, 1, 10, ImGuiInputTextFlags_CharsDecimal);

        if (ImGui::Button("Apply"))
        {
            // Al presionar Apply, recargamos el mapa con los nuevos valores
            resetTilemap();
        }

        ImGui::Text("Clic en un tile para seleccionarlo.");
        ImGui::Text("Shift+Clic para seleccionar múltiples.");

        if (texture)
        {
            ImVec2 cursorPos = ImGui::GetCursorScreenPos();
            ImTextureID texID = (void *)(intptr_t)texture;

            ImGui::Image(texID, ImVec2(mapWidth * tileSize, mapHeight * tileSize));

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
            {
                ImVec2 mousePos = ImGui::GetMousePos();
                int tileX = (mousePos.x - cursorPos.x) / tileSize;
                int tileY = (mousePos.y - cursorPos.y) / tileSize;

                if (tileX >= 0 && tileX < mapWidth && tileY >= 0 && tileY < mapHeight)
                {
                    bool shiftPressed = ImGui::GetIO().KeyShift;
                    handleTileSelection(tileX, tileY, shiftPressed);
                }
            }

            for (const auto &tile : selectedTiles)
            {
                int x = tile.first;
                int y = tile.second;

                ImVec2 rectMin = ImVec2(cursorPos.x + x * tileSize, cursorPos.y + y * tileSize);
                ImVec2 rectMax = ImVec2(rectMin.x + tileSize, rectMin.y + tileSize);

                ImGui::GetWindowDrawList()->AddRect(rectMin, rectMax, IM_COL32(255, 0, 0, 255), 0.0f, 0, 3.0f);
            }
        }
        else
        {
            ImGui::Text("Textura no cargada.");
        }

        ImGui::Separator();

        if (lastSelectedTileData.textureID != 0)
        {
            auto it = std::find(selectedTextures.begin(), selectedTextures.end(), lastSelectedTileData);
            if (it == selectedTextures.end())
            {
                selectedTextures.push_back(lastSelectedTileData);
            }

            ImGui::Text("Última textura seleccionada:");
            ImGui::Image((void *)(intptr_t)lastSelectedTileData.textureID, ImVec2(64, 64),
                         lastSelectedTileData.uv0, lastSelectedTileData.uv1);
        }
        else
        {
            ImGui::Text("No hay textura seleccionada.");
        }

        ImGui::End();

        if (!selectedTextures.empty())
        {
            ImGui::Begin("Selected Textures");
            for (const auto &tileData : selectedTextures)
            {
                ImGui::Image((void *)(intptr_t)tileData.textureID, ImVec2(64, 64), tileData.uv0, tileData.uv1);
            }
            ImGui::End();
        }
    }
};