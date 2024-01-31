#version 330 core


uniform vec3 position;
uniform vec3 lcolor;
uniform vec3 direction;
uniform float intensity;
uniform float kc; 
uniform float kl;
uniform float kq;
uniform float ambientIntensity;
uniform float specularIntensity;

uniform vec3 viewPosition;

in vec3 Normal;

in vec4 ModelPosition;

in vec2 Tex;

uniform sampler2D Texture;

out vec4 FragmentColor;

uniform vec3 color;

vec3 PointLightFactor(vec3 normal, int index);

void main()
{
	vec3 result3;

	result3 = (PointLightFactor(Normal, 2));

	if(color.r == 0.0f && color.g == 0.0f && color.b == 0.0f)
	{
		FragmentColor = vec4(texture(Texture, Tex).rgb, 0.0f)  * vec4(result3, 1.0f) * 0.5f;
	}
	else
	{
		FragmentColor = vec4(color, 0.0f)  * vec4(result3, 1.0f) * 0.5f;
	}

}

vec3 PointLightFactor(vec3 normal, int index)
{
	vec3 lightDirection = position - ModelPosition.xyz;

	vec3 ambient = ambientIntensity * vec3(1.0f);

	vec3 norm = normal.xyz;

	float d = length(lightDirection);

	float attenuation =	1.0f /	(kc +
							   kl * d +
                                  kq * d * d);
	vec3 N = normalize(norm);
	vec3 L = normalize(lightDirection);

	vec3 diffuse = max(dot(N, L), 0.0) * intensity * lcolor * attenuation;

	vec3 V = normalize(viewPosition - ModelPosition.xyz);
	vec3 R = reflect(-L, N);

	vec3 specular = pow(max(dot(V, R), 0.0), 64.0f) * lcolor * attenuation * specularIntensity;

	vec3 result = (ambient + diffuse + specular);
	return result;
}
