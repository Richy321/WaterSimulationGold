#pragma once
#include "Plane.h"
#include "wave.h"
#include <chrono>

using namespace std::chrono;

class WaterPlane : public Plane
{
private:
	octet::ref<octet::param_uniform> colourUniform;
	octet::ref<octet::param_uniform> alphaUniform;
	octet::ref<octet::param_uniform> timeUniform;
	octet::ref<octet::param_uniform> waveCountUniform;
	octet::ref<octet::param_uniform> wavesUniform;
	
	octet::ref<octet::param_uniform> amplitudeUniform;
	octet::ref<octet::param_uniform> wavelengthUniform;
	octet::ref<octet::param_uniform> speedUniform;
	octet::ref<octet::param_uniform> steepnessUniform;
	octet::ref<octet::param_uniform> directionXUniform;
	octet::ref<octet::param_uniform> directionYUniform;

	octet::ref<octet::param_uniform> waveTypeUniform;
	
	const float PI = 3.14159f;
	octet::random rand;

	time_point<high_resolution_clock> startTime;
	float secondsSinceStart = 0.0f;
public:
	enum WavesType
	{
		Sine = 0,
		Gerstner = 1,
		Radial = 2,
	} waveType = Gerstner;

	octet::dynarray<wave*> waves;
	static const int waveCount = 8;
	octet::vec3 colour;
	float alpha = 0.6f;

public:
	WaterPlane(const octet::ivec3 dimensions, const octet::vec3 size) : Plane(dimensions, size)
	{
		colour = octet::vec3(0.0f, 0.0f, 1.0f);

		rand = octet::random((unsigned int)time(NULL));

		octet::param_shader* shader = new octet::param_shader("src/examples/water/waves.vp", "src/examples/water/waves.fp");
		material = new octet::material(octet::vec4(colour, alpha) , shader);
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
		waves.reset();
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
		colourUniform = material->add_uniform(nullptr, atom_colour, GL_FLOAT_VEC3, 1, octet::param::stage_fragment);

		octet::atom_t atom_alpha = octet::app_utils::get_atom("alpha");
		alphaUniform = material->add_uniform(nullptr, atom_alpha, GL_FLOAT, 1, octet::param::stage_fragment);

		octet::atom_t atom_time = octet::app_utils::get_atom("time");
		timeUniform = material->add_uniform(nullptr, atom_time, GL_FLOAT, 1, octet::param::stage_vertex);

		octet::atom_t atom_waveType = octet::app_utils::get_atom("waveType");
		waveTypeUniform = material->add_uniform(nullptr, atom_waveType, GL_INT, 1, octet::param::stage_vertex);

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

		Update();

		octet::atom_t atom_waves = octet::app_utils::get_atom("waves");
		wavesUniform = material->add_uniform(nullptr, atom_waves, GL_FLOAT, waveCount * (sizeof(wave) / sizeof(float)), octet::param::stage_vertex); //current 6 floats per wave
		
		size_t waveSize = sizeof(waves[0]);
		__unaligned wave** waveData = waves.data();
		size_t wavesSize = sizeof(waveData);
		material->set_uniform(wavesUniform, waveData, waves.size()*sizeof(wave));
	}

	void Update()
	{
		//Get current time from start using c++11 high res timer
		time_point<high_resolution_clock> nowTime = high_resolution_clock::now();
		duration<float> durationSinceStart = duration_cast<duration<float>>(nowTime - startTime);
		secondsSinceStart = durationSinceStart.count();

		//update time
		material->set_uniform(timeUniform, &secondsSinceStart, sizeof(secondsSinceStart));

		//update wave type
		material->set_uniform(waveTypeUniform, &waveType, sizeof(int));

		//update colour & alpha
		material->set_uniform(colourUniform, &colour, sizeof(colour));
		material->set_uniform(alphaUniform, &alpha, sizeof(alpha));

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
			directionX[i] = waves[i]->directionX;
			directionY[i] = waves[i]->directionY;
		}

		//update all wave parameters
		material->set_uniform(amplitudeUniform, &amplitude, waveCount * sizeof(float));
		material->set_uniform(wavelengthUniform, &wavelength, waveCount * sizeof(float));
		material->set_uniform(speedUniform, &speed, waveCount * sizeof(float));
		material->set_uniform(steepnessUniform, &steepness, waveCount * sizeof(float));
		material->set_uniform(directionXUniform, &directionX, waveCount * sizeof(float));
		material->set_uniform(directionYUniform, &directionY, waveCount * sizeof(float));
	}
};
