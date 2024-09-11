#version 430

in layout(location = 0) vec3 vertexPosition_modelspace;
in layout(location = 1) vec2 vertexUV;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec3 vertexTangent_modelspace;
in layout(location = 4) vec3 vertexBitangent_modelspace;

uniform mat4 PM;
uniform mat4 MM;
uniform mat4 VM;
uniform mat3 MV3x3;
uniform bool fog_flag;
uniform float FogDensity;
uniform float FogGradient;
uniform bool normalMapping_flag;
uniform vec3 eyePositionWorld;
uniform vec3 lightPositionWorld_1;
uniform vec3 lightPositionWorld_2;

out vec3 normalWorld;
out vec2 UV;
out float visibility;
out vec3 LightDirection1;
out vec3 LightDirection2;
out vec3 EyeDirection;

void main()
{
    vec4 v = vec4(vertexPosition_modelspace, 1);
	vec4 newPosition = MM * v;
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

	vec3 vertexPositionWorld = newPosition.xyz;

	if (normalMapping_flag)
	{
		vec3 EyeDirection_cameraspace = vec3(0,0,0) - ViewPosition.xyz;

		vec3 LightPosition1_cameraspace = ( VM * vec4(lightPositionWorld_1,1)).xyz;
		vec3 LightDirection1_cameraspace = LightPosition1_cameraspace + EyeDirection_cameraspace;
		vec3 LightPosition2_cameraspace = ( VM * vec4(lightPositionWorld_2,1)).xyz;
		vec3 LightDirection2_cameraspace = LightPosition2_cameraspace + EyeDirection_cameraspace;

		vec3 vertexTangent_cameraspace = MV3x3 * normalize(vertexTangent_modelspace);
		vec3 vertexBitangent_cameraspace = MV3x3 * normalize(vertexBitangent_modelspace);
		vec3 vertexNormal_cameraspace = MV3x3 * normalize(normal);

		mat3 TBN = transpose(mat3(
			vertexTangent_cameraspace,
			vertexBitangent_cameraspace,
			vertexNormal_cameraspace	
		));

		LightDirection1 = TBN * LightDirection1_cameraspace;
		LightDirection2 = TBN * LightDirection2_cameraspace;
		EyeDirection =  TBN * EyeDirection_cameraspace;
	}
	else
	{
		LightDirection1 = lightPositionWorld_1 - vertexPositionWorld;
		LightDirection2 = lightPositionWorld_2 - vertexPositionWorld;
		EyeDirection =  eyePositionWorld - vertexPositionWorld;
	}
}
