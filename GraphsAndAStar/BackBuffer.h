#ifndef _BACK_BUFFER_H_
#define _BACK_BUFFER_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: BackBuffer.h
// Description   	: A Class in charge of the window and displaying on the window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//

//Library Includes
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

//Forward Declaration
class SpriteTrack;

enum Align
{
	Left, Centre, Right
};

//Implementation
class BackBuffer
{
	//Member function
public:
	BackBuffer();
	~BackBuffer();

	bool Initialise(int _w, int _h);
	bool Deinitialise();

	sf::Sprite* CreateSprite(std::string fileLoc);
	
	void PushTransform(const sf::Transform _transform);
	void PopTransform(const sf::Transform _transform);
	bool ExtendWindow(int dx, int dy);

	//Font Functions:
	bool LoadFont(std::string _fontLoc);
	void SetFontSize(int _size);
	void SetFontAlign(Align _align);

	//Drawing Functions:
	void Display() const;
	void Clear() const;
	void Draw(const sf::Drawable& _object);
	void DrawAt(const sf::Drawable& _object, sf::Vector2f _position);
	void DrawCircle(float x, float y, float sizeR, bool _IsFill = true);
	void DrawEllipse(float x, float y, float sizeR_x, float sizeR_y);
	void DrawRectangle(float x, float y, float w, float h);
	void DrawText(std::string msg, float x, float y);
	void DrawLine(sf::Vector2f from, sf::Vector2f to, sf::Color fromColour = sf::Color::Black, sf::Color toColour = sf::Color::Black);
	sf::RenderWindow* GetWindow() const;
	const sf::Vector2u GetWindowSize() const;

	void SetFullScreen(bool _isFullScreen);
	void SetWindowSize(sf::Vector2u size);
	void SetColour(sf::Color _colour);
	void SetScreenOffset(sf::Vector2i _shake);
private:
	
protected:
	//Member data
public:

private:
	sf::View viewport;

protected:
	unsigned int m_width, m_height;
	sf::RenderWindow* m_pWindow;
	sf::Vector2i m_WindowPos;

	std::map<std::string, sf::Texture*>* m_pTextureMap;

	sf::Transform m_transform, m_currShake;
	
	sf::Color m_GlobalColour;

	sf::Font* m_font;
	int m_fontSize;
	Align m_fontAlign;

	bool m_isFullScreen;
};
#endif //_BACK_BUFFER_H_