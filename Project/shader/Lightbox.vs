#version 430

in layout(location = 0) vec3 vertexPosition_modelspace;

uniform mat4 PM;
uniform mat4 MM;
uniform mat4 VM;
uniform bool fog_flag;
uniform float FogDensity;
uniform float FogGradient;

out float visibility;

void main()
{
	vec4 v = vec4(vertexPosition_modelspace, 1);
	vec4 ViewPosition = VM * MM * v;
	gl_Position = PM * ViewPosition;

	if (fog_flag)
	{
		float distance = length(ViewPosition);
		visibility = exp(-pow((distance * FogDensity), FogGradient));
		visibility = clamp(visibility, 0, 1);
	}
	else visibility = 1;
}
