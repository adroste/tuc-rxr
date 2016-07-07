#version 120

varying vec2 curPos;

void main() 
{
	curPos =  gl_Vertex.xy;
	vec2 new;
	new.x = gl_Vertex.x * 1280.0;
	new.y = gl_Vertex.y * 20.0;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(new,10.0,1.0);
}