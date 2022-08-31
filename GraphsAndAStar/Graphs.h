#ifndef _GRAPHS_H_
#define _GRAPHS_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Graph.h
// Description   	: Class for displaying DFS and BFS
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Window.h"

//Library Include
#include <queue>
#include <stack>
#include <list>

//Forward Declaration
class Button;

//Structure prototyping
struct GEdge;

//Enums
enum GNodeState { CLEAN, QUEUED, STACKED, SEARCHED };

//Structures
struct GNode
{
	sf::Vector2f worldPos;
	std::string name;
	std::vector<GEdge*> edges;
	GNodeState state;
};

struct GEdge
{
	GEdge() { from = 0; to = 0;  isUnidirectional = false; };
	GNode* from;
	GNode* to;
	bool isUnidirectional;
};

class Graph : public Window
{
	//Member function
public:
	Graph(); //TODO output stack/queue.
	virtual ~Graph();

	//Construction and Destruction functions:
	bool Initialise();
	bool Deinitialise();
	
	//Main loop:
	virtual bool DoWindowLoop();

	//Inherited Mouse Functions:
	virtual void MouseMoved(const int& _mouseX, const int& _mouseY);
	virtual void MousePressed(const int& _mouseX, const int& _mouseY);
	virtual void MouseReleased(const int& _mouseX, const int& _mouseY);

private:
	enum CreateMode {NONE, NODE, U_EDGE, B_EDGE, BFS, DFS};

	//Create Functions:
	void CreateNode(int, int);
	void CreateEdge();

	//Drawing Functions:
	void DrawNode(BackBuffer& buffer, GNode& n);
	void DrawEdge(BackBuffer& buffer, GEdge& e);
	void DrawOverlay(BackBuffer& buffer);
	void OutputGraph();

	//Clearing and cleaning:
	void CleanNodes();
	void ClearNodes();
	void ClearEdges();

	//Collision Function
	GNode* GetFirstNodeOverlapping(sf::Vector2f postion);

	//Existance check:
	GEdge* DoesEdgeExist(GNode& from, GNode& to);

	//Breath-first search
	void StartBFS(GNode* start);
	void RunBFS();
	void StopBFS();

	//Depth-first search
	void StartDFS(GNode* start);
	void RunDFS();
	void StopDFS();

	//Button Control:
	void DeactivateButtons();
	void SetEnableButtons(bool status);

protected:
	//Inherited functions:
	virtual void DrawContents(BackBuffer& buffer);
	virtual void Process(float dT);

	//Member data
public:

private:
	static int NODE_SIZE;
	CreateMode m_mode;
	unsigned int m_width, m_height;
	std::string m_nextNodeName;

	//Buttons:
	Button* m_btnNode;
	Button* m_btnUEdge;
	Button* m_btnBEdge;
	Button* m_btnClearEdge;
	Button* m_btnClearAll;
	Button* m_btnBFS;
	Button* m_btnDFS;
	Button* m_btnSlow;
	Button* m_btnQuit;

	//Time: (seconds)
	float delayCurr;
	float delayUpdate;
	float delayMax;

	//Booleans:
	bool m_BFSRunning, m_DFSRunning, m_displayResult;
	
	//Used to create edges:
	GEdge* tempEdge;
	sf::Vector2f mousePos;
	
protected:
	//Data-Structures:
	std::vector<GNode*> m_nodes;
	std::vector<GEdge*> m_edges;
	std::vector<GNode*> m_search;
	std::queue<GNode*> m_BFS;
	std::stack<GNode*> m_DFS;
	std::list<GNode*> m_displayList;

};
#endif // !_GRAPHS_H_