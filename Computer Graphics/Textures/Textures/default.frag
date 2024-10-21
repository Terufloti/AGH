#version 330 core
in vec3 vertexColor;
in vec2 vertexTexture;
out vec4 fragmentColor;
uniform sampler2D uTexture;
uniform float scrollUpMix;
void main() {
    fragmentColor = mix(texture(uTexture, vertexTexture), vec4(0.0, 0.0, 1.0, 1.0), scrollUpMix);
}