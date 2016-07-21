#version 330 core

uniform Transforms
{
	mat4 matProjection;
	mat4 matCamera;
	mat4 matModel;
};

in vec4 textInfo;

out vec2 texpos;

void main(void) 
{
	gl_Position = matProjection * matCamera * vec4(textInfo.xy, 0, 1);
	texpos = textInfo.zw;
}