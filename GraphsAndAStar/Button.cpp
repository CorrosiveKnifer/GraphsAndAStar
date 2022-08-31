#include "Button.h"
#include "BackBuffer.h"
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Button.cpp
// Description   	: A class in charge of a toggling button
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//

//Constructor
Button::Button()
	: m_nonActive(128, 128, 128)
	, m_active(255, 255, 51)
	, m_hover(192, 192, 192)
	, m_disable(55, 55, 55)
	, m_isEnabled(true)
	, m_isActive(false)
{

}

//Destructor
Button::~Button()
{
	
}

//Initialise ( )
//
// Description:	
//		Initialises the button for use, this must be called before any
//		other function is called.
//
// @param	sf::Sprite*		button sprite.
//
// @return	boolean			Status of the initialisation.
bool Button::Initialise(sf::Sprite* btnSprite)
{
	m_pSprite = btnSprite;
	if (btnSprite != 0)
	{
		m_x = btnSprite->getPosition().x;
		m_y = btnSprite->getPosition().y;
		m_width = btnSprite->getTexture()->getSize().x;
		m_height = btnSprite->getTexture()->getSize().y;
		m_pSprite->setColor(m_nonActive);
	}
	
	return btnSprite != 0;
}

//Deinitialise (  )
//
// Description:	
//		Deintialises the button and clears all memory used by this
//		object. Can be used to reuse this object by initialising again.
//
// @return	boolean			Status of the deinitialisation.
bool Button::Deinitialise()
{
	delete m_pSprite;
	m_pSprite = 0;

	return true;
}

// Draw( BackBuffer& buffer )
//
// Description:	uses the buffer, to render the button to the window.
//				
// @Param	BackBuffer&		reference to the backbuffer where you want to draw to.
//
// @return	n/a
//
void Button::Draw(BackBuffer& _buffer)
{
	_buffer.Draw(*m_pSprite);
}

// ToggleHover( const int& mouseX, const int& mouseY )
//
// Description:	Detects if the the mouse location provided is overlapping. 
//				Handles the mouse position in a hovering manor.
//				
// @param	const int&		mouse position on the x-axis.
// @param	const int&		mouse position on the y-axis.
//
// @return	n/a
//
void Button::ToggleHover(const int& mouseX, const int& mouseY)
{
	if (m_isEnabled)
	{
		if (!m_isActive)
		{
			if (mouseX >= m_x && mouseX < m_x + m_width && mouseY >= m_y && mouseY < m_y + m_height)
			{
				m_pSprite->setColor(m_hover);
			}
			else
			{
				m_pSprite->setColor(m_nonActive);
			}
		}
	}
}

// MouseClickEvent( const int& mouseX, const int& mouseY )
//
// Description:	Detects if the the mouse location provided is overlapping. 
//				Handles the mouse position and activates the button.
//				
// @param	const int&		mouse position on the x-axis.
// @param	const int&		mouse position on the y-axis.
//
// @return	bool	status of the button being active. 
//
bool Button::MouseClickEvent(const int& mouseX, const int& mouseY)
{
	if (m_isEnabled)
	{
		if (!m_isActive)
		{
			if (mouseX >= m_x && mouseX < m_x + m_width && mouseY >= m_y && mouseY < m_y + m_height)
			{
				m_pSprite->setColor(m_active);
				m_isActive = true;
			}
		}
		return m_isActive;
	}
	return false;
}

// MouseClickEvent( const int& mouseX, const int& mouseY )
//
// Description:	Detects if the the mouse location provided is overlapping. 
//				Handles the mouse position and toggles the button.
//				
// @param	const int&		mouse position on the x-axis.
// @param	const int&		mouse position on the y-axis.
//
// @return	bool	status of the button being active. 
//
bool Button::ToggleMouseClickEvent(const int& mouseX, const int& mouseY)
{
	if (m_isEnabled)
	{
		if (mouseX >= m_x && mouseX < m_x + m_width && mouseY >= m_y && mouseY < m_y + m_height)
		{
			m_pSprite->setColor(m_active);
			m_isActive = !m_isActive;
			return true;
		}
		return false;
	}
	return false;
}

// IsMouseWithin( const int& mouseX, const int& mouseY )
//
// Description:	Detects if the mouse is overlapping with the button.
//				
// @param	const int&		mouse position on the x-axis.
// @param	const int&		mouse position on the y-axis.
//
// @return	bool	mouse is overlapping?
//
bool Button::IsMouseWithin(const int& mouseX, const int& mouseY)
{
	if (mouseX >= m_x && mouseX < m_x + m_width && mouseY >= m_y && mouseY < m_y + m_height)
	{
		return true;
	}
	return false;
}

// SetActive( bool _active )
//
// Description:	Manually set the activeness of this button.
//				
// @param	bool	new active status.
//
// @return	n/a
//
void Button::SetActive(bool _active)
{
	if (m_isEnabled)
	{
		m_isActive = _active;
		if (m_isActive)
		{
			m_pSprite->setColor(m_active);
		}
		else
		{
			m_pSprite->setColor(m_nonActive);
		}
	}
}

// SetEnable( bool _enabled )
//
// Description: enable or disable the button's functionality.
//				
// @param	bool	new enable status.
//
// @return	n/a
//
void Button::SetEnable(bool _enabled)
{
	if (m_isEnabled != _enabled)
	{
		if (_enabled)
		{
			if (m_isActive)
				m_pSprite->setColor(m_active);
			else
				m_pSprite->setColor(m_nonActive);
		}
		else
		{
			m_pSprite->setColor(m_disable);
			m_isActive = false;
		}
	}
	m_isEnabled = _enabled;
}

// SetEnable( )
//
// Description: return the status of this button's being active.
//
// @return	bool	status of this button's being active.
//
bool Button::IsActive()
{
	return m_isActive;
}