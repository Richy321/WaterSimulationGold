#pragma once
#include "../../octet.h"

namespace octet
{
	struct PlaneSource : mesh_terrain::geometry_source 
	{
		mesh::vertex vertex(vec3_in bb_min, vec3_in uv_min, vec3_in uv_delta, vec3_in pos)
		{
			float y = 0.0;

			vec3 p = bb_min + pos + vec3(0, y, 0);
			vec3 normal = normalize(vec3(0, 1, 0));
			vec3 uv = uv_min + vec3((float)pos.x(), (float)pos.z(), 0) * uv_delta;

			return mesh::vertex(p, normal, uv);
		}
	};

}

