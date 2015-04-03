#pragma once
#include "Plane.h"
#include "wave.h"
#include <chrono>

using namespace std::chrono;

class WaterPlane : public Plane
{
private:

	octet::dynarray<wave*> waves;
	octet::vec4 colour;
	static const int waveCount =8;

	octet::ref<octet::param_uniform> colourUniform;
	octet::ref<octet::param_uniform> timeUniform;
	octet::ref<octet::param_uniform> waveCountUniform;
	//octet::ref<octet::param_uniform> wavesUniform;
	
	octet::ref<octet::param_uniform> amplitudeUniform;
	octet::ref<octet::param_uniform> wavelengthUniform;
	octet::ref<octet::param_uniform> speedUniform;
	octet::ref<octet::param_uniform> steepnessUniform;
	octet::ref<octet::param_uniform> directionXUniform;
	octet::ref<octet::param_uniform> directionYUniform;
	
	const float PI = 3.14159f;
	octet::random rand;

	time_point<high_resolution_clock> startTime;
	float secondsSinceStart = 0.0f;


public:
	WaterPlane(const octet::ivec3 dimensions, const octet::vec3 size) : Plane(dimensions, size)
	{
		colour = octet::vec4(0.0f, 0.0f, 1.0f, 1.0f);

		rand = octet::random(time(NULL));

		octet::param_shader* shader = new octet::param_shader("src/examples/water/waves.vp", "src/examples/water/waves.fp");
		material = new octet::material(colour, shader);
		setArrays();

		InitialiseWaves(waveCount);
		InitialiseUniforms();

		startTime = std::chrono::high_resolution_clock::now();
	}

	~WaterPlane()
	{
	}

	void InitialiseWaves(const int count)
	{
		for (int i = 0; i < count; i++)
		{
			float angle = rand.get(-PI / 3, PI / 3);

			wave *newWave = new wave(
				8.0f * PI / (i+1), 
				0.5f / (i+1), 
				1.0f + 2.0f * i, 
				1.0f,
				octet::vec2(octet::math::cos(angle), octet::math::sin(angle)).normalize(), 
				count);
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

		octet::atom_t atom_waveCount = octet::app_utils::get_atom("waveCount");
		waveCountUniform = material->add_uniform(nullptr, atom_waveCount, GL_INT, 1, octet::param::stage_vertex);
		material->set_uniform(waveCountUniform, &waveCount, sizeof(waveCount));

		octet::atom_t atom_amplitude = octet::app_utils::get_atom("amplitude");
		amplitudeUniform = material->add_uniform(nullptr, atom_amplitude, GL_FLOAT, 8, octet::param::stage_vertex);

		octet::atom_t atom_wavelength = octet::app_utils::get_atom("wavelength");
		wavelengthUniform = material->add_uniform(nullptr, atom_wavelength, GL_FLOAT, 8, octet::param::stage_vertex);

		octet::atom_t atom_speed = octet::app_utils::get_atom("speed");
		speedUniform = material->add_uniform(nullptr, atom_speed, GL_FLOAT, 8, octet::param::stage_vertex);

		octet::atom_t atom_steepness = octet::app_utils::get_atom("steepness");
		steepnessUniform = material->add_uniform(nullptr, atom_steepness, GL_FLOAT, 8, octet::param::stage_vertex);

		octet::atom_t atom_directionX = octet::app_utils::get_atom("directionX");
		directionXUniform = material->add_uniform(nullptr, atom_directionX, GL_FLOAT, 8, octet::param::stage_vertex);

		octet::atom_t atom_directionY = octet::app_utils::get_atom("directionY");
		directionYUniform = material->add_uniform(nullptr, atom_directionY, GL_FLOAT, 8, octet::param::stage_vertex);

		//material->set_uniform(amplitudeUniform, &waveCount, sizeof(waveCount));
		Update();

		//octet::atom_t atom_waves = octet::app_utils::get_atom("waves");
		//wavesUniform = material->add_uniform(nullptr, atom_waves, GL_FLOAT, waveCount * (sizeof(wave) / sizeof(float)), octet::param::stage_vertex); //current 6 floats per wave
		//material->set_uniform(wavesUniform, waves.data(), waves.size()*sizeof(waves[0]));
	}

	void Update()
	{
		//Get current time from start using c++11 high res timer
		time_point<high_resolution_clock> nowTime = high_resolution_clock::now();
		duration<float> durationSinceStart = duration_cast<duration<float>>(nowTime - startTime);
		secondsSinceStart = durationSinceStart.count();

		material->set_uniform(timeUniform, &secondsSinceStart, sizeof(secondsSinceStart));

		float amplitude[waveCount];
		float wavelength[waveCount];
		float speed[waveCount];
		float steepness[waveCount];
		float directionX[waveCount];
		float directionY[waveCount];

		for (unsigned int i = 0; i < waves.size(); i++)
		{
			amplitude[i] = waves[i]->amplitude;
			wavelength[i] = waves[i]->wavelength;
			speed[i] = waves[i]->speed;
			steepness[i] = waves[i]->steepness;
			directionX[i] = waves[i]->direction.x();
			directionY[i] = waves[i]->direction.y();
		}

		material->set_uniform(amplitudeUniform, &amplitude, waveCount * sizeof(float));
		material->set_uniform(wavelengthUniform, &wavelength, waveCount * sizeof(float));
		material->set_uniform(speedUniform, &speed, waveCount * sizeof(float));
		material->set_uniform(steepnessUniform, &steepness, waveCount * sizeof(float));
		material->set_uniform(directionXUniform, &directionX, waveCount * sizeof(float));
		material->set_uniform(directionYUniform, &directionY, waveCount * sizeof(float));
	}
};
