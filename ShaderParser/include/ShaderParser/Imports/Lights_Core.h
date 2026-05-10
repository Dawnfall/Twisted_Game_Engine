#pragma once
#include <string>

namespace Twisted
{
	inline const std::string Lights_Core_Frag =
R"(struct DirLight
{
    vec4  direction;
    vec4  lightColor;
    float intensity;
    float _pad0; float _pad1; float _pad2;
};

struct PointLight
{
    vec4  position;
    vec4  lightColor;
    float intensity;
    float radius;
    float _pad0; float _pad1;
};

struct SpotLight
{
    vec4  position;
    vec4  direction;
    vec4  lightColor;
    float intensity;
    float radius;
    float innerCos;
    float outerCos;
};

layout(std140, binding = 1) uniform ShaderLightsBuffer
{
    DirLight   dirLights[8];
    int        dirLightCount;

    PointLight pointLights[64];
    int        pointLightCount;

    SpotLight  spotLights[32];
    int        spotLightCount;

    vec4 uAmbientLight; // xyz = color, w = intensity
};

vec3 calcAmbient()
{
    return uAmbientLight.rgb * uAmbientLight.w;
}

float calcDiffuse(vec3 normal, vec3 lightDir)
{
    return max(dot(normalize(normal), normalize(lightDir)), 0.0);
}

float calcSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, float shininess)
{
    vec3 H = normalize(normalize(lightDir) + normalize(viewDir));
    return pow(max(dot(normalize(normal), H), 0.0), shininess);
}

// Smooth quadratic window: full brightness at center, drops cleanly to 0 at radius.
float calcAttenuation(float dist, float radius)
{
    float x = clamp(1.0 - (dist * dist) / (radius * radius), 0.0, 1.0);
    return x * x;
}

vec3 applyDirLight(vec3 normal, vec3 viewDir, DirLight light, float shininess)
{
    vec3  lightDir = normalize(-light.direction.xyz);
    float diff     = calcDiffuse(normal, lightDir);
    float spec     = calcSpecular(normal, lightDir, viewDir, shininess);
    vec3  lit      = light.lightColor.rgb * light.intensity;
    return (diff + spec) * lit;
}

vec3 applyPointLight(vec3 fragPos, vec3 normal, vec3 viewDir, PointLight light, float shininess)
{
    vec3  toLight  = light.position.xyz - fragPos;
    float dist     = max(length(toLight), 0.0001);
    vec3  lightDir = toLight / dist;
    float diff     = calcDiffuse(normal, lightDir);
    float spec     = calcSpecular(normal, lightDir, viewDir, shininess);
    vec3  lit      = light.lightColor.rgb * light.intensity * calcAttenuation(dist, light.radius);
    return (diff + spec) * lit;
}

vec3 applySpotLight(vec3 fragPos, vec3 normal, vec3 viewDir, SpotLight light, float shininess)
{
    vec3  toLight  = light.position.xyz - fragPos;
    float dist     = max(length(toLight), 0.0001);
    vec3  lightDir = toLight / dist;
    float cosTheta = dot(lightDir, normalize(-light.direction.xyz));
    float epsilon  = light.innerCos - light.outerCos;
    float spot     = clamp((cosTheta - light.outerCos) / epsilon, 0.0, 1.0);
    float diff     = calcDiffuse(normal, lightDir);
    float spec     = calcSpecular(normal, lightDir, viewDir, shininess);
    vec3  lit      = light.lightColor.rgb * light.intensity * calcAttenuation(dist, light.radius) * spot;
    return (diff + spec) * lit;
}

// Returns raw lighting contribution (no albedo). Multiply by albedo in the shader.
vec3 calcLighting(vec3 fragPos, vec3 normal, vec3 viewPos, float shininess)
{
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 color   = calcAmbient();
    for (int i = 0; i < dirLightCount;   ++i)
        color += applyDirLight(normal, viewDir, dirLights[i], shininess);
    for (int i = 0; i < pointLightCount; ++i)
        color += applyPointLight(fragPos, normal, viewDir, pointLights[i], shininess);
    for (int i = 0; i < spotLightCount;  ++i)
        color += applySpotLight(fragPos, normal, viewDir, spotLights[i], shininess);
    return color;
}
)";
}
