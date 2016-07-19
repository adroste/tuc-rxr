#version 120

varying vec2 curPos;

vec3 rotateY(vec3 v, float angle)
{
	vec3 o;
	float cosa = cos(angle);
	float sina = sin(angle);
	
	o.x = cosa * v.x + sina * v.z;
	o.y = v.y;
	o.z = -sina * v.x + cosa * v.z;
	return o;
}

vec3 rotateX(vec3 v, float angle)
{
	vec3 o;
	float cosa = cos(angle);
	float sina = sin(angle);
	
	o.x = v.x;
	o.y = cosa * v.y - sina * v.z;
	o.z = sina * v.y + cosa * v.z;
	return o;
}

float func(float value)
{
	// f[0,1] -> R := -0.8x²+0.8x <- surface of the button
	// f'(x) = -1.6x + 0.8x
	return -1.6 * value + 0.8;
}

void main()
{
	vec3 buttColor = vec3(0.145,0.317,0.352) * 1.5;
	
	// calculate surface normal
	vec3 normal = vec3(0.0,0.0,1.0);
	normal = rotateY(normal, func(curPos.x));
	normal = rotateX(normal, func(curPos.y));

	// incomming light
	vec3 inLight = normalize( vec3(0.0,0.85,-1.0) );
	
	// lampert refelction
	float theta = dot( reflect(inLight,normal), normal);
	
	
	// specular term (angle between eye and reflected ray)
	float phi = dot( reflect(inLight,normal), vec3(0.0,0.0,1.0) );
	phi = max(phi, 0.0);
	
	phi = pow(phi, 5.0);
	
	//				ambient light   || diffuse light    || specular light
	vec3 outColor = buttColor * 0.2 + buttColor * theta + (buttColor * 0.3) * phi;
	
	gl_FragColor = vec4( clamp(outColor,0.0,1.0) , 1.0);
}