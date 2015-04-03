#pragma once
#include "../../octet.h"

class wave
{
public:
	float wavelength; //L - crest to crest distance between waves in world space. (L relates to frequency w as w = 2pi/L
	float amplitude; //A - height from the water plane to the wave crest
	float speed; //S - distance the crest moves forward per second. (phase constant - S x 2pi/L)
	float steepness; // K - steepness value for 
	octet::vec2 direction; //D - horizontal vector perpendicular to the wave front along which the crest travels

	wave(float wavelength, float amplitude, float speed, float steepness, octet::vec2 direction, const int waveCount)
	{
		this->wavelength = wavelength;
		this->amplitude = amplitude;
		this->speed = speed;
		this->steepness = steepness; //CalculateSteepness(Q ? ? , waveCount);
		this->direction = direction;
	}

	~wave()
	{
	}

	float CalculateSteepness(float Q, int waveCount)
	{
		//  Q / W * A * waveCount
		return 0.0f;
	}

	/*float CalculateSpeed(float s)
	{
		//Phase-constant S x 2pi/L
		return s * ((2 * PI) / wavelength);
	}*/
};

