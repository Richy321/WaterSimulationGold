#pragma once
#include "../../octet.h"
#include "TerrainSources.h"

class Plane2
{
public:

	bool renderWireframe = false;
	octet::material *material = nullptr;
	octet::PlaneSource *source;

	octet::ref<octet::mesh_terrain> plane;

	Plane2(const octet::ivec3 dimensions, const octet::vec3 size)
	{
		source = new octet::PlaneSource();
		plane = new octet::mesh_terrain(size, dimensions, *source);
		plane->set_mode(GL_TRIANGLES);
	}

	virtual ~Plane2()
	{
	}
public:
	void toggleWireframe()
	{
		renderWireframe = !renderWireframe;

		if (renderWireframe)
			plane->set_mode(GL_LINES);
		else
			plane->set_mode(GL_TRIANGLES);
	}

	octet::material* getMaterial() { return material; }
	void setMaterial(octet::material* mat) { material = mat; }

	octet::mesh* getMesh() { return plane; }
};

