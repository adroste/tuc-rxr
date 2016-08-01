// gamma correction

vec3 correctGamma(vec3 v)
{
	// return sqrt for faster processing
	return pow(v,vec3(1.0 / 2.2));
}

vec3 revertGamma(vec3 v)
{
	return pow(v, vec3(2.2));
}