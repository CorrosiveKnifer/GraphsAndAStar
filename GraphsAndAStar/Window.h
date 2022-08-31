#ifndef _WINDOW_H_
#define _WINDOW_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Window.h
// Description   	: A parent class handling the important functions and data all windows should contain.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//

//Library Includes
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

//Local Includes
#include "BackBuffer.h"
#include "InputHandler.h"

//Forward Declaration
class InputHandler;

//Frame Rate:
static const float sm_frameTime = 1.0f / 60.0f;

//Implementation
class Window
{
	//Member function
public:
	Window();
	virtual ~Window();

	bool Initialise(BackBuffer& _backBuffer);
	bool Deinitialise();

	virtual bool DoWindowLoop() = 0;

	//Mouse Functions:
	virtual void MouseMoved(const int& _mouseX, const int& _mouseY);
	virtual void MousePressed(const int& _mouseX, const int& _mouseY);
	virtual void MouseReleased(const int& _mouseX, const int& _mouseY);

	virtual void Quit();
private:

protected:
	void Draw(BackBuffer& buffer);
	void SetRunning(bool _running);

	//Main functions:
	virtual void DrawContents(BackBuffer& buffer) = 0;
	virtual void Process(float dT) = 0;
	
	//Member data
public:

private:

protected:
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;

	sf::Vector2f m_mousePos;
	sf::Clock m_Clock;
	float m_renderTime, m_timePassed, m_lag;
	bool m_IsRunning, m_HasQuit;
};

#endif //_WINDOW_H_