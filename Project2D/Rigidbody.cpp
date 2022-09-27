#include "Rigidbody.h"

#include <iostream>

#define MIN_LINEAR_THRESHOLD 0.1f
#define MIN_ANGULAR_THRESHOLD 0.1f

Rigidbody::Rigidbody(ShapeType shapeID, vec2 position, vec2 velocity,
	float orientation, float mass, float elasticity)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_elasticity = elasticity;
	m_orientation = orientation;
	m_mass = mass;
	m_angularVelocity = 0;
}

/// <summary>
/// FixedUpdate is called when the timestep reaches the maximun
/// </summary>
void Rigidbody::FixedUpdate(vec2 gravity, float timeStep)
{
	m_position += m_velocity * timeStep;
	ApplyForce(gravity * m_mass * timeStep);

	m_orientation += m_angularVelocity * timeStep;

	m_velocity -= m_velocity * m_linearDrag * timeStep;
	m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep;

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD) {
		m_velocity = vec2(0, 0);
	}
	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD) {
		m_angularVelocity = 0;
	}
}

/// <summary>
/// ApplyForce is used to apply a certain force to the rigidbody given a vector 2
/// </summary>
void Rigidbody::ApplyForce(vec2 force)
{
	m_velocity += force / m_mass;
}

/// <summary>
/// ApplyForce is used to apply a certain force to the rigidbody given a vector 2 and a contact position
/// </summary>
void Rigidbody::ApplyForce(vec2 force, vec2 pos)
{
	m_velocity += force / GetMass();
	m_angularVelocity += (force.y * pos.x + force.x * pos.y) / GetMoment();
}

/// <summary>
/// ResolveCollision is used to resolve the collision between 2 diffrent rigidbodies
/// </summary>
void Rigidbody::ResolveCollision(Rigidbody* actor2, vec2 contact, vec2* collisionNormal)
{
	// Find the vector between their centres, or use the provided direction 
	// of force, and make sure it's normalised 
	vec2 normal = normalize(collisionNormal ? *collisionNormal : actor2->m_position - m_position);
	// Get the vector perpendicular to the collision normal 
	vec2  perp(normal.y, -normal.x);

	// Determine the total velocity of the contact points for the two objects,  
	// for both linear and rotational

	// 'r' is the radius from axis to application of force 
	float r1 = dot(contact - m_position, -perp);
	float r2 = dot(contact - actor2->m_position, perp);
	// Velocity of the contact point on this object  
	float v1 = dot(m_velocity, normal) - r1 * m_angularVelocity;
	// Velocity of contact point on actor2 
	float v2 = dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	if (v1 > v2) // they're moving closer 
	{
		// Calculate the effective mass at contact point for each object 
		// ie how much the contact point will move due to the force applied. 
		float mass1 = 1.0f / (1.0f / m_mass + (r1 * r1) / m_moment);
		float mass2 = 1.0f / (1.0f / actor2->m_mass + (r2 * r2) / actor2->m_moment);

		float elasticity = (GetElasticity() + actor2->GetElasticity()) / 2.0f;

		vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;

		// Apply equal and opposite forces 
		ApplyForce(-force, contact - m_position);
		actor2->ApplyForce(force, contact - actor2->m_position);
	}
}