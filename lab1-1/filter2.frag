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
    tc.y = tc.y + offset;
    vec4 u = texture(texUnit, tc);
    tc.y = tc.y - 2.0*offset;
    vec4 d = texture(texUnit, tc);
    fragColor = (c + c + u + d) * 0.25;

}
