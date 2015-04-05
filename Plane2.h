#pragma once
#include "../../octet.h"

class Plane2 : public octet::mesh
{
public:

	Plane2(const unsigned int sizeX, const unsigned int sizeY)
	{
		CreatePlane();
	}

	virtual ~Plane2()
	{
	}

	static uint32_t make_color(octet::vec3 colour) {
		float r = colour.x();
		float g = colour.y();
		float b = colour.z();
		return 0xff000000 + ((int)(r*255.0f) << 0) + ((int)(g*255.0f) << 8) + ((int)(b*255.0f) << 16);
	}

protected:
	octet::dynarray<vertex> vertices;
	octet::dynarray<uint32_t> indices;

	struct PlaneVertex 
	{
		octet::vec3p pos;
		octet::vec3p normal;
		uint32_t color;
	};

	void CreatePlane(const unsigned int sizeX, const unsigned int sizeY)
	{

		//Lock index and vertex buffers
		octet::gl_resource::wolock vl(get_vertices());
		PlaneVertex *vtx = (PlaneVertex *)vl.u8();
		octet::gl_resource::wolock il(get_indices());
		uint32_t *idx = il.u32();

		for (unsigned int i = 0; i != sizeX; i++)
		{
			for (unsigned int j = 0; j != sizeY; j++)
			{
				vtx->pos = octet::vec3p(j, -i, 0.0f);
				vtx->normal = octet::vec3p(0.0f, 1.0f, 0.0f);
				vtx->color = make_color(octet::vec3(0.0f, 0.1f, 1.0f));
				vtx++;
			}
		}
	}
};

