#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 TexCoord;
uniform int textureActive;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;
uniform sampler2D texture5;
uniform sampler2D texture6;
uniform sampler2D texture8;
uniform sampler2D texture7;
uniform vec4 overrideColor;

void main() {
    if(textureActive == 1) {
        FragColor = texture(texture1, TexCoord);
    } else if(textureActive == 2) {
        FragColor = texture(texture2, TexCoord);
    } else if(textureActive == 3) {
        FragColor = texture(texture3, TexCoord);
    } else if(textureActive == 4) {
        FragColor = texture(texture4, TexCoord);
    } else if(textureActive == 5) {
        FragColor = texture(texture5, TexCoord);
    } else if(textureActive == 6) {
        FragColor = texture(texture6, TexCoord);
    } else if(textureActive == 7) {
        FragColor = texture(texture7, TexCoord);
    } else if(textureActive == 8) {
        FragColor = texture(texture8, TexCoord);
    } else if(overrideColor.w >= 0) {
        FragColor = overrideColor;
    } else {
        FragColor = ourColor;
    }

}
