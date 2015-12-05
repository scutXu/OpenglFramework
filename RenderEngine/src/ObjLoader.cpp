#include "../RenderEngine/include/ObjLoader.h"
#include <fstream>
#include <sstream>
using namespace std;
string trim(const string & str,const string & whiteSpace = " ")
{
    const string::size_type strBegin = str.find_first_not_of(whiteSpace);
    if(strBegin == string::npos)
        return "";
    const string::size_type strEnd = str.find_last_not_of(whiteSpace);
    const string::size_type strRange = strEnd - strBegin + 1;
    return str.substr(strBegin,strRange);
}
void readObjData(const string & path,ObjData * data)
{
	vector<float> tempVertices;
	vector<float> tempNormals;
	vector<float> tempUVs;
	data->indices.clear();
	data->vertices.clear();
	data->normals.clear();
	data->texCoords.clear();
    ifstream file(path);
    if(file.fail()) {
        return;
    }
    string line;
    string prefix;
    stringstream lineStream;
    while(!getline(file,line).eof()) {
        line = trim(line," ");
        if(line.length() == 0)
            continue;
        if(line.find_first_of("#") != string::npos)
            continue;
        string::size_type spaceIndex = line.find_first_of(" ");
        prefix = line.substr(0,spaceIndex);
        line = line.substr(spaceIndex + 1);
        lineStream.clear();
        lineStream.str(line);
        if(prefix.compare("v")==0) {
            float value;
            for(int i=0;i<3;++i) {
                lineStream>>value;
				tempVertices.push_back(value);
            }
            continue;
        }
		if (prefix.compare("vt") == 0) {
			float value;
			for (int i = 0; i < 2; ++i) {
				lineStream >> value;
				tempUVs.push_back(value);
			}
			continue;
		}
        if(prefix.compare("f")==0) {
            unsigned v , vt , vn = 0;
			int vertexIndex = 0;
            while(!lineStream.eof()) {
                lineStream>>v;
				vertexIndex = 3 * (v - 1);
				data->vertices.push_back(tempVertices[vertexIndex]);
				data->vertices.push_back(tempVertices[vertexIndex + 1]);
				data->vertices.push_back(tempVertices[vertexIndex + 2]);
                char next = lineStream.peek();
                if(next == '/') {
					lineStream.ignore(1);
                    next = lineStream.peek();
                    if(next == '/') {
                        /*		v//vn		*/
                        lineStream.ignore(1);
                        lineStream>>vn;
						vertexIndex = 3 * (vn - 1);
						data->normals.push_back(tempNormals[vertexIndex]);
						data->normals.push_back(tempNormals[vertexIndex + 1]);
						data->normals.push_back(tempNormals[vertexIndex + 2]);
                    }
                    else {
                        lineStream>>vt;
						vertexIndex = 2 * (vt - 1);
						data->texCoords.push_back(tempUVs[vertexIndex]);
						data->texCoords.push_back(tempUVs[vertexIndex + 1]);
                        next = lineStream.peek();
                        if(next == '/') {
                            /*		v/vt/vn		*/
                            lineStream.ignore(1);
                            lineStream>>vn;
							vertexIndex = 3 * (vn - 1);
							data->normals.push_back(tempNormals[vertexIndex]);
							data->normals.push_back(tempNormals[vertexIndex + 1]);
							data->normals.push_back(tempNormals[vertexIndex + 2]);
                        }
                        /*
                        else {
                            v/vt
                        }
                        */
                    }
                }
                /*
                else {
                    v
                }
                */
			}
		}
	}
}
