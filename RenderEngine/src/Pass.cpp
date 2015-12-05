#include"../RenderEngine/include/Pass.h"
Pass::Pass() :
	polygonMode(GL_FILL),
	renderFace(GL_FRONT_AND_BACK),
	enablePolygonOffsetFill(false),
	enablePolygonOffsetLine(false),
	lineWidth(1.0f)
{
	
}
Pass::~Pass()
{

}
void Pass::bindTextures()
{
	unsigned int textureUnit = GL_TEXTURE0;
	unsigned int uniform = 0;
	for (auto iter = textureMap.begin(); iter != textureMap.end(); ++iter) {
		iter->second->bind(textureUnit);
		shader->setUniformValue(iter->first.c_str(), uniform);
		++textureUnit;
		++uniform;
	}
}
void Pass::setShaderProgram(ShaderProgramPtr pShader)
{
	shader = pShader;
}
void Pass::addTexture(TexturePtr texture,const string & name)
{
	textureMap.insert(TextureMap::value_type(name, texture));
}
ShaderProgramPtr Pass::getShaderProgram()
{
	return shader;
}
void Pass::bindUserUniforms()
{
	for (auto iter = vec3Map.begin(); iter != vec3Map.end(); ++iter) {
		shader->setUniformValue(iter->first.c_str(), iter->second);
	}
}
void Pass::addUnifrom(const string & uniformName, QVector3D value)
{
	vec3Map.insert(Vec3UniformMap::value_type(uniformName, value));
}