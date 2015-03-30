#pragma once
#include "../../octet.h"

class Plane : public octet::mesh
{

private:
	octet::dynarray<vertex> vertices;
	octet::dynarray<uint32_t> indices;
	octet::material *customMaterial;

	octet::ivec3 dimensions;
	octet::vec3 size;

public:

	Plane(const octet::ivec3 dimensions, const octet::vec3 size)
	{
		this->dimensions = dimensions;
		this->size = size;

		set_default_attributes();
		set_aabb(octet::aabb(octet::vec3(0, 0, 0), size));

		buildPlane();
	}

	virtual ~Plane()
	{
	}

	void buildPlane()
	{
		vertices.reserve((dimensions.x() + 1) * (dimensions.z() + 1));

		octet::vec3 dimf = (octet::vec3)(dimensions);
		octet::aabb bb = get_aabb();
		octet::vec3 bb_min = bb.get_min();
		octet::vec3 bb_delta = bb.get_half_extent() / dimf * 2.0f;
		bb_delta.y() = 0;
		octet::vec3 uv_min = octet::vec3(0);
		octet::vec3 uv_delta = octet::vec3(30.0f / dimf.x(), 30.0f / dimf.z(), 0);

		float tiling = 0.1f;

		float fTextureUStep = 1.0f / (dimensions.x() * tiling);
		float fTextureVStep = 1.0f / (dimensions.z() * tiling);

		for (int x = 0; x <= dimensions.x(); ++x)
		{
			for (int z = 0; z <= dimensions.z(); ++z)
			{
				octet::vec3 xz = octet::vec3((float)x, 0, (float)z) * bb_delta;
				vertex vertex;
				vertex.pos = octet::vec3p(xz);
				vertex.normal = octet::vec3p(0.0f, 1.0f, 0.0f);
				vertex.uv = octet::vec2p(x * fTextureUStep, z * fTextureVStep);
				vertices.push_back(vertex);
			}
		}

		indices.reserve(dimensions.x() * dimensions.z() * 6);

		int stride = dimensions.x() + 1;
		for (int x = 0; x < dimensions.x(); ++x)
		{
			for (int z = 0; z < dimensions.z(); ++z)
			{
				// 01 11
				// 00 10
				indices.push_back((x + 0) + (z + 0)*stride);
				indices.push_back((x + 0) + (z + 1)*stride);
				indices.push_back((x + 1) + (z + 0)*stride);
				indices.push_back((x + 1) + (z + 0)*stride);
				indices.push_back((x + 0) + (z + 1)*stride);
				indices.push_back((x + 1) + (z + 1)*stride);
			}
		}
	}

	void setArrays()
	{
		set_vertices(vertices);
		set_indices(indices);
	}


};

