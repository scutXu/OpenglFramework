#ifndef Pass_H
#define Pass_H
#include<memory>
#include<map>
#include<string>
#include<QOpenGLShaderProgram>
#include<QOpenGLTexture>
using std::shared_ptr;
using std::map;
using std::string;
typedef shared_ptr<QOpenGLTexture> TexturePtr;
typedef shared_ptr<QOpenGLShaderProgram> ShaderProgramPtr;
typedef map<string, TexturePtr> TextureMap;
typedef map<string, QVector3D> Vec3UniformMap;
class Pass {
public:
	GLenum polygonMode;
	GLenum renderFace;
	bool enablePolygonOffsetFill;
	bool enablePolygonOffsetLine;
	QVector2D polygonOffsetParams;
	float lineWidth;
public:
	Pass::Pass();
	Pass::~Pass();
	void setShaderProgram(ShaderProgramPtr pShader);
	void addTexture(TexturePtr texture,const string & name);
	ShaderProgramPtr getShaderProgram();
	void bindTextures();

	void addUnifrom(const string & uniformName, QVector3D value);
	void bindUserUniforms();
protected:
	TextureMap textureMap;
	ShaderProgramPtr shader;

	Vec3UniformMap vec3Map;
};

typedef shared_ptr<Pass> PassPtr;

#endif // Pass_H
