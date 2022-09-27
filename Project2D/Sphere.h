#pragma once
#include "Rigidbody.h"

class Sphere : public Rigidbody
{
public:
	/// <summary>
	/// Default Constructor
	/// </summary>
	Sphere() {};
	Sphere(vec2 position, vec2 velocity, float mass, float radius, float elasticity, vec4 colour);
	~Sphere() {};

	/// <summary>
	/// Draw is used to draw the sphere to the screen
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// GetRadius returns the m_radius
	/// </summary>
	float GetRadius() { return m_radius; }
	/// <summary>
	/// GetColour returns the m_colour
	/// </summary>
	vec4 GetColour() { return m_colour; }

protected:
	float m_radius;
	vec4 m_colour;
};