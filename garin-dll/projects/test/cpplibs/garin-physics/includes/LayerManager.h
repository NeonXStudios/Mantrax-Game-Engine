#pragma once
// struct LayerManager
// {
//     static const int NUM_LAYERS = 31;
//     const char *layerNames[NUM_LAYERS] = {
//         "Default", "TransparentFX", "IgnoreRaycast", "Water", "UI",
//         "Player", "Enemy", "FriendlyNPC", "Environment", "Interactable",
//         "Trigger", "StaticObject", "DynamicObject", "Ground", "Air",
//         "Obstacle", "Pickup", "Weapon", "ParticleSystem", "Skybox",
//         "Character", "Projectile", "Sensor", "Debris", "Audio",
//         "Camera", "UIElement", "Background", "Vehicle", "Cloud",
//         "Portal"};
//     bool layerSelected[NUM_LAYERS] = {false};

//     int getSelectedLayersMask()
//     {
//         int mask = 0;
//         for (int i = 0; i < NUM_LAYERS; ++i)
//         {
//             if (layerSelected[i])
//             {
//                 mask |= (1 << i);
//             }
//         }
//         return mask;
//     }
// };
