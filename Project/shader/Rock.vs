#version 430

in layout(location = 0) vec3 vertexPosition_modelspace;
in layout(location = 1) vec2 vertexUV;
in layout(location = 2) vec3 normal;
in layout(location = 3) mat4 instanceMatrix;

uniform mat4 PM;
uniform mat4 MM;
uniform mat4 VM;
uniform bool fog_flag;
uniform float FogDensity;
uniform float FogGradient;

out vec3 normalWorld;
out vec2 UV;
out vec3 vertexPositionWorld;
out float visibility;

void main()
{
    vec4 v = vec4(vertexPosition_modelspace, 1);
	vec4 newPosition = MM * instanceMatrix * v;
	vec4 ViewPosition = VM * newPosition;
	gl_Position = PM * ViewPosition;

	if (fog_flag)
	{
		float distance = length(ViewPosition);
		visibility = exp(-pow((distance * FogDensity), FogGradient));
		visibility = clamp(visibility, 0, 1);
	}
	else visibility = 1;

	vec4 normal_temp = MM * vec4(normal, 0);
	normalWorld = normal_temp.xyz;

	UV = vertexUV;
	vertexPositionWorld = newPosition.xyz;
}