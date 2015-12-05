#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <string>
using std::vector;
using std::string;
struct ObjData
{
    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;
	vector<unsigned int> indices;
};
void readObjData(const string & path,ObjData * data);

#endif // OBJLOADER_H
