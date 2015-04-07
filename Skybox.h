#pragma once
#include "../../octet.h"

class Skybox
{
public:
	octet::mesh *box;
	octet::image *texture;
	octet::material *customMaterial;

	Skybox(char* texturePath)
	{
		octet::param_shader* shader = new octet::param_shader("src/examples/water/cubemap.vp", "src/examples/water/cubemap.fp");
		customMaterial = new octet::material(octet::vec4(0, 1, 0, 1), shader);

		texture = new octet::image(texturePath);
		customMaterial->add_sampler(0, octet::app_utils::get_atom("texture"), texture, new octet::sampler());

		box = new octet::mesh_box(octet::vec3(1000.0f, 1000.0f, 1000.0f));
	}

	virtual ~Skybox()
	{
	}


	octet::material* GetMaterial() { return customMaterial; }

};

