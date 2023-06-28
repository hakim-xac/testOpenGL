#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoordBackground;
layout (location = 2) in vec2 aTexCoordShip;

out vec2 TexCoordBackground;
out vec2 TexCoordShip;

uniform mat4 transform;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	TexCoordBackground = vec2(aTexCoordBackground.x, aTexCoordBackground.y);
	TexCoordShip = vec2(aTexCoordShip.x, aTexCoordShip.y);
}