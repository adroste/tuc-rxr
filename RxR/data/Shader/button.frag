#version 330 core

#include "uniforms/Framework.glsl"
#include "light/gamma.glsl"

uniform sampler2D tex;
uniform sampler2D heightMap;

uniform vec3 lightPos;

in vec2 texCoord;
in vec3 pos;

const vec2 size = vec2(2.0,0.0);
const ivec3 off = ivec3(-1,0,1);

vec3 calcDirectionalLight(vec3 normal, vec3 direction, vec3 mdiff, vec3 lightColor, vec3 mspec, vec3 eye)
{
	vec3 reflected = reflect(direction, normal);
	float theta = dot(normal, reflected);
	
	float phi = dot(reflected, eye ); // eye
	phi = pow(max(phi,0.0),250.0);
	
	return mdiff * lightColor * theta + mspec * phi;
}

void main()
{
	if(!framework_isInRect(pos.xy)) discard;

	vec4 raw = texture(heightMap, texCoord);
	
	vec3 normal = normalize( ((255.0 / 128.0 * raw.xyz) - 1.0) );
	
	vec4 sampleColor = texture(tex, texCoord);
	vec3 diffuse = revertGamma( sampleColor.rgb );
	vec3 specular = vec3(1.0);
	
	vec3 col = vec3(0.0);
	
	col += calcDirectionalLight(normal, normalize(vec3(0.2,0.5,-0.7)), diffuse, vec3(1.2,0.8,0.8),
		vec3(0.4), normalize(vec3(0.0,1.0,1.0)));
	
	col += calcDirectionalLight(normal, normalize(pos - vec3(framework.mouse.xy,100.0)),diffuse,
		vec3(1.0)/*vec3(1.2,0.8,0.8)*/,vec3(0.001), vec3(0.0,0.0,1.0));
	
	if(sampleColor.a == 0)
	{
		col += lightPos;
	}
	
	gl_FragColor = vec4(correctGamma(col), sampleColor.a);
}