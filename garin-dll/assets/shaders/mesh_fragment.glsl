#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D sampler_texture;
uniform vec3 ambientLightColor; 
uniform vec3 lightDir;         
uniform vec3 lightColor;        
uniform bool castambiencelight;

void main()
{
    vec3 ambient = ambientLightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * texture(sampler_texture, TexCoords).rgb;
    
    if (castambiencelight){
        FragColor = vec4(result, 1.0);
    }else{
        FragColor = texture(sampler_texture, TexCoords);
    }
}
