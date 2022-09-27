#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"

class Plane : public PhysicsObject
{
public:
	/// <summary>
	/// Default Contructor
	/// </summary>
	Plane();
	Plane(vec2 normal, float distance, vec4 colour);
	~Plane() {};

	/// <summary>
	/// FixedUpdate is called when the timestep reaches the maximun
	/// </summary>
	virtual void FixedUpdate(vec2 gravity, float timeStep);
	/// <summary>
	/// Draw is used to draw the object to the screen
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// GetNormal returns m_normal
	/// </summary>
	vec2 GetNormal() { return m_normal; }
	/// <summary>
	/// GetDistance returns m_distanceToOrigin
	/// </summary>
	float GetDistance() { return m_distanceToOrigin; }

	/// <summary>
	/// ResolveCollision is used to resolve collisions between the plane and a rigidbody
	/// </summary>
	void ResolveCollision(Rigidbody* actor2, vec2 contact);

protected:
	vec2 m_normal;
	float m_distanceToOrigin;
	vec4 m_colour;
};

