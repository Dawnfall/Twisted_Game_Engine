#pragma once
#include <string>

std::string lightsData =
"R(
struct PointLight
{
	float4 position;
	float4 direction;
	float4 lightColor;
	float intensity;
	float radius;
	float _padding[2];
};

struct SpotLight
{
	float4 position;
	float4 direction;
	float4 lightColor;
	float intensity;
	float radius;
	float inner;
	float outer;
};


layout(std140) uniform Lights
{
	DirLight dirLights[512];
	SpotLight spotLights[512];
	PointLight pointLights[512];
};

// Lambertian diffuse
float calcDiffuse(vec3 normal, vec3 lightDir) {
	return max(dot(normalize(normal), normalize(lightDir)), 0.0);
}

// Blinn-Phong specular
float calcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, float shininess) {
	vec3 H = normalize(normalize(lightDir) + normalize(viewDir)); // half vector
	return pow(max(dot(normalize(normal), H), 0.0), shininess);
}

// Optional: attenuation for point or spot lights
float calcAttenuation(float distance, float radius) {
	// simple linear attenuation
	return clamp(1.0 - distance / radius, 0.0, 1.0);
}

// Optional: spot light factor
float calcSpotFactor(vec3 lightDir, vec3 spotDir, float inner, float outer) {
	float cosTheta = dot(normalize(-lightDir), normalize(spotDir));
	return clamp((cosTheta - outer) / (inner - outer), 0.0, 1.0);
}

vec3 applyDirLight(vec3 normal, vec3 viewDir, vec3 lightDir, vec3 lightColor, float intensity, float shininess, vec3 albedo) {
	float diff = calcDiffuse(normal, -lightDir); // negative because lightDir points to light
	float spec = calcSpecular(normal, -lightDir, viewDir, shininess);
	return (diff + spec) * lightColor * intensity * albedo;
}

vec3 applyPointLight(
	vec3 fragPos,      // world space position of the fragment
	vec3 normal,       // normal at the fragment
	vec3 viewPos,      // camera position
	PointLight light,  // your struct
	float shininess,
	vec3 albedo
) {
	vec3 lightDir = light.position.xyz - fragPos;
	float distance = length(lightDir);
	lightDir = normalize(lightDir);

	// Diffuse and specular
	float diff = calcDiffuse(normal, lightDir);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = calcSpecular(normal, lightDir, viewDir, shininess);

	// Attenuation
	float attenuation = calcAttenuation(distance, light.radius);

	// Combine
	return (diff + spec) * light.lightColor.rgb * light.intensity * attenuation * albedo;
}

vec3 applySpotLight(
	vec3 fragPos,       // world-space fragment position
	vec3 normal,        // normal at the fragment
	vec3 viewPos,       // camera position
	SpotLight light,    // your struct
	float shininess,
	vec3 albedo
) {
	vec3 lightDir = light.position.xyz - fragPos;
	float distance = length(lightDir);
	lightDir = normalize(lightDir);

	// Spotlight cone factor
	float theta = dot(lightDir, normalize(-light.direction.xyz)); // angle between lightDir and light facing direction
	float epsilon = light.inner - light.outer;
	float intensity = clamp((theta - light.outer) / epsilon, 0.0, 1.0); // smoothstep can also be used

	// Diffuse and specular
	float diff = calcDiffuse(normal, lightDir);
	vec3 viewDir = normalize(viewPos - fragPos);
	float spec = calcSpecular(normal, lightDir, viewDir, shininess);

	// Attenuation by distance
	float attenuation = calcAttenuation(distance, light.radius);

	// Combine all factors
	return (diff + spec) * light.lightColor.rgb * light.intensity * attenuation * intensity * albedo;
}

vec3 calcLighting(vec3 fragPos, vec3 normal, vec3 viewPos, float shininess, vec3 albedo) {
	vec3 color = vec3(0.0);

	// Directional lights
	for (int i = 0; i < 512; ++i) {
		if (dirLights[i].intensity <= 0.0) break; // assume unused lights have 0 intensity
		vec3 lightDir = normalize(-dirLights[i].dir.xyz); // directional lights point from dir
		float diff = calcDiffuse(normal, lightDir);
		float spec = calcSpecular(normal, lightDir, normalize(viewPos - fragPos), shininess);
		color += (diff + spec) * dirLights[i].color.rgb * dirLights[i].intensity * albedo;
	}

	// Point lights
	for (int i = 0; i < 512; ++i) {
		if (pointLights[i].intensity <= 0.0) break;
		color += applyPointLight(fragPos, normal, viewPos, pointLights[i], shininess, albedo);
	}

	// Spot lights
	for (int i = 0; i < 512; ++i) {
		if (spotLights[i].intensity <= 0.0) break;
		color += applySpotLight(fragPos, normal, viewPos, spotLights[i], shininess, albedo);
	}

	return color;
}
)";