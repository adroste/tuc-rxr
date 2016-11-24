
layout( std140 ) uniform Framebuffer
{
	vec2 pstep;
	vec2 dummy; // if i dont add this alex's graphics card will optimize it away -> resulting in a deiferent uniform buffer size...
} frame;
