#version 430

in vec3 TexCoords;
in float visibility;

out vec4 color;

uniform samplerCube skybox;
uniform bool FogColor;

void main()
{
	vec4 finalColor = texture(skybox, vec3(TexCoords.x, -TexCoords.y, TexCoords.z));
	vec3 Fog_Real_Color;
	if (FogColor)
	{
		Fog_Real_Color = vec3(1, 165.0/255.0, 0);
	}
	else
	{
		Fog_Real_Color = vec3(0.5, 0.5, 0.5);
	}
	color = mix(vec4(Fog_Real_Color, 1), finalColor, visibility);
}
