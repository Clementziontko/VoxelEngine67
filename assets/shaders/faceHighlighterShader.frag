#version 330 core
out vec4 FragColor;

uniform vec4 hitFaceColor;

void main()
{
    FragColor = hitFaceColor; 
}  