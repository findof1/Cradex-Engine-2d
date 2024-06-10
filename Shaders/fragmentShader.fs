#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 TexCoord;
uniform int textureActive;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixVal;
uniform vec4 overrideColor;

void main() {
    if(textureActive == 1) {
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixVal);
    } else if(overrideColor != vec4(-1, -1, -1, -1)) {
        FragColor = overrideColor;
    } else {
        FragColor = ourColor;
    }

}
