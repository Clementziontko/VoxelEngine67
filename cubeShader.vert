#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNor;
layout(location = 2) in vec2 aTexCoords;
//layout(location = 3) in vec3 offsets;


out VS_OUT {
    vec3 FragPos;
    vec3 Nor;
    vec2 TexCoords;
} vs_out;


layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;
uniform bool StencilTest;

void main(){
    vec3 localPos;
    /*
    if(StencilTest){
      localPos = aPos;
    }
    else{
     localPos = aPos + offsets;
    }
    */
    

    //vec4 worldPos = model * vec4(localPos, 1.0);

    //vs_out.FragPos = worldPos.xyz;
    vs_out.FragPos = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);

    vs_out.Nor = mat3(transpose(inverse(model))) * aNor;
    vs_out.TexCoords = aTexCoords;
}