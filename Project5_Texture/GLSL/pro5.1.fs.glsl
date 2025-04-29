#version 430
out vec4 color;
in vec2 tc;
uniform vec4 mv_matrix;
uniform vec4 proj_matrix;
in vec2 varyingTexCoord;
layout(binding = 0) uniform sampler2D samp;
void main(void)
{
    color = texture(samp, tc);
    // color = varyingColor;
}