#pragma once

struct Colour {
	float r;
	float g;
	float b;
	float a;

	Colour(float x_t, float y_t, float z_t) {
		r = x_t;
		g = y_t;
		b = z_t;
		a = 1.f;
	}
	Colour(float Red, float Green, float Blue, float Alpha) {
		r = Red;
		g = Green;
		b = Blue;
		a = Alpha;
	}
};