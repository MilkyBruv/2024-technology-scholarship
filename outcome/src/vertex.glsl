#version 330 core

// Input vertex attributes
layout(location = 0) in vec3 position; // Vertex position
layout(location = 1) in vec2 texCoord; // Vertex texture coordinate

// Output to the fragment shader
out vec2 fragTexCoord;

// Uniforms
uniform mat4 mvp; // Model-View-Projection matrix

void main()
{
    // Pass the texture coordinate to the fragment shader
    fragTexCoord = texCoord;

    // Transform the vertex position by the MVP matrix
    gl_Position = mvp * vec4(position, 1.0);
}

