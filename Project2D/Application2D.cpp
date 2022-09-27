#include "Application2D.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Gizmos.h"
#include "imgui.h"
#include "gl_core_4_4.h"

#include <glm/glm.hpp>

using aie::Gizmos;
using namespace glm;

/// <summary>
/// Default Constructor
/// </summary>
Application2D::Application2D() 
{
	// Initilise all extra values
	memset(newVelocity, 0, sizeof(newVelocity));
	memset(newColour, 0, sizeof(newColour));
	memset(newPosition, 0, sizeof(newPosition));

	newMass = 1.0f;
	newRadius = 20.0f;
	newElasticity = 0.5f;

	m_backgroundColour = vec4(0.25, 0.25, 0.25, 1.0f);

	mouseButtonJustPressed = false;
}

Application2D::~Application2D() {

}

/// <summary>
/// Startup is used to start the functions of the application
/// </summary>
/// <returns></returns>
bool Application2D::startup() {

	// increase the 2d line count to maximize the number of objects we can draw 
	Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetGravity(vec2(0, (-9.81f * 2)));
	m_physicsScene->SetTimeStep(0.01f);

	SetupWalls();

	return true;
}

/// <summary>
/// Shutdown is used to stop the fuctions of the application
/// </summary>
void Application2D::shutdown() {
	
	delete m_font;
	delete m_2dRenderer;

	Gizmos::destroy();
}

/// <summary>
/// Update is called once every frame
/// </summary>
/// <param name="deltaTime"></param>
void Application2D::update(float deltaTime) {

	m_timer += deltaTime;

	// input example
	aie::Input* input = aie::Input::getInstance();

	Gizmos::clear();

	// Create sphere on click
	if (input->isMouseButtonDown(0) && !mouseButtonJustPressed)
	{
		newPosition[0] = input->getMouseX();
		newPosition[1] = input->getMouseY();

		Sphere* sphere = new Sphere(vec2(newPosition[0], newPosition[1]),
			vec2(newVelocity[0], newVelocity[1]), newMass,
			newRadius, newElasticity, vec4(newColour[0], newColour[1], newColour[2], 1));

		m_physicsScene->AddActor(sphere);
		mouseButtonJustPressed = true;
	}

	if (input->isMouseButtonUp(0))
		mouseButtonJustPressed = false;

	m_physicsScene->Update(deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

/// <summary>
/// Draw is used to draw all the graphics
/// </summary>
void Application2D::draw() {

	// wipe the screen to the background colour 
	clearScreen();

	setBackgroundColour(m_backgroundColour.r, m_backgroundColour.g,
		m_backgroundColour.b, m_backgroundColour.a);

	// begin drawing sprites 
	m_2dRenderer->begin();

	m_physicsScene->Draw();

	// draw your stuff here! 
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

	UILogic();

	// done drawing sprites
	m_2dRenderer->end();
}

/// <summary>
/// UILogic is used to update and draw the debug UI
/// </summary>
void Application2D::UILogic()
{
	// Create the UI for shape creation
	ImGui::Begin("Create a Sphere");
		ImGui::SetWindowSize(ImVec2(400, 200));
		ImGui::SetWindowPos(ImVec2(200, 0));
		ImGui::SliderFloat2("Position", newPosition, 0, 1280);
		ImGui::SliderFloat2("Velocity", newVelocity, -2000, 2000);
		ImGui::SliderFloat("Mass", &newMass, 0.1, 1000);
		ImGui::SliderFloat("Radius", &newRadius, 0.1, 100);
		ImGui::SliderFloat("Elasticity", &newElasticity, 0.1, 1);
		ImGui::ColorEdit3("Colour", newColour);
		ImGui::Text("Click to Create Sphere");
	ImGui::End();

	// Create the UI for Debug menus
	int fps = (int)getFPS();

	ImGui::Begin("Debug");
		ImGui::SetWindowSize(ImVec2(200, 200));
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::Text("FPS: %i", fps);
		ImGui::Text("Actor Count: %i", m_physicsScene->GetActorCount());
		ImGui::Text("Press 'ESC' to exit");
		if (ImGui::Button("Exit"))
		{
			quit();
		}
	ImGui::End();
}

/// <summary>
/// Seetup Walla is used to setup the outside walls in the scene
/// </summary>
void Application2D::SetupWalls()
{
	Plane* plane = new Plane(vec2(0, 1), 60, vec4(1, 1, 1, 1));
	Plane* plane2 = new Plane(vec2(1, 0), 60, vec4(1, 1, 1, 1));
	Plane* plane3 = new Plane(vec2(-1, 0), -1220, vec4(1, 1, 1, 1));
	Plane* plane4 = new Plane(vec2(0, -1), -660, vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	m_physicsScene->AddActor(plane4);
}