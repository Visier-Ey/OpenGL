#version 430
layout(location = 0) in vec3 position;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;

out vec4 varyingColor;

mat4 buildRotateX(float angle);
mat4 buildRotateY(float angle);
mat4 buildRotateZ(float angle);
mat4 buildTranslate(float x, float y, float z);

void main(void)
{
    float i =gl_InstanceID + tf;
    float a = sin(203.0*i/8000.0)*403.0;
    float b = sin(301.0*i/4001.0)*401.0;
    float c = sin(400.0*i/6003.0)*405.0;

    mat4 translate = buildTranslate(a, b, c);
    mat4 rotateX = buildRotateX(i*3);
    mat4 rotateY = buildRotateY(i*3);
    mat4 rotateZ = buildRotateZ(i*3);

    mat4 model = translate * rotateX * rotateY * rotateZ;
    mat4 mvp = proj_matrix * v_matrix * model;

    gl_Position = mvp * vec4(position, 1.0);
    varyingColor = vec4(position * 0.5 + 0.5, 1.0);
}

mat4 buildTranslate(float x, float y, float z) {
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                x, y, z, 1.0);
}

mat4 buildRotateX(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, c, -s, 0.0,
                0.0, s, c, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 buildRotateY(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(c, 0.0, s, 0.0,
                0.0, 1.0, 0.0, 0.0,
                -s, 0.0, c, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 buildRotateZ(float angle) {
    float c = cos(angle);
    float s = sin(angle);
    return mat4(c, -s, 0.0, 0.0,
                s, c, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                0.0, 0.0, 0.0, 1.0);
}

mat4 buildScale(float x, float y, float z) {
    return mat4(x, 0.0, 0.0, 0.0,
                0.0, y, 0.0, 0.0,
                0.0, 0.0, z, 0.0,
                0.0, 0.0, 0.0, 1.0);
}