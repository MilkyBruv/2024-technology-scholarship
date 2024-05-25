#version 330 core

// Input from the vertex shader
in vec2 fragTexCoord;

// Output color
out vec4 finalColor;

// Uniforms
uniform sampler2D screenTexture; // Screen texture

void main()
{
	vec4 col = texture(screenTexture, fragTexCoord);
    // Sample the screen texture
    col.r = col.r;
    col.g = col.b;
    col.b = col.b;
    finalColor = col;
}

