#include "Sphere.h"

Sphere::Sphere(vec2 position, vec2 velocity, float mass, float radius, float elasticity, vec4 colour) 
	: Rigidbody(SPHERE, position, velocity, 0, mass, elasticity)
{
	m_radius = radius;
	m_colour = colour;
	m_moment = 0.5f * m_mass * m_radius * m_radius;
}

/// <summary>
/// Draw is used to draw the sphere to the screen
/// </summary>
void Sphere::Draw()
{
	vec2 end = vec2(std::cos(m_orientation), std::sin(m_orientation)) * m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 18, m_colour);

	if (m_colour.r <= 0.5f && m_colour.g <= 0.5f && m_colour.b <= 0.5f)
		aie::Gizmos::add2DLine(m_position, m_position + end, vec4(1, 1, 1, 1));
	else
		aie::Gizmos::add2DLine(m_position, m_position + end, vec4(0, 0, 0, 1));
}