#include "./Torus.h"

Torus::Torus(int prec=48,float inner=0.5f,float outer=0.2f){
    this->prec = prec;
    this->inner = inner;
    this->outer = outer;
    init();
}

Torus::~Torus(){}

float Torus::toRadians(float degrees){
    return degrees*3.14159f/180.0f;
}

int Torus::getNumVertices(){
    return numVertices;
}

int Torus::getNumIndices(){
    return numIndices;
}

std::vector<int> Torus::getIndices(){
    return indices;
}

std::vector<glm::vec3> Torus::getVertices(){
    return vertices;
}

std::vector<glm::vec3> Torus::getNormals(){
    return normals;
}

std::vector<glm::vec2> Torus::getTexCoords(){
    return texCoords;
}

std::vector<glm::vec3> Torus::getSTangents(){
    return sTangents;
}

std::vector<glm::vec3> Torus::getTTangents(){
    return tTangents;
}

void Torus::init(){
    numVertices = (prec+1)*(prec+1);
    numIndices = prec*prec*6;
    // init
    for (int i = 0; i < numVertices; i++)
    {
        vertices.push_back(glm::vec3(0.0f));
        normals.push_back(glm::vec3(0.0f));
        texCoords.push_back(glm::vec2(0.0f));
        sTangents.push_back(glm::vec3(0.0f));
        tTangents.push_back(glm::vec3(0.0f));
    }
    for (int i = 0; i < numIndices; i++)
    {
        indices.push_back(0);
    }
    // calculate the first ring
    for (int i = 0; i <= prec; i++)
    {
        float amt = toRadians(i*360.0f/prec);
        // rotate to form the ring
        glm::mat4 rMat = glm::rotate(glm::mat4(1.0f),amt,glm::vec3(0.0f,0.0f,1.0f));
        glm::vec3 initPos(rMat*glm::vec4(outer,0.0f,0.0f,1.0f));
        vertices[i] = glm::vec3(initPos+glm::vec3(inner,0.0f,0.0f));
        // texture coordinates
        texCoords[i] = glm::vec2(0.0f,(float)i/(float)prec);
        // tTangents
        tTangents[i] = glm::vec3(rMat*glm::vec4(0.0f,-1.0f,0.0f,1.0f));
        // sTangents
        sTangents[i] = glm::vec3(0.0f,0.0f,-1.0f);
        // normals
        normals[i] = glm::cross(sTangents[i],tTangents[i]);
    }
    // calculate the remaining rings
    for (int ring = 1; ring < prec+1; ring++)
    {
        for (int i = 0; i < prec+1; i++)
        {
            float amt = toRadians(ring*360.0f/prec);
            // rotate to form the ring
            glm::mat4 rMat = glm::rotate(glm::mat4(1.0f),amt,glm::vec3(0.0f,1.0f,0.0f));
            // vertices
            vertices[ring*(prec+1)+i] = glm::vec3(rMat*glm::vec4(vertices[i],1.0f));
            // texture coordinates
            texCoords[ring*(prec+1)+i] = glm::vec2((float)ring/prec,texCoords[i].t);
            // tTangents
            tTangents[ring*(prec+1)+i] = glm::vec3(rMat*glm::vec4(tTangents[i],1.0f));
            // sTangents
            sTangents[ring*(prec+1)+i] = glm::vec3(rMat*glm::vec4(sTangents[i],1.0f));
            // normals
            normals[ring*(prec+1)+i] = glm::cross(sTangents[ring*(prec+1)+i],tTangents[ring*(prec+1)+i]);
        }
    }
    // calculate the indices
    for (int ring = 0; ring < prec; ring++)
    {
        for (int vert = 0; vert < prec; vert++)
        {
            indices[6*(ring*prec+vert)] = ring*(prec+1)+vert;
            indices[6*(ring*prec+vert)+1] = (ring+1)*(prec+1)+vert;
            indices[6*(ring*prec+vert)+2] = ring*(prec+1)+vert+1;
            indices[6*(ring*prec+vert)+3] = ring*(prec+1)+vert+1;
            indices[6*(ring*prec+vert)+4] = (ring+1)*(prec+1)+vert;
            indices[6*(ring*prec+vert)+5] = (ring+1)*(prec+1)+vert+1;
        }
    }
}