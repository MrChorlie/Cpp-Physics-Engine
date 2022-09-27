#pragma once
#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
	Box() {};
	Box(vec2 position, vec2 velocity, float width, float height, 
		float mass, float orientation, float elasticity, vec4 colour);
	~Box() {};

	virtual void FixedUpdate(vec2 gravity, float timeStep);
	virtual void Draw();

	vec2 GetExtents() { return m_extents; }
	vec2 GetLocalX() { return m_localX; }
	vec2 GetLocalY() { return m_localY; }
	float GetWidth() { return m_extents.x * 2; }
	float GetHeight() { return m_extents.y * 2; }

	// Check if any of the other box's corners are in this box
	bool CheckBoxCorneres(const Box& box, vec2& contact, int& numContacts, float& pen, vec2& edgeNormal);

protected:
	vec2 m_extents;
	vec4 m_colour;

	// Store the local x and y axes of the box based on its angle of rotation
	vec2 m_localX;
	vec2 m_localY;
};