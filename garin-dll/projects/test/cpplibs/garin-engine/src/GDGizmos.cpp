#include "../includes/GDGizmos.h"

GDGizmos *GDGizmos::instance = nullptr;
const float PI = 3.14159265359f;

const char *vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoords = aTexCoords;    
}
)";

const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform vec3 color;

void main()
{
    vec4 texColor = texture(texture1, TexCoords);
    FragColor = vec4 (color.x, color.y, color.z, 1);
}
)";

float vertices[] = {
    // V�rtices de la cara frontal (Front Face)
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Abajo izquierda
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // Abajo derecha
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Arriba derecha
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // Arriba izquierda

    // V�rtices de la cara posterior (Back Face)
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // Abajo izquierda
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  // Abajo derecha
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f,   // Arriba derecha
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  // Arriba izquierda

    // V�rtices de la cara izquierda (Left Face)
    -0.5f, -0.5f, 0.5f, 1.0f, 0.0f,  // Abajo izquierda
    -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,   // Arriba izquierda
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // Arriba derecha
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // Abajo derecha

    // V�rtices de la cara derecha (Right Face)
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // Abajo izquierda
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f,  // Arriba izquierda
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // Arriba derecha
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f // Abajo derecha
};

// �ndices para dibujar el plano
// �ndices para dibujar un cubo 3D
// �ndices para dibujar un cubo 3D completo
unsigned int indices[] = {
    // Aristas de la cara frontal
    0, 1, 1, 2, 2, 3, 3, 0,

    // Aristas de la cara posterior
    4, 5, 5, 6, 6, 7, 7, 4,

    // Conexiones entre caras
    0, 4, 1, 5, 2, 6, 3, 7};

void GDGizmos::create()
{
    instance = new GDGizmos();

    start();
}

void GDGizmos::release()
{
}

void GDGizmos::start()
{
    // shader = new Shader("C:\\Users\\tupap\\Documents\\GoldEngine\\Projects\\Vortex\\assets\\shaders\\wire.vert", "C:\\Users\\tupap\\Documents\\GoldEngine\\Projects\\Vortex\\assets\\shaders\\wire.frag");
    // shader->use();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Enlazar el b�fer de v�rtices y configurar los datos
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Enlazar el b�fer de �ndices y configurar los datos
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configurar las propiedades de los v�rtices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Configurar las coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Crear una textura
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Configurar opciones de filtrado de textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Cargar una imagen como textura
    int width, height, nrChannels;
    unsigned char *data = stbi_load("", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Error al cargar la textura" << std::endl;
    }

    stbi_image_free(data);

    // Compilar shaders
    vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Enlazar shaders en un programa
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);
}

unsigned int GDGizmos::compileShader(unsigned int type, const char *source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    // Verificar errores de compilaci�n
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Error al compilar shader: " << infoLog << std::endl;
    }

    return shader;
}

void GDGizmos::DrawCapsule(const glm::vec3 &position, const float radius, const float height, const glm::vec3 &color, float Width)
{
    glLineWidth(Width);
    DrawPrism(position, glm::vec3(0, 90, 0), radius, height, color, 8);
    DrawPrism(position, glm::vec3(0, 0, 0), radius, height, color, 8);
}

void GDGizmos::DrawPrism(const glm::vec3 &position, glm::vec3 Rotation, const float radius, const float height, const glm::vec3 &color, float Width)
{
    glm::quat rotation;

    std::vector<float> verticesLeft = {
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 0
        1.0f, 0.5f, 0.0f, 0.0f, 1.0f, // 2
        1.0f, 4.5f, 0.0f, 0.0f, 1.0f, // 1
        0.0f, 5.0f, 0.0f, 0.0f, 1.0f, // 3

        0.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // 0
        -1.0f, 0.5f, 0.0f, 0.0f, 1.0f, // 2
        -1.0f, 4.5f, 0.0f, 0.0f, 1.0f, // 1
        0.0f, 5.0f, 0.0f, 0.0f, 1.0f,  // 3

    };

    std::vector<unsigned int> indicesLeft = {
        0,
        1,
        1,
        2,
        2,
        3,
        3,
        0,
        4,
        5,
        5,
        6,
        6,
        7,
        7,
        4,
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Enlaza el VAO
    glBindVertexArray(VAO);

    // Enlaza y configura el b�fer de v�rtices (posiciones y coordenadas de textura)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesLeft.size() * sizeof(float), verticesLeft.data(), GL_STATIC_DRAW);

    // Configura los atributos de los v�rtices
    // Atributo 0: Posiciones de los v�rtices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: Coordenadas de textura
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Enlaza y configura el b�fer de elementos
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLeft.size() * sizeof(unsigned int), indicesLeft.data(), GL_STATIC_DRAW);

    // Finalmente, puedes dibujar la c�psula llamando a glDrawElements.
    glDrawElements(GL_TRIANGLES, indicesLeft.size(), GL_UNSIGNED_INT, 0);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    glm::quat rotationZ = glm::angleAxis(glm::radians(Rotation.x), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::quat rotationY = glm::angleAxis(glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat rotationX = glm::angleAxis(glm::radians(Rotation.z), glm::vec3(1.0f, 0.0f, 0.0f));

    rotation = rotationZ * rotationY * rotationX;

    // glm::mat4 rotationMatrix = glm::mat4_cast(rotation);

    model *= glm::mat4_cast(rotation);
    model = glm::scale(model, glm::vec3(radius, height, radius) * 2.2f);
    glUseProgram(shaderProgram);

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Graphics::get_main_camera()->GetView()));

    // Enviar la matriz de proyecci�n al shader
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Graphics::get_main_camera()->GetProjectionMatrix()));

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(color));

    // Dibujar el cubo
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // Utilizar GL_LINES y 24 �ndices para dibujar el cubo con l�neas
}

void GDGizmos::DrawHalfSphere(const glm::vec3 &position, const float radius, const glm::vec3 &color, float Width)
{
    glLineWidth(Width);
}

void GDGizmos::DrawCylinder(const glm::vec3 &position, const float radius, const float height, const glm::vec3 &color, float Width)
{
    glLineWidth(Width);
}

void GDGizmos::DrawSphere(const glm::vec3 &position, const float radius, const glm::vec3 &color, float Width)
{
    glLineWidth(Width);
    // Configura las matrices de modelo, vista y proyecci�n
    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), position);
    glm::mat4 viewMatrix = Graphics::get_main_camera()->GetView();
    glm::mat4 projectionMatrix = Graphics::get_main_camera()->GetProjectionMatrix();

    // Activa tu programa de shaders
    glUseProgram(shaderProgram);

    // Configura las matrices de modelo, vista y proyecci�n en los shaders
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(color));

    // Genera los v�rtices de la esfera
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    int stacks = 20; // N�mero de "bandas" en la esfera
    int slices = 20; // N�mero de "rebanadas" en la esfera

    for (int i = 0; i <= stacks; ++i)
    {
        float theta = i * glm::pi<float>() / stacks;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for (int j = 0; j <= slices; ++j)
        {
            float phi = j * 2 * glm::pi<float>() / slices;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

            float x = cosPhi * sinTheta;
            float y = cosTheta;
            float z = sinPhi * sinTheta;

            // Agrega el v�rtice a la lista
            vertices.push_back(radius * x);
            vertices.push_back(radius * y);
            vertices.push_back(radius * z);

            // Calcula las coordenadas de textura (puedes configurarlas seg�n tus necesidades)
            float texCoordX = static_cast<float>(j) / slices;
            float texCoordY = static_cast<float>(i) / stacks;
            vertices.push_back(texCoordX);
            vertices.push_back(texCoordY);
        }
    }

    // Genera los �ndices para dibujar la esfera
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int first = (i * (slices + 1)) + j;
            int second = first + slices + 1;
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }

    // Crea y configura los b�feres de v�rtices y elementos
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Configura los atributos de los v�rtices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Dibuja la esfera
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}

void GDGizmos::DrawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec3 &color, float Width)
{
    glLineWidth(Width);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, size * 2.0f);

    glUseProgram(shaderProgram);

    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Graphics::get_main_camera()->GetView()));

    // Enviar la matriz de proyecci�n al shader
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(Graphics::get_main_camera()->GetProjectionMatrix()));

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, glm::value_ptr(color));

    // Dibujar el cubo
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // Utilizar GL_LINES y 24 �ndices para dibujar el cubo con l�neas
}

void GDGizmos::Draw()
{
}

// std::vector<float> vertices;
// std::vector<unsigned int> indices;

// int stacks = 20;  // N�mero de "bandas" en la c�psula
// int slices = 20;  // N�mero de "rebanadas" en la c�psula

// float halfHeight = height / 2.0f;

//// Dibuja el cilindro
// for (int i = 0; i <= stacks; ++i) {
//     float y = (static_cast<float>(i) / stacks) * height - halfHeight;
//     float radiusAtY = radius;

//    for (int j = 0; j <= slices; ++j) {
//        float phi = j * 2 * glm::pi<float>() / slices;
//        float sinPhi = sin(phi);
//        float cosPhi = cos(phi);

//        float x = radiusAtY * cosPhi;
//        float z = radiusAtY * sinPhi;

//        // Agrega el v�rtice a la lista
//        vertices.push_back(x);
//        vertices.push_back(y);
//        vertices.push_back(z);

//        // Calcula las coordenadas de textura (puedes configurarlas seg�n tus necesidades)
//        float texCoordX = static_cast<float>(j) / slices;
//        float texCoordY = static_cast<float>(i) / stacks;
//        vertices.push_back(texCoordX);
//        vertices.push_back(texCoordY);
//    }
//}

//// Genera los �ndices para dibujar el cilindro
// for (int i = 0; i < stacks; ++i) {
//     for (int j = 0; j < slices; ++j) {
//         int first = (i * (slices + 1)) + j;
//         int second = first + slices + 1;

//        // Dibuja los tri�ngulos que forman el cilindro
//        indices.push_back(first);
//        indices.push_back(second);
//        indices.push_back(first + 1);

//        indices.push_back(second);
//        indices.push_back(second + 1);
//        indices.push_back(first + 1);
//    }
//}

//// Dibuja las medias esferas arriba y abajo
// DrawHalfSphere(position + glm::vec3(0.0f, height + radius, 0.0f), radius, color);
// DrawHalfSphere(position + glm::vec3(0.0f, -height + radius, 0.0f), radius, color);

//// Configura los b�feres de v�rtices e �ndices
// GLuint VAO, VBO, EBO;
// glGenVertexArrays(1, &VAO);
// glGenBuffers(1, &VBO);
// glGenBuffers(1, &EBO);

//// Enlaza el VAO
// glBindVertexArray(VAO);

//// Enlaza y configura el b�fer de v�rtices (posiciones y coordenadas de textura)
// glBindBuffer(GL_ARRAY_BUFFER, VBO);
// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

//// Configura los atributos de los v�rtices
//// Atributo 0: Posiciones de los v�rtices
// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
// glEnableVertexAttribArray(0);

//// Atributo 1: Coordenadas de textura
// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
// glEnableVertexAttribArray(1);

//// Enlaza y configura el b�fer de elementos
// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

//// Finalmente, puedes dibujar la c�psula llamando a glDrawElements.
// glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);