// material desciption

layout( std140 ) uniform Material
{
	vec3 MaterialDiffuse;
	vec4 MaterialSpecular; // rgb -> color | w -> glossy
};