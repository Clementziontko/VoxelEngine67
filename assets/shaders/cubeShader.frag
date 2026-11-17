#version 330 core


in VS_OUT {
    vec3 FragPos;
    vec3 Nor;
    vec2 TexCoords;
} fs_in;


out vec4 FragColor;

uniform bool blinn;
uniform bool gamma; 

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

struct Sun{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Sun sun;

struct PointLight{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform PointLight pl;

uniform vec3 viewPos;

vec3 CalculateDirLight(Sun light, vec3 normal, vec3 viewDir, bool blinn, bool gamma){
    vec3 lightDir = normalize(-light.direction);
    vec4 texColor = texture(texture_diffuse1, fs_in.TexCoords);
    vec3 albedo = pow(texColor.rgb, vec3(2.2));

    vec3 specularSample = texture(texture_specular1, fs_in.TexCoords).rgb * 0.02;

    float diff = max(dot(normal, lightDir), 0.0);

    float spec;
    if(blinn)
    {

    vec3 halfWayDir = normalize(viewDir + lightDir);
    spec = pow(max(dot(viewDir, halfWayDir), 0.0), 32.0);

    }
    else
    {

        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    }
    vec3 ambient;
    vec3 diffuse;
    if(gamma)
    {
         ambient  = light.ambient * albedo;
         diffuse  = light.diffuse * diff * albedo;

    }
    else
    {

         ambient  = light.ambient * vec3(texColor);
         diffuse  = light.diffuse * diff * vec3(texColor);
    }

    vec3 specular = light.specular * spec * specularSample;

    return vec3(ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, bool blinn){
    float distance    = length(light.position - fragPos);
    vec3 lightDir     = normalize(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  

    float diff = max(dot(normal, lightDir), 0.0);

    float spec;
    if(blinn)
    {

    vec3 halfWayDir = normalize(viewDir + lightDir);
    spec = pow(max(dot(viewDir, halfWayDir), 0.0), 32.0);

    }
    else
    {

        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    }

    vec3 ambient  = light.ambient * vec3(texture(texture_diffuse1, fs_in.TexCoords));
    vec3 diffuse  = light.diffuse * diff * vec3(texture(texture_diffuse1, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, fs_in.TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return vec3(ambient + diffuse + specular);
}

void main(){
    vec3 result;
    vec3 norm    = normalize(fs_in.Nor);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    result  = CalculateDirLight(sun, norm, viewDir, blinn, gamma);
    //result += CalculatePointLight(pl, norm, viewDir, fs_in.FragPos, blinn);

    if(gamma)
    {
        result    = pow(result, vec3(1.0/2.2));
        FragColor = vec4(result, texture(texture_diffuse1, fs_in.TexCoords).a);
    }else
    {
        FragColor = vec4(result, texture(texture_diffuse1, fs_in.TexCoords).a);
    } 
}