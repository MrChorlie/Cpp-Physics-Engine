#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include <iostream>

/// <summary>
	/// Default Constructor
	/// </summary>
PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(vec2(0, 0))
{
}

PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

/// <summary>
/// AddActor is used to add a physics object to the list of actors in the scene
/// </summary>
/// <param name="actor"></param>
void PhysicsScene::AddActor(PhysicsObject* actor)
{
	m_actors.push_back(actor);
}

/// <summary>
/// RemoveActor is used to remove a physics object to the list of actors in the scene
/// </summary>
/// <param name="actor"></param>
void PhysicsScene::RemoveActor(PhysicsObject* actor)
{
	remove(m_actors.begin(), m_actors.end(), actor);
}

/// <summary>
/// Update is called once per frame
/// </summary>
/// <param name="deltaTime"></param>
void PhysicsScene::Update(float deltaTime)
{
	// Update physics at a fixed time step

	static float accumlatedTime = 0.0f;
	accumlatedTime += deltaTime;

	while (accumlatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->FixedUpdate(m_gravity, m_timeStep);
		}

		accumlatedTime -= m_timeStep;
	}

	CheckForCollision();
}

/// <summary>
/// Draw is used to draw every physics object in the scene
/// </summary>
void PhysicsScene::Draw()
{
	for (auto pActor : m_actors)
	{
		pActor->Draw();
	}
}

#pragma region CollisionFunctions

// Function pointer array for doing our collisions
typedef bool (*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
	PhysicsScene::Plane2Plane, PhysicsScene::Plane2Sphere,
	PhysicsScene::Sphere2Plane, PhysicsScene::Sphere2Sphere
};

void PhysicsScene::CheckForCollision()
{
	// Check for collisions
	int actorCount = m_actors.size();

	// Need to check for collisions against all objects except this one
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];

			int shapeId1 = object1->GetShapeID();
			int shapeId2 = object2->GetShapeID();

			// Using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// Did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

bool PhysicsScene::Plane2Plane(PhysicsObject* object1, PhysicsObject* object2)
{
	return false;
}

bool PhysicsScene::Plane2Sphere(PhysicsObject* object1, PhysicsObject* object2)
{
	// 𝐷1 =(𝐶∙𝑁)−𝐷  −𝑅 
	//𝐷1 is the distance of the sphere surface to the plane surface
	//	• 𝐶 is the centre of the sphere
	//	• 𝑁 is the normal to the plane
	//	• 𝐷 is the distance of the plane from the origin
	//	• 𝑅 is the radius of the sphere
	// if the value for 𝐷1 is negative, it means that the sphere has collided with the plane. 

	Plane* plane = dynamic_cast<Plane*>(object1);
	Sphere* sphere = dynamic_cast<Sphere*>(object2);

	if (sphere != nullptr && plane != nullptr)
	{
		vec2 collisionNormal = plane->GetNormal();
		float sphereToPlane = dot(sphere->GetPosition(), plane->GetNormal()) - plane->GetDistance();

		float intersection = sphere->GetRadius() - sphereToPlane;
		float velocityOutOfPlane = dot(sphere->GetVelocity(), plane->GetNormal());

		if (intersection > 0 && velocityOutOfPlane < 0)
		{
			vec2 contact = sphere->GetPosition() + (collisionNormal * -sphere->GetRadius());
			plane->ResolveCollision(sphere, contact);
			return true;
		}
	}
	return false;
}

bool PhysicsScene::Sphere2Plane(PhysicsObject* object1, PhysicsObject* object2)
{
	return Plane2Sphere(object2, object1);
}

bool PhysicsScene::Sphere2Sphere(PhysicsObject* object1, PhysicsObject* object2)
{
	// try to cast objects to sphere and sphere 
	Sphere* sphere1 = dynamic_cast<Sphere*>(object1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(object2);
	// if we are successful then test for collision  
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float radius1 = sphere1->GetRadius();
		float radius2 = sphere2->GetRadius();

		// When the sum of the radii is more then the distance 
		// between the two spheres the objects have collided
		if ((radius1 + radius2) > distance(sphere1->GetPosition(), sphere2->GetPosition()))
		{
			sphere1->ResolveCollision(sphere2, 0.5f * (sphere1->GetPosition() + sphere2->GetPosition()));

			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}
#pragma endregion