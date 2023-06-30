#version 330 core
out vec4 FragColor;

in vec2 TexCoordBackground;
in vec2 TexCoordShip;

// Текстурные сэмплеры
uniform sampler2D texture_background;
uniform sampler2D texture_ship;

uniform float overlay;

void main()
{
	FragColor = mix(texture(texture_background, TexCoordBackground), texture(texture_ship, TexCoordShip), overlay);
	//FragColor = texture(texture1, TexCoord);
	//FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);

}