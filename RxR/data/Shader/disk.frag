#version 120

varying vec2 curPos;
uniform vec4 color;

void main()
{
	float alpha = 0.0;
	if (length(curPos) <= 1.0)
	{
		alpha = 1.0;
	}
	gl_FragColor = vec4(color.rgb, color.a * alpha);
}