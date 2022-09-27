#pragma once

#include <glm/glm.hpp>

using namespace glm;

/// <summary>
/// ShapeType is used to identify the current shape
/// </summary>
enum ShapeType {
	PLANE = 0,
	SPHERE,
	SHAPE_COUNT
};

class PhysicsObject
{
protected:
	/// <summary>
	/// Default Constructor
	/// </summary>
	PhysicsObject() {};
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	/// <summary>
	/// FixedUpdate is called when the timestep reaches the maximun
	/// </summary>
	virtual void FixedUpdate(vec2 gravity, float timeStep) = 0;
	/// <summary>
	/// Draw is used to draw the object to the screen
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// ResetPosition is used to reset the position of the object
	/// </summary>
	virtual void ResetPosition() {};

	/// <summary>
	/// GetShapeID is used to get the objects shape type
	/// </summary>
	int GetShapeID() { return m_shapeID; }

protected:
	ShapeType m_shapeID;
};