#version 150

in vec3 in_Position;
in vec2 in_TexCoord;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

out vec2 outTexCoord;

void main(void)
{
	outTexCoord = in_TexCoord;
	gl_Position = projMatrix * viewMatrix * vec4(in_Position, 1.0);
}
