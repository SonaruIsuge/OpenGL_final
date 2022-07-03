
#include "ObjLoader.h"
#include "Utils.h"
#include <iostream>
#include <fstream>


ObjLoader* ObjLoader::_instance = nullptr;


ObjLoader::ObjLoader() 
{

}

ObjLoader::~ObjLoader() 
{

}


ObjLoader* ObjLoader::Create() 
{
    if (_instance == nullptr)  _instance = new (std::nothrow) ObjLoader;

    return _instance;
}


ObjLoader* ObjLoader::Instance() 
{
    if (_instance == nullptr)  _instance = new (std::nothrow) ObjLoader;

    return _instance;
}


void ObjLoader::Destroy() 
{
    SAFE_DELETE(_instance);
}


ShapeData ObjLoader::GetObjData(char path[])
{
    ShapeData result;

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        printf("Impossible to open the file !n");
        return  result;
    }

    while (1) {

        char lineHeader[200];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;

        if (strcmp(lineHeader, "v") == 0) {
            vec4 vertex;
            fscanf(file, "%f %f %fn", &vertex.x, &vertex.y, &vertex.z);
            vertex.w = 1.0f;
            tempPoints.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vt") == 0) {
            vec2 uv;
            fscanf(file, "%f %fn", &uv.x, &uv.y);
            tempTexs.push_back(uv);
            //printf("%f %f\n", uv.x, uv.y);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            vec3 normal;
            fscanf(file, "%f %f %fn", &normal.x, &normal.y, &normal.z);
            tempNormals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0) {
            char str[200];
            fscanf(file, " %[^\n] ", str);

            vector<string> pointsData = split(str, ' ');
            if (pointsData.size() != 3) continue;
            for (auto& pointData : pointsData) {
                //A point: vertex/uv/normal
                vector<string> data = split(pointData, '/');
                points.push_back( tempPoints[stoi(data[0])-1] );
                //printf("(%f, %f)\n", tempTexs[stoi(data[1]) - 1].x, tempTexs[stoi(data[1]) - 1].y);
                texs.push_back( tempTexs[stoi(data[1])-1] );
                normals.push_back( tempNormals[stoi(data[2])-1] );
            }
        }
        else ;
    }
    fclose(file);

    result.vtxNum = points.size();
    result.points = points;
    result.texs = texs;
    result.normals = normals;
    return result;
}


void ObjLoader::ClearData() 
{
    tempPoints.clear();
    tempNormals.clear();
    tempTexs.clear();
    points.clear();
    normals.clear();
    texs.clear();
}