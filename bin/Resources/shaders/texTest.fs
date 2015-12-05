#version 430
uniform sampler2D normalMap;
in vec2 ouv;
void main()
{
	vec3 normal = (texture2D(normalMap,ouv)).xyz;
	if(length(normal) < 0.1) {
		discard;
	}
	if(normal.z < 0) {
		normal = -normal;
	}
	vec3 lightDir = normalize(vec3(0,0,1));
	float diffuse = max(0,dot(lightDir,normal));
	gl_FragColor = vec4(0,0,1,1) * diffuse;
}