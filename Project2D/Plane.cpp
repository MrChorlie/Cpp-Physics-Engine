#include "Plane.h"
#include "Gizmos.h"

#include <glm/glm.hpp>


/// <summary>
/// Default Contructor
/// </summary>
Plane::Plane() : PhysicsObject(ShapeType::PLANE)
{
	m_normal = vec2(0, 1);
	m_distanceToOrigin = 0;
	m_colour = vec4(1, 1, 1, 1);
}

Plane::Plane(vec2 normal, float distance, vec4 colour) : PhysicsObject(ShapeType::PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distance;
	m_colour = colour;
}

/// <summary>
/// FixedUpdate is called when the timestep reaches the maximun
/// </summary>
void Plane::FixedUpdate(vec2 gravity, float timeStep)
{

}

/// <summary>
/// Draw is used to draw the object to the screen
/// </summary>
void Plane::Draw()
{
	float lineSegmentLength = 2000;
	vec2 centerPoint = m_normal * m_distanceToOrigin;
	// easy to rotate normal through 90 degrees around z 
	vec2 parallel(m_normal.y, -m_normal.x);
	vec4 colourFade = m_colour;
	colourFade.a = 0;
	vec2 start = centerPoint + (parallel * lineSegmentLength);
	vec2 end = centerPoint - (parallel * lineSegmentLength);
	aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_colour, m_colour,
		colourFade);
	aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f,
		m_colour, colourFade, colourFade);
}

/// <summary>
/// ResolveCollision is used to resolve collisions between the plane and a rigidbody
/// </summary>
void Plane::ResolveCollision(Rigidbody* actor2, vec2 contact)
{
	// The position at which we'll apply the force relative to the object's COM 
	vec2 localContact = contact - actor2->GetPosition();

	// The plane isn't moving, so the relative velocity is just actor2's velocity at the contact point
	vec2 vRel = actor2->GetVelocity() + actor2->GetAngularVelocity() * vec2(-localContact.y, localContact.x);
	float velocityIntoPlane = dot(vRel, m_normal);

	// This is the perpendicular distance we apply the force at relative to the COM, so Torque = F * r
	float r = dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

	// Work out the "effective mass" - this is a combination of moment of 
	// inertia and mass, and tells us how much the contact point velocity  
	// will change with the force we're applying 
	float mass0 = 1.0f / (1.0f / actor2->GetMass() + (r * r) / actor2->GetMoment());

	float j = -(1 + actor2->GetElasticity()) * velocityIntoPlane * mass0;

	vec2 force = m_normal * j;

	actor2->ApplyForce(force, contact - actor2->GetPosition());

}