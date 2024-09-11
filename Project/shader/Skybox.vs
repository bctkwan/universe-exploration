#version 430

layout(location = 0) in vec3 position;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 M;
uniform mat4 VM;
uniform bool fog_flag;
uniform float FogDensity;
uniform float FogGradient;

out vec3 TexCoords;
out float visibility;

void main()
{
    vec4 pos = projection * view * M * vec4(position, 1.0);
	gl_Position = pos;

	if (fog_flag)
	{
		float distance = length(VM * M * vec4(position, 1.0));
		visibility = exp(-pow((distance * FogDensity), FogGradient));
		visibility = clamp(visibility, 0, 1);
	}
	else visibility = 1;

	TexCoords = position;
}