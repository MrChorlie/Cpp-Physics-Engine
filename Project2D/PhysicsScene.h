#pragma once

#include "PhysicsObject.h"

#include <glm/glm.hpp>
#include <vector>

using namespace glm;

class PhysicsScene
{
protected:
	vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

public:
	/// <summary>
	/// Default Constructor
	/// </summary>
	PhysicsScene();
	~PhysicsScene();

	/// <summary>
	/// AddActor is used to add a physics object to the list of actors in the scene
	/// </summary>
	/// <param name="actor"></param>
	void AddActor(PhysicsObject* actor);
	/// <summary>
	/// RemoveActor is used to remove a physics object to the list of actors in the scene
	/// </summary>
	/// <param name="actor"></param>
	void RemoveActor(PhysicsObject* actor);
	/// <summary>
	/// Update is called once per frame
	/// </summary>
	/// <param name="deltaTime"></param>
	void Update(float deltaTime);
	/// <summary>
	/// Draw is used to draw every physics object in the scene
	/// </summary>
	void Draw();

	/// <summary>
	/// SetGravity is used to set the gravity constant
	/// </summary>
	/// <param name="gravity"></param>
	void SetGravity(const vec2 gravity) { m_gravity = gravity; }
	/// <summary>
	/// GetGravity returns the value of m_gravity
	/// </summary>
	/// <returns></returns>
	vec2 GetGravity() const { return m_gravity; }

	/// <summary>
	/// SetTimeStep is used to set the time step constant
	/// </summary>
	/// <param name="timeStep"></param>
	void SetTimeStep(const float timeStep) { m_timeStep = timeStep; }
	/// <summary>
	/// GetTimeStep returns the value of m_timestep
	/// </summary>
	/// <returns></returns>
	float GetTimeStep() const { return m_timeStep; }

	/// <summary>
	/// GetActorCount returns a count of the current actors
	/// </summary>
	/// <returns></returns>
	int GetActorCount() { return m_actors.size(); }

	/// <summary>
	/// CheckForCollision is ran every frame to see if a collision has occured
	/// </summary>
	void CheckForCollision();

	// Collision Resolution Functions
	static bool Plane2Plane(PhysicsObject* object1, PhysicsObject* object2);
	static bool Plane2Sphere(PhysicsObject* object1, PhysicsObject* object2);
	static bool Sphere2Plane(PhysicsObject* object1, PhysicsObject* object2);
	static bool Sphere2Sphere(PhysicsObject* object1, PhysicsObject* object2);
};