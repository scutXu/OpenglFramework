#version 430
uniform mat4 p;
in vec3 pixelPos;
void main()
{
	vec2 coord = (2 * gl_PointCoord) - vec2(1,1);
	float X2AddY2 = dot(coord,coord);
	if(X2AddY2 > 1) {
		discard;
	}
	float zCoord = -sqrt(1 - X2AddY2);
	vec3 realPixelPos = -(1.2 * vec3(coord,zCoord)) + pixelPos;
	vec4 clipSpacePos = p * vec4(realPixelPos,1);
	gl_FragDepth = clipSpacePos.z / clipSpacePos.w;
	gl_FragColor = vec4(realPixelPos,1);
}