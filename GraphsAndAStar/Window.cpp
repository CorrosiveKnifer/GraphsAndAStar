//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Window.cpp
// Description   	: Implementation of windows.h for polymorphism
// Author         	: Michael Jordan
// Mail         	: michael.jor8823@mediadesign.com
//

//Self include
#include "Window.h"
//Library Includes

//Local Includes

//Default Constructor
Window::Window()
	: m_pBackBuffer(0), m_pInputHandler(0)
	, m_IsRunning(true), m_HasQuit(false)
	, m_lag(0)
	, m_mousePos(0, 0)
{

}

//Deconstructor
Window::~Window()
{
	
}

// Initialise( BackBuffer* _backBuffer , SoundBuffer* _soundBuffer )
//
// Description:	Sets up the window's values for use. This function MUST
//				be called before using any of the other functions.
//
// @param	BackBuffer*		Pointer to this game's back buffer.
// @param	SoundBuffer*	Pointer to this game's sound buffer.
//
// @return	bool	Status of the initialisation.
//
bool Window::Initialise(BackBuffer& _backBuffer)
{
	m_pBackBuffer = &_backBuffer;

	m_pInputHandler = new InputHandler();
	m_pInputHandler->Initialise(*m_pBackBuffer);

	return true;
}

// Deinitialise( )
//
// Description:	Releases any memory the window's values for use.
//
// @param	n/a
//
// @return	bool	Status of the deinitialisation.
//
bool Window::Deinitialise()
{
	//Backbuffer isn't owned by this window.
	//SoundBuffer isn't owned by this window.
	if (m_pInputHandler != 0)
	{
		delete m_pInputHandler;
		m_pInputHandler = 0;
	}
	return true;
}

// Draw( BackBuffer& buffer )
//
// Description:	Clears, calls the DrawContents function then displays.
//				
// @Param	BackBuffer&		reference to the backbuffer where you want to draw to.
//
// @Return	n/a
//
void Window::Draw(BackBuffer& buffer)
{
	buffer.Clear();

	DrawContents(buffer);

	buffer.Display();
}

// SetRunning( bool _running )
//
// Description:	switch the window running boolean.
//				
// @Param	bool	boolean value for the window running.
//
// @Return	n/a
//
void Window::SetRunning(bool _running)
{
	m_IsRunning = _running;
}

// Quit()
//
// Description:	Sets the boolean variable, to quit and end all opperations.
//				
// @Param	n/a
//
// @Return	n/a
//
void Window::Quit()
{
	m_HasQuit = true;
	m_IsRunning = false;
}

// MouseMoved( const int& _mouseX, const int& _mouseY )
//
// Description:	Handle when the MouseMoved event is triggered.
//				
// @Param	const int&	Reference to the position of the mouse on the x-axis.
// @Param	const int&	Reference to the position of the mouse on the y-axis.
//
// @Return	n/a
//
void Window::MouseMoved(const int& _mouseX, const int& _mouseY)
{
	//Do Nothing
}

// MousePressed( const int& _mouseX, const int& _mouseY )
//
// Description:	Handle when the MouseClickPressed event is triggered.
//				
// @Param	const int&	Reference to the position of the mouse on the x-axis.
// @Param	const int&	Reference to the position of the mouse on the y-axis.
//
// @Return	n/a
//
void Window::MousePressed(const int& _mouseX, const int& _mouseY)
{
	//Do Nothing
}

// MouseReleased( const int& _mouseX, const int& _mouseY )
//
// Description:	Handle when the MouseClickReleased event is triggered.
//				
// @Param	const int&	Reference to the position of the mouse on the x-axis.
//
// @Return	n/a
//
void Window::MouseReleased(const int& _mouseX, const int& _mouseY)
{
	//Do Nothing
}