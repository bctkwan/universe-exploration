#version 430

in float visibility;

uniform vec3 lightColor;
uniform bool FogColor;

out vec4 daColor;

void main()
{
	vec3 Fog_Real_Color;
	if (FogColor)
	{
		Fog_Real_Color = vec3(1, 165.0/255.0, 0);
	}
	else
	{
		Fog_Real_Color = vec3(0.5, 0.5, 0.5);
	}
	vec3 finalColor = mix(Fog_Real_Color, lightColor, visibility);
	daColor = vec4(finalColor, 1.0);
}
