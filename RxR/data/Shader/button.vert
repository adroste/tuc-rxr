#version 120

varying vec2 curPos;

void main() 
{
	curPos =  gl_MultiTexCoord0.xy;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0);
}