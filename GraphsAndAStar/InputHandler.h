#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: InputHandler.h
// Description   	: Handles the input from the keyboard and mouse on the window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8823@mediadesign.com
//
//Library Includes
#include <SFML/Graphics.hpp>

//Forward Declaration
class BackBuffer;
class AStarDisplay;
class LogoWindow;
class Window;
class Graph;

//Implementation
class InputHandler
{
	//Member function
public:
	InputHandler();
	~InputHandler();

	bool Initialise(const BackBuffer& buffer);

	void HandleMouseInput(Window& window);

private:

protected:

	//Member data
public:

private:
	
protected:
	const BackBuffer* m_pBackBuffer;
	bool m_KeyA, m_KeyD, m_KeySpace;
	bool m_KeyTilde, m_KeyEsc;
};
#endif // _INPUT_HANDLER_H_