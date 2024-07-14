#include "../includes/LightingPass.h"

// std::vector<glm::vec3> LightingPass::lightPositions;
// std::vector<glm::vec3> LightingPass::lightColors;

// LightingPass::LightingPass(GBuffer &gbuffer) : gbuffer(gbuffer)
// {
//     setupQuad();
// }

// void LightingPass::setupQuad()
// {
//     float quadVertices[] = {
//         // positions        // texture Coords
//         -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//         -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

//         -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
//         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//         1.0f, 1.0f, 0.0f, 1.0f, 1.0f};

//     glGenVertexArrays(1, &quadVAO);
//     glGenBuffers(1, &quadVBO);
//     glBindVertexArray(quadVAO);
//     glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
//     glEnableVertexAttribArray(1);
//     glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
// }

// void LightingPass::render(Shader &shader)
// {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     shader.use();
//     shader.setVec3("lightPosition", glm::vec3(0, 0, 0));
//     shader.setVec3("lightColor", glm::vec3(1, 0, 0));
//     shader.setVec3("viewPos", SceneManager::GetOpenScene()->main_camera->cameraPosition);
//     shader.setFloat("shininess", 0.5f);

//     glActiveTexture(GL_TEXTURE0);
//     glBindTexture(GL_TEXTURE_2D, gbuffer.getPositionTexture());
//     glActiveTexture(GL_TEXTURE1);
//     glBindTexture(GL_TEXTURE_2D, gbuffer.getNormalTexture());
//     glActiveTexture(GL_TEXTURE2);
//     glBindTexture(GL_TEXTURE_2D, gbuffer.getAlbedoSpecTexture());

//     glBindVertexArray(quadVAO);
//     glDrawArrays(GL_TRIANGLES, 0, 6);
//     glBindVertexArray(0);
// }