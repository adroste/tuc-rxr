#version 330 core

varying vec4 normal;

void main()
{
	vec3 lightvec = normalize( vec3(-0.1,-1.0,-0.3) );
	vec3 n = normalize(normal.xyz);
	float theta = dot( reflect( lightvec, n) , n);

	theta = max(theta,0.0);
	gl_FragColor = vec4(1.0,0.0,0.0,1.0) * theta;
	//gl_FragColor = vec4(abs(normal.xyz),1.0);
}