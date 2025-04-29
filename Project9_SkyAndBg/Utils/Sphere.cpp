#include "./Sphere.h"


Sphere::Sphere(int prec)
{
    init(prec);
}

Sphere::~Sphere()
{

}

float Sphere::toRadians(float degrees)
{
    return degrees * (M_PI / 180.0f);
}

void Sphere::init(int prec)
{
    numVertices = (prec+1) * (prec+1);
    numIndices = prec * prec * 6;
    // push back vertices, normals, and texCoords
    for (int i = 0;i< numVertices; i++)
    {
        vertices.push_back(glm::vec3(0.0f));
        normals.push_back(glm::vec3(0.0f));
        texCoords.push_back(glm::vec2(0.0f));
    }
    for (int i = 0; i < numIndices; i++)
    {
        indices.push_back(0);
    }
    // TODO:calculate vertices, normals, and texCoords
    for (int  i = 0; i <= prec; i++)
    {
        for (int j = 0; j <= prec; j++)
        {
            float y = (float)cos(toRadians(180.0f - i * 180.0f / prec));
            float x = -(float)cos(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
            float z = (float)sin(toRadians(j * 360.0f / prec)) * (float)abs(cos(asin(y)));
            vertices[i*(prec+1)+j] = glm::vec3(x, y, z);
            normals[i*(prec+1)+j] = glm::vec3(x, y, z);
            texCoords[i*(prec+1)+j] = glm::vec2(((float)j / prec), ((float)i / prec));
            // theoretical indices
        }
    }
    // TODO:calculate indices
    for (int i = 0; i < prec; i++)
    {
        for (int j = 0; j < prec; j++)
        {
            indices[6*(i*prec+j)+0] = i*(prec+1)+j;
            indices[6*(i*prec+j)+1] = i*(prec+1)+j+1;
            indices[6*(i*prec+j)+2] = (i+1)*(prec+1)+j;
            indices[6*(i*prec+j)+3] = i*(prec+1)+j+1;
            indices[6*(i*prec+j)+4] = (i+1)*(prec+1)+j+1;
            indices[6*(i*prec+j)+5] = (i+1)*(prec+1)+j;
        }
    }
}

int Sphere::getNumVertices()
{
    return numVertices;
}

int Sphere::getNumIndices()
{
    return numIndices;
}

std::vector<int> Sphere::getIndices()
{
    return indices;
}

std::vector<glm::vec3> Sphere::getVertices()
{
    return vertices;
}

std::vector<glm::vec2> Sphere::getTexCoords()
{
    return texCoords;
}

std::vector<glm::vec3> Sphere::getNormals()
{
    return normals;
}

