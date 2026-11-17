#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;
uniform vec3 hitFace;


void emitVertex(vec3 offset) {
    vec4 localPos = gl_in[0].gl_Position + vec4(offset, 0.0);
    
    gl_Position = projection * view * model * localPos;
    
    EmitVertex();
}


void main(){
    vec3 n = hitFace;
    
    if (n.x > 0.5) { // +X
        emitVertex(vec3(1.0, 0.0, 0.0));
        emitVertex(vec3(1.0, 1.0, 0.0));
        emitVertex(vec3(1.0, 0.0, 1.0));
        emitVertex(vec3(1.0, 1.0, 1.0));
    }
    else if (n.x < -0.5) { // -X
        emitVertex(vec3(0.0, 0.0, 1.0));
        emitVertex(vec3(0.0, 1.0, 1.0));
        emitVertex(vec3(0.0, 0.0, 0.0));
        emitVertex(vec3(0.0, 1.0, 0.0));
    }
    else if (n.y > 0.5) { // +Y
        emitVertex(vec3(0.0, 1.0, 1.0));
        emitVertex(vec3(0.0, 1.0, 0.0));
        emitVertex(vec3(1.0, 1.0, 1.0));
        emitVertex(vec3(1.0, 1.0, 0.0));
    }
    else if (n.y < -0.5) { // -Y
        emitVertex(vec3(0.0, 0.0, 0.0));
        emitVertex(vec3(0.0, 0.0, 1.0));
        emitVertex(vec3(1.0, 0.0, 0.0));
        emitVertex(vec3(1.0, 0.0, 1.0));
    }
    else if (n.z > 0.5) { // +Z
        emitVertex(vec3(0.0, 0.0, 1.0));
        emitVertex(vec3(0.0, 1.0, 1.0));
        emitVertex(vec3(1.0, 0.0, 1.0));
        emitVertex(vec3(1.0, 1.0, 1.0));
    }
    else if (n.z < -0.5) { // -Z
        emitVertex(vec3(1.0, 0.0, 0.0));
        emitVertex(vec3(1.0, 1.0, 0.0));
        emitVertex(vec3(0.0, 0.0, 0.0));
        emitVertex(vec3(0.0, 1.0, 0.0));
    }

    EndPrimitive();
}