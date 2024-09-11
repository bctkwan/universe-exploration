#version 430

in vec2 UV;
in vec3 normalWorld;
in float visibility;
in vec3 LightDirection1;
in vec3 LightDirection2;
in vec3 EyeDirection;

out vec4 daColor;

uniform sampler2D myTextureSampler_1;
uniform sampler2D myTextureSampler_2;
uniform bool normalMapping_flag;
uniform bool multiTexMapping_flag;
uniform vec3 ambientLight;
uniform vec3 coefficient_d;
uniform vec3 coefficient_s;
uniform vec3 lightColor_1;
uniform vec3 lightColor_2;
uniform bool FogColor;

void main()
{
	vec3 normal = normalize(normalWorld);
	if (normalMapping_flag)
	{
		normal = texture(myTextureSampler_2, UV).rgb;
		normal = normalize(normal * 2.0 - 1.0);
	}

	vec3 Material_Clr = texture( myTextureSampler_1, UV).rgb;
	if (multiTexMapping_flag){
		Material_Clr = (0.3*texture( myTextureSampler_1, UV) + 0.7*texture( myTextureSampler_2, UV)).rgb;
	}

	vec3 lightVectorWorld_1 = normalize(LightDirection1);
	vec3 lightVectorWorld_2 = normalize(LightDirection2);
	float diffuse_1 = clamp(dot(lightVectorWorld_1, normal), 0, 1);
	float diffuse_2 = clamp(dot(lightVectorWorld_2, normal), 0, 1);

	vec3 reflectedLightVectorWorld_1 = reflect(-lightVectorWorld_1, normal);
	vec3 reflectedLightVectorWorld_2 = reflect(-lightVectorWorld_2, normal);
	vec3 eyeVectorWorld = normalize(EyeDirection);
	float specular_1 = pow(clamp(dot(reflectedLightVectorWorld_1, eyeVectorWorld), 0, 1), 50);
	float specular_2 = pow(clamp(dot(reflectedLightVectorWorld_2, eyeVectorWorld), 0, 1), 50);
	
	vec3 finalColor = Material_Clr * (ambientLight + (lightColor_1 * diffuse_1  + lightColor_2 * diffuse_2) * coefficient_d + (lightColor_1 * specular_1 + lightColor_2 * specular_2) * coefficient_s);
	
	vec3 Fog_Real_Color;
	if (FogColor)
	{
		Fog_Real_Color = vec3(1, 165.0/255.0, 0);
	}
	else
	{
		Fog_Real_Color = vec3(0.5, 0.5, 0.5);
	}
	finalColor = mix(Fog_Real_Color, finalColor, visibility);
	daColor = vec4(finalColor, 1.0);
}
