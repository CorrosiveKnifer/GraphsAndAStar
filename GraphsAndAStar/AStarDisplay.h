#ifndef _A_STAR_DISPLAY_H_
#define _A_STAR_DISPLAY_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: AStarDisplay.h
// Description   	: A class in charge of calculating and displaying an A* algorithm
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Window.h"

//Library Include
#include <queue>

//Forward Declaration
class Button;

//Enums
enum class ANodeState {NONE, WALL, COLLECTED, SELECTED, PATH};

//Structures
struct Node
{
	unsigned int G, H;
	float m_x, m_y, m_width, m_height;
	ANodeState m_state;
	Node* m_nextNode;
};

class AStarDisplay : public Window
{
	//Member functions
public:
	AStarDisplay();
	virtual ~AStarDisplay();

	//Construction and Destruction functions:
	bool Initialise(unsigned int columns, unsigned int rows);
	bool Deinitialise();

	//Main loop:
	virtual bool DoWindowLoop();
	
	//Inherited Mouse Functions:
	virtual void MouseMoved(const int& _mouseX, const int& _mouseY);
	virtual void MousePressed(const int& _mouseX, const int& _mouseY);
	virtual void MouseReleased(const int& _mouseX, const int& _mouseY);

	//Compare algorithm:
	bool operator()(Node* a, Node* b) { return (a->H + a->G) > (b->H + b->G); }
private:
	enum class Mode { NONE, START, END, WALL, RUN };
	
	//Handle and edit nodes:
	void HandleBoardClickEvent(int i, int j);
	void Clear(bool _all);

	//A* Functions
	void StartAStar();
	void RunAStar();
	void AddNodeToQueue(Node* source, int i, int j, int dist);
	bool CheckValid(Node* source, int i, int j);
	void CreatePath();
	void StartDisplayingPath();

	//Drawing Functions:
	void DrawValues(BackBuffer& buffer);
	void DrawOverlay(BackBuffer& buffer);
	
	//Button Control:
	void DeactivateButtons();
	void SetEnabledButtons(bool isEnabled);

protected:
	//Inherited functions:
	virtual void DrawContents(BackBuffer& buffer);
	virtual void Process(float dT);

	//Member Data
public:

private:
	unsigned int m_width, m_height, m_columns, m_rows;
	bool m_isWalls, m_showValues, m_isRunningAStar, m_isDisplayingPath;
	bool m_showNoPath;
	Mode m_userMode;
	int m_fontSize;

	//Mouse location:
	sf::Vector2f* mousePressed;
	
	//Time:
	float m_time;
	float m_displayTime;
	
	//Nodes:
	Node** m_nodes;
	Node* m_pStart;
	Node* m_pEnd;
	
	//Buttons:
	Button* m_btnStart;
	Button* m_btnEnd;
	Button* m_btnWall;
	Button* m_btnRun;
	Button* m_btnShow;
	Button* m_btnClear;
	Button* m_btnClearAll;
	Button* m_btnQuit;

protected:
	//Data-Structures
	std::priority_queue<Node*, std::vector<Node*>, AStarDisplay>* m_PrioQueue;
	std::vector<Node*> m_searchList;
	std::vector<Node*> m_path;
};
#endif // !_A_STAR_DISPLAY_H_
