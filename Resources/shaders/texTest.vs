#version 430
uniform mat4 mvp;
in vec3 pos;
in vec2 uv;
out vec2 ouv;
void main()
{
	gl_Position = vec4(pos,1);
	ouv = uv;
}