#version 330
//FXAA
uniform sampler2D textureSampler;
uniform vec2 inverseResolution;

in vec2 uvCoord;

out vec4 outColor;

vec3 luma = vec3(0.299, 0.587, 0.114);

void main() 
{
    vec2 texelSize = vec2(1.0) / inverseResolution;
    vec3 color = texture(textureSampler, uvCoord).rgb;

    vec3 colorN = texture(textureSampler, uvCoord + vec2(0.0, -1.0) * texelSize).rgb;
    vec3 colorS = texture(textureSampler, uvCoord + vec2(0.0, 1.0) * texelSize).rgb;
    vec3 colorW = texture(textureSampler, uvCoord + vec2(-1.0, 0.0) * texelSize).rgb;
    vec3 colorE = texture(textureSampler, uvCoord + vec2(1.0, 0.0) * texelSize).rgb;
    
    float lumaN = dot(colorN, luma);
    float lumaS = dot(colorS, luma);
    float lumaW = dot(colorW, luma);
    float lumaE = dot(colorE, luma);
    float lumaM = dot(color, luma);
    
    float minLuma = min(lumaM, min(min(lumaN, lumaS), min(lumaW, lumaE)));
    float maxLuma = max(lumaM, max(max(lumaN, lumaS), max(lumaW, lumaE)));
    
    vec2 dir = vec2(-((lumaE + lumaW) - (lumaN + lumaS)), ((lumaN + lumaS) - (lumaE + lumaW)));
    float dirReduce = max((maxLuma - minLuma) / maxLuma, 0.0);
    float edgeBlend = clamp((maxLuma - minLuma) / (0.25 * maxLuma), 0.0, 1.0);
    
    float lumaP = dot(texture(textureSampler, uvCoord + dir * (1.0 / 3.0 - 0.5)).rgb, luma);
    float lumaQ = dot(texture(textureSampler, uvCoord + dir * (2.0 / 3.0 - 0.5)).rgb, luma);
    
    float gradient = abs(lumaQ - lumaP);
    float threshold = max(0.0312, 0.063 * dirReduce);
    
    if (gradient < threshold)
    {
        edgeBlend = 0.0;
    }
    
    outColor.rgb = mix(color, mix(vec3(0.0), color, edgeBlend), 0.5);
    outColor.a = 1.0;
}
