#pragma once
#include "Plane.h"
#include "wave.h"
class WaterPlane : public Plane
{
private:
	octet::dynarray<wave*> waves;
	octet::vec4 colour;
	
	octet::ref<octet::param_uniform> colourUniform;
	octet::ref<octet::param_uniform> timeUniform;
	octet::ref<octet::param_uniform> wavesUniform;

	const int waveCount = 1;

public:
	WaterPlane(const octet::ivec3 dimensions, const octet::vec3 size) : Plane(dimensions, size)
	{
		colour = octet::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		octet::param_shader* shader = new octet::param_shader("src/examples/water/waves.vs", "src/examples/water/waves.fs");
		material = new octet::material(colour, shader);
		setArrays();

		InitialiseWaves(waveCount);
		InitialiseUniforms();
	}

	~WaterPlane()
	{
	}


	void InitialiseWaves(const int count)
	{
		for (int i = 0; i < count; i++)
		{
			wave *newWave = new wave(10.0f, 5.0f, 0.15f, octet::vec2(1.0f, 0.8f).normalize(), count);
			waves.push_back(newWave);
		}
	}

	void InitialiseUniforms()
	{
		octet::atom_t atom_colour = octet::app_utils::get_atom("colour");
		colourUniform = material->add_uniform(nullptr, atom_colour, GL_FLOAT_VEC4, 1, octet::param::stage_fragment);
		material->set_uniform(colourUniform, &colour, sizeof(colour));

		octet::atom_t atom_time = octet::app_utils::get_atom("time");
		timeUniform = material->add_uniform(nullptr, atom_time, GL_FLOAT, 1, octet::param::stage_vertex);

		octet::atom_t atom_waves = octet::app_utils::get_atom("waves");
		wavesUniform = material->add_uniform(nullptr, atom_waves, GL_FLOAT, waveCount * (sizeof(wave) / sizeof(float)), octet::param::stage_vertex); //current 6 floats per wave
		material->set_uniform(wavesUniform, waves.data(), waves.size()*sizeof(waves[0]));
	}

	void Update(float time)
	{
		material->set_uniform(timeUniform, &time, sizeof(time));
	}
};

