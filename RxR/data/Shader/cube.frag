#version 330 core

uniform vec4 color;
in vec4 normal;

void main()
{
	vec3 lightvec = normalize( vec3(0.0,-1.0,-0.7) );
	vec3 n = normalize(normal.xyz);
	float theta = dot( reflect( lightvec, n) , n);

	theta = max(theta,0.1);
	gl_FragColor = color * theta;
	//gl_FragColor = vec4(abs(normal.xyz),1.0);
}