#version 430

uniform mat4 mvp;
uniform mat4 mv;
in vec3 pos;
out vec3 pixelPos;
void main()
{
	gl_Position = mvp * vec4(pos,1);
	gl_PointSize = 800 / gl_Position.w;
	pixelPos = (mv * vec4(pos,1)).xyz;
}
