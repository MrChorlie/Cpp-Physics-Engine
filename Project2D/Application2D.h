#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Gizmos.h"
#include "PhysicsScene.h"

class Application2D : public aie::Application {
public:

	/// <summary>
	/// Default Constructor
	/// </summary>
	Application2D();
	virtual ~Application2D();

	/// <summary>
	/// Startup is used to start the functions of the application
	/// </summary>
	/// <returns></returns>
	virtual bool startup();
	/// <summary>
	/// Shutdown is used to stop the fuctions of the application
	/// </summary>
	virtual void shutdown();

	/// <summary>
	/// Update is called once every frame
	/// </summary>
	/// <param name="deltaTime"></param>
	virtual void update(float deltaTime);
	/// <summary>
	/// Draw is used to draw all the graphics
	/// </summary>
	virtual void draw();

	/// <summary>
	/// UILogic is used to update and draw the debug UI
	/// </summary>
	void UILogic();

	/// <summary>
	/// Seetup Walla is used to setup the outside walls in the scene
	/// </summary>
	void SetupWalls();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene*		m_physicsScene;

	float m_timer;
	vec4 m_backgroundColour;

	bool mouseButtonJustPressed;

	float newPosition[2];
	float newVelocity[2];
	float newMass;
	float newRadius;
	float newElasticity;
	float newColour[3];
};