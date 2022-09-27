#pragma once
#include "PhysicsObject.h"
#include "Gizmos.h"

class Rigidbody : public PhysicsObject
{
public:
	/// <summary>
	/// Default Constructor
	/// </summary>
	Rigidbody() {};
	Rigidbody(ShapeType shapeID, vec2 position, vec2 velocity, 
		float orientation, float mass, float elasticity);
	~Rigidbody() {};

	/// <summary>
	/// FixedUpdate is called when the timestep reaches the maximun
	/// </summary>
	virtual void FixedUpdate(vec2 gravity, float timeStep);
	/// <summary>
	/// ApplyForce is used to apply a certain force to the rigidbody given a vector 2
	/// </summary>
	void ApplyForce(vec2 force);
	/// <summary>
	/// ApplyForce is used to apply a certain force to the rigidbody given a vector 2 and a contact position
	/// </summary>
	void ApplyForce(vec2 force, vec2 pos);

	/// <summary>
	/// GetPosition is used to return m_position
	/// </summary>
	vec2 GetPosition() { return m_position; }
	/// <summary>
	/// GetOrientatation is used to return m_orientation
	/// </summary>
	float GetOrientatation() { return m_orientation; }
	/// <summary>
	/// GetElasticity is used to return m_elasticity
	/// </summary>
	float GetElasticity() { return m_elasticity; }
	/// <summary>
	/// GetVelocity is used to return m_velocity
	/// </summary>
	vec2 GetVelocity() { return m_velocity; }
	/// <summary>
	/// GetAngularVelocity is used to return m_angularVelocity
	/// </summary>
	float GetAngularVelocity() { return m_angularVelocity; }
	/// <summary>
	/// GetMass is used to return m_mass
	/// </summary>
	float GetMass() { return m_mass; }
	/// <summary>
	/// GetMoment is used to return m_moment
	/// </summary>
	float GetMoment() { return m_moment; }

	/// <summary>
	/// ResolveCollision is used to resolve the collision between 2 diffrent rigidbodies
	/// </summary>
	void ResolveCollision(Rigidbody* actor2, vec2 contact, vec2* collisionNormal = nullptr);

protected:
	vec2 m_position;
	vec2 m_velocity;
	float m_mass;
	float m_orientation;
	float m_elasticity;
	float m_angularVelocity;
	float m_moment;
	float m_linearDrag = 0.3f;
	float m_angularDrag = 0.3f;
};