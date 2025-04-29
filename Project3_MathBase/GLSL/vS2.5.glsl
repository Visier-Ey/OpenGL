#version 430
uniform float offset;
uniform float translateX, translateY, translateZ; 
uniform float rotateXAngle, rotateYAngle, rotateZAngle; 
uniform float scaleX, scaleY, scaleZ; 

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

void main(void)
{
    mat4 translationMatrix = buildTranslate(translateX, translateY, translateZ);
    mat4 rotationXMatrix = buildRotateX(rotateXAngle);
    mat4 rotationYMatrix = buildRotateY(rotateYAngle);
    mat4 rotationZMatrix = buildRotateZ(rotateZAngle);
    mat4 scaleMatrix = buildScale(scaleX, scaleY, scaleZ);
 
    mat4 transformMatrix = translationMatrix * rotationZMatrix * rotationYMatrix * rotationXMatrix * scaleMatrix;

    if (gl_VertexID == 0)
        gl_Position = transformMatrix * vec4(0.25 + offset, -0.25, 0.0, 1.0);
    else if (gl_VertexID == 1)
        gl_Position = transformMatrix * vec4(-0.25 + offset, -0.25, 0.0, 1.0);
    else 
        gl_Position = transformMatrix * vec4(offset, 0.1775, 0.0, 1.0);
}