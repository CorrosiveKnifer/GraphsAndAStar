#ifndef _BUTTON_H_
#define _BUTTON_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Button.h
// Description   	: A class in charge of a toggling button
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
#include <SFML/Graphics.hpp>

class BackBuffer;
class Button
{
	//Member Functions
public:
	Button();
	~Button();

	bool Initialise(sf::Sprite* btnSprite);
	bool Deinitialise();
	void Draw(BackBuffer& _buffer);

	void ToggleHover(const int& mouseX, const int& mouseY);
	bool MouseClickEvent(const int& mouseX, const int& mouseY);
	bool ToggleMouseClickEvent(const int& mouseX, const int& mouseY);
	bool IsMouseWithin(const int& mouseX, const int& mouseY);

	void SetActive(bool _active);
	void SetEnable(bool _enabled);
	bool IsActive();
private:

protected:

	//Member Data
public:

private:
	sf::Sprite* m_pSprite;
	sf::Color m_nonActive, m_active, m_hover, m_disable;
	bool m_isActive, m_isEnabled;
	unsigned int m_width, m_height;
	unsigned int m_x, m_y;
protected:
};

#endif // !_BUTTON_H_

