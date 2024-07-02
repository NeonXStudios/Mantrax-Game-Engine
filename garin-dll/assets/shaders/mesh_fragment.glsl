#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D sampler_texture;
uniform vec3 ambientLightColor; // Color de la luz ambiental
uniform vec3 lightDir;          // Dirección de la luz
uniform vec3 lightColor;        // Color de la luz direccional

void main()
{
    // Iluminación ambiental
    vec3 ambient = ambientLightColor;

    // Iluminación direccional
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    // Combina la iluminación ambiental y direccional con el color de la textura
    vec3 result = (ambient + diffuse) * texture(sampler_texture, TexCoords).rgb;
    
    //FragColor = vec4(result, 1.0);

    FragColor = texture(sampler_texture, TexCoords);
}