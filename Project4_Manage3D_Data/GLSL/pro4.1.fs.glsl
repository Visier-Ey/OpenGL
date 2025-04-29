#version 430
out vec4 color;
in vec4 varyingColor;
uniform vec4 mv_matrix;
uniform vec4 proj_matrix;

void main(void)
{
    color = varyingColor;
}