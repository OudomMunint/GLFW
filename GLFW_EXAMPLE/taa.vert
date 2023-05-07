uniform sampler2D tex;
uniform vec2 pixelSize;
uniform float sharpness;

vec4 getSample(vec2 uv) {
    return texture(tex, uv);
}

vec4 getSampleBilinear(vec2 uv) {
    vec2 pixel = pixelSize;
    vec2 p = floor(uv/pixel) * pixel;
    vec2 f = fract(uv/pixel);
    vec4 A = getSample(p);
    vec4 B = getSample(p + vec2(pixel.x, 0.0));
    vec4 C = getSample(p + vec2(0.0, pixel.y));
    vec4 D = getSample(p + vec2(pixel.x, pixel.y));
    return mix(mix(A, B, f.x), mix(C, D, f.x), f.y);
}

vec4 taa(vec2 uv) {
    vec4 currentSample = getSampleBilinear(uv);
    vec4 lastSample = getSampleBilinear(uv - vec2(0.5) * pixelSize);
    vec4 blendedSample = mix(currentSample, lastSample, sharpness);
    return blendedSample;
}

void main() {
    gl_FragColor = taa(gl_FragCoord.xy);
}