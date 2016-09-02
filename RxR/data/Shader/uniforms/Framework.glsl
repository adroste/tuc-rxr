// framework desciption

layout( std140 ) uniform Framework
{
	vec4 clip; // x1,y1, x2,y2
	vec4 mouse;
} framework;

bool framework_isInRect(vec2 pos)
{
	return pos.x >= framework.clip.x && pos.x <= framework.clip.z && pos.y >= framework.clip.y && pos.y <= framework.clip.w;
}