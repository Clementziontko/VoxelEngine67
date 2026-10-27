#version 330 core

layout(points) in;
layout(line_strip, max_vertices = 24) out;


layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

void main(){
    gl_Position = projection * view * model * gl_in[0].gl_Position; 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 0.0, 1.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * gl_in[0].gl_Position; 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * gl_in[0].gl_Position; 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 0.0, 1.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 0.0, 1.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 1.0, 1.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 0.0, 1.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 0.0, 1.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 0.0, 1.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 1.0, 1.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 1.0, 1.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 1.0, 1.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 1.0, 1.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0)); 
    EmitVertex();

    EndPrimitive();

    
    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 1.0, 0.0, 0.0)); 
    EmitVertex();

    gl_Position = projection * view * model * (gl_in[0].gl_Position + vec4(0.0, 1.0, 1.0, 0.0)); 
    EmitVertex();

    EndPrimitive();





}