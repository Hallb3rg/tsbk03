#version 150
uniform sampler2D texUnit;
const float texSize=512;
in vec2 outTexCoord;
out vec4 fragColor;

void main(void)
{
    float offset = 2.0 / texSize;
    vec2 tc = outTexCoord;
    vec4 c = texture(texUnit, tc);
    tc.x = tc.x + offset;
    vec4 l = texture(texUnit, tc);
    tc.x = tc.x - 2.0*offset;
    vec4 r = texture(texUnit, tc);
    //tc.x = tc.x - offset;
    fragColor = (c + c + l + r) * 0.25;

}
