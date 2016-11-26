// bump mapping helper functions

// VertexShader: calculates matrix for bumpReadNormal
mat3 bumpGetRotation(vec3 inNormal, vec3 inTangent)
{
	// reorder tangent so that its perfectly othogonal (improves visual quality)
	vec3 tangent = normalize(inTangent - dot(inTangent, inNormal) * inNormal);

	vec3 biTangent = cross(tangent, inNormal); // 3rd axis for coordinate system
	
	return mat3(tangent, biTangent, inNormal);
}

// VertexShader: calculates matrix for bumpReadNormal
mat3 bumpGetRotation(vec3 inNormal, vec3 inTangent, vec3 inBitangent)
{
	return mat3(inNormal,inTangent,inBitangent);
}

// FragmentShader: transfers normal from sampler to correct normal
vec3 bumpReadNormal(vec3 rawNormal, mat3 matrix)
{
	return normalize(matrix * ((255.0 / 128.0 * rawNormal) - 1.0));
}