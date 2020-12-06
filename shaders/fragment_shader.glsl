#version 330

in vec2 nextTexture;
out vec4 FragColor;

uniform sampler2D texture_;

void main() {
    FragColor = texture(texture_, vec2(nextTexture.x, nextTexture.y));
}
