#version 330 core
out vec4 FragColor;

in vec3 bNormal;
in vec2 TexCoords;
in vec3 FragPos;
uniform vec3 fragPos;

uniform vec3 dirlight;
uniform sampler2D texture_sampler;
uniform sampler2D texture_normal;
uniform mat4 model;

uniform float light_intensity = 5.0f;
uniform float diffuse_intensity = 0.5f;
uniform float normal_intensity = 0.5f;

vec3 CalculateDirLight(vec3 Normal, vec3 lightDirection);
vec3 CalculatePointLight(vec3 Normal, vec3 lightPosition, float constant, float linear, float quadratic);

void main()
{
    vec3 result_dir_light = CalculateDirLight(bNormal, dirlight);
    
    vec3 point_light_position = vec3(0.0, 5.0, 0.0); 
    
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
    
    vec3 result_point_light = CalculatePointLight(bNormal, fragPos, constant, linear, quadratic);

     vec3 finalColor = result_dir_light + result_point_light;
    
    FragColor = vec4(finalColor, 1.0);
}

vec3 CalculateDirLight(vec3 Normal, vec3 lightDirection)
{
    vec3 N = normalize(model * vec4(Normal, 1.0)).xyz;
    float intensity = max(dot(N, normalize(lightDirection)), 0.0);

    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = vec3(diffuse_intensity);

    float light = 0.1 + 0.9 * intensity * light_intensity;

    vec3 texColor = texture(texture_sampler, TexCoords).rgb * lightColor;
    vec3 finalColor = texColor * light * diffuse;

    return finalColor;
}

vec3 CalculatePointLight(vec3 Normal, vec3 lightPosition, float constant, float linear, float quadratic)
{
    vec3 N = normalize(model * vec4(Normal, 1.0)).xyz;
    vec3 lightDir = normalize(lightPosition - FragPos);

    float distance = length(lightPosition - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    
    float intensity = max(dot(N, lightDir), 0.0);

    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = vec3(diffuse_intensity);

    float light = 0.1 + 0.9 * intensity * light_intensity;

    vec3 texColor = texture(texture_sampler, TexCoords).rgb * lightColor;
    vec3 finalColor = texColor * light * diffuse * attenuation;

    return finalColor;
}
