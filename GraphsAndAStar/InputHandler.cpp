//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: InputHandler.cpp
// Description   	: Handles the input from the keyboard and mouse on the window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8823@mediadesign.com
//
//Self Include
#include "InputHandler.h"

//Local Includes
#include "BackBuffer.h"
#include "Window.h"
#include "AStarDisplay.h"

//Constructor
InputHandler::InputHandler()
	:	m_pBackBuffer(0) 
	,	m_KeyA(false), m_KeyD(false), m_KeySpace(false)
	,	m_KeyTilde(false), m_KeyEsc(false)
{

}

//Destructor
InputHandler::~InputHandler()
{
	//Doesn't handle BackBuffer destruction. 
	m_pBackBuffer = 0;
}

// Initialise( BackBuffer* buffer )
//
// Description:	Sets up the handler's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	BackBuffer*		Pointer to this game's back buffer.
//
// @return	boolean		Status of the initialisation.
//
bool InputHandler::Initialise(const BackBuffer& buffer)
{
	m_pBackBuffer = &buffer;
	return true;
}

// HandleGameInput( Game& _Game )
//
// Description:	Handles specific input for main game application.
//
// @param	Game&	reference to the game class.
//
// @return	n/a
//
void InputHandler::HandleMouseInput(Window& window)
{
	sf::Event event;
	sf::RenderWindow* _Rwindow = m_pBackBuffer->GetWindow();
//
//	//Poll All events and update key states
	while (_Rwindow->pollEvent(event))
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			window.MousePressed(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			window.MouseReleased(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.type == sf::Event::MouseMoved)
		{
			window.MouseMoved(event.mouseMove.x, event.mouseMove.y);
		}
		else if (event.type == sf::Event::Closed)
		{
			window.Quit();
		}
	}
}