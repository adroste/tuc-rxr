#include "MeshLineBox.h"

static const Mesh::Vertex VERTS[] = {
	{{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f}},
	{{0.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f}},
	{{1.0f,0.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f}},
	{{1.0f,1.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f}},
	{{0.0f,0.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f}},
	{{1.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f}},
	{{0.0f,1.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f}},
	{{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f},{0.0f,0.0f,0.0f}}
};


static const unsigned int INDICES[] = {
	0, 1,
	0, 2,
	0, 3,
	4, 1,
	4, 2,
	4, 7,
	5, 2,
	5, 3,
	5, 7,
	6, 7,
	6, 3,
	6, 1
};

MeshLineBox::MeshLineBox()
	:
	Mesh(VERTS, sizeof(VERTS) / sizeof(Vertex), INDICES, sizeof(INDICES) / sizeof(unsigned int))
{
	
}
