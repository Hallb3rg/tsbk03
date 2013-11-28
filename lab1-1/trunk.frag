#version 150

uniform sampler2D texUnit;

in vec2 outTexCoord;
out vec4 fragColor;

void main(void) {

    vec4 col = texture(texUnit, outTexCoord);
    fragColor.r = max(col.r -1.0,0.0);
    fragColor.g = max(col.g -1.0,0.0);
    fragColor.b = max(col.b -1.0,0.0);

}
