#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D _MainTexture;

void main()
{    
    if (texture(_MainTexture, TexCoords).a < 0.1)
        discard;

    FragColor = texture(_MainTexture, TexCoords);
}