#include "Graphs.h"
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Graphs.cpp
// Description   	: A class in charge of calculating and displaying an A* algorithm
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//

//Local Includes
#include "Button.h"

//Library Includes
#include <iostream>

//Static Values:
int Graph::NODE_SIZE = 15;

//Constructor
Graph::Graph()
	: m_width(1280), m_height(720), m_mode(CreateMode::NONE), m_nextNodeName("A")
	, delayCurr(0.0f), delayUpdate(0), delayMax(1.0f) //Time
	, m_btnNode(0), m_btnUEdge(0), m_btnBEdge(0), m_btnClearEdge(0) //Buttons
	, m_btnClearAll(0), m_btnBFS(0), m_btnDFS(0), m_btnSlow(0), m_btnQuit(0) //Buttons
	, m_BFSRunning(false), m_DFSRunning(false), m_displayResult(false) //Booleans
	, mousePos(0, 0), tempEdge(0) //Used to create edges
{

}

//Destructor
Graph::~Graph()
{
	//Call Deinitialise to clear memory propperly.
}

//Initialise ( )
//
// Description:	
//		Initialises the window for use, this must be called before any
//		other function is called.
//
// @return	boolean			Status of the initialisation.
bool Graph::Initialise()
{
	//Window:
	BackBuffer* _b = new BackBuffer();
	_b->Initialise(m_width, m_height);
	_b->LoadFont("Assets/font.ttf");
	_b->SetFontSize(NODE_SIZE * 1.5);

	//Create Node Button:
	sf::Sprite* sp = _b->CreateSprite("Assets/btnNode.png");
	sp->setPosition(1030, 20);
	m_btnNode = new Button();
	m_btnNode->Initialise(sp);

	//Create U. Edge Button:
	sp = _b->CreateSprite("Assets/btnUEdge.png");
	sp->setPosition(1030, 100);
	m_btnUEdge = new Button();
	m_btnUEdge->Initialise(sp);
	
	//Create B. Edge Button:
	sp = _b->CreateSprite("Assets/btnBEdge.png");
	sp->setPosition(1030, 180);
	m_btnBEdge = new Button();
	m_btnBEdge->Initialise(sp);

	//Clear Edges Button:
	sp = _b->CreateSprite("Assets/btnClearEdges.png");
	sp->setPosition(1030, 260);
	m_btnClearEdge = new Button();
	m_btnClearEdge->Initialise(sp);

	//Clear All Button:
	sp = _b->CreateSprite("Assets/btnClearAll.png");
	sp->setPosition(1030, 340);
	m_btnClearAll = new Button();
	m_btnClearAll->Initialise(sp);

	//Run BFS Button:
	sp = _b->CreateSprite("Assets/btnBFS.png");
	sp->setPosition(1030, 420);
	m_btnBFS = new Button();
	m_btnBFS->Initialise(sp);

	//Run DFS Button:
	sp = _b->CreateSprite("Assets/btnDFS.png");
	sp->setPosition(1030, 500);
	m_btnDFS = new Button();
	m_btnDFS->Initialise(sp);

	//Slow Mode Button:
	sp = _b->CreateSprite("Assets/btnSlow.png");
	sp->setPosition(1030, 580);
	m_btnSlow = new Button();
	m_btnSlow->Initialise(sp);

	//Quit Button:
	sp = _b->CreateSprite("Assets/btnQuit.png");
	sp->setPosition(1030, 660);
	m_btnQuit = new Button();
	m_btnQuit->Initialise(sp);

	//Temp Edge for creating edges:
	tempEdge = new GEdge();
	tempEdge->isUnidirectional = false;

	return Window::Initialise(*_b);
}

//Deinitialise (  )
//
// Description:	
//		Deintialises the window and clears all memory used by this
//		object. Can be used to reuse this object by initialising again.
//
// @return	boolean			Status of the deinitialisation.
bool Graph::Deinitialise()
{
	m_btnNode->Deinitialise();
	delete m_btnNode;
	m_btnNode = 0;

	m_btnUEdge->Deinitialise();
	delete m_btnUEdge;
	m_btnUEdge = 0;

	m_btnBEdge->Deinitialise();
	delete m_btnBEdge;
	m_btnBEdge = 0;

	m_btnClearEdge->Deinitialise();
	delete m_btnClearEdge;
	m_btnClearEdge = 0;

	m_btnClearAll->Deinitialise();
	delete m_btnClearAll;
	m_btnClearAll = 0;

	m_btnBFS->Deinitialise();
	delete m_btnBFS;
	m_btnBFS = 0;

	m_btnDFS->Deinitialise();
	delete m_btnDFS;
	m_btnDFS = 0;

	m_btnSlow->Deinitialise();
	delete m_btnSlow;
	m_btnSlow = 0;

	m_btnQuit->Deinitialise();
	delete m_btnQuit;
	m_btnQuit = 0;

	delete tempEdge;
	tempEdge = 0;

	ClearNodes();

	m_pBackBuffer->Deinitialise();
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	return Window::Deinitialise();
}

//DoWindowLoop ( )
//
// Description:	
//		Starts the main window loop, keeping to a 60 frames.
//		Returns a true if the window was forced closed.
//
// @return	boolean		Status of the application if it should quit.
bool Graph::DoWindowLoop()
{
	m_Clock.restart(); //Clean up clock.
	do
	{
		float deltaTime = m_Clock.getElapsedTime().asSeconds();
		m_Clock.restart();

		m_pInputHandler->HandleMouseInput(*this);

		m_lag += deltaTime;
		if (m_lag >= sm_frameTime)
		{
			Process(sm_frameTime);
			m_lag -= sm_frameTime;
			Draw(*m_pBackBuffer);
		}
	} while (m_IsRunning);

	return m_HasQuit;
}

// MousePressed( const int& _mouseX, const int& _mouseY )
//
// Description:
//		Handles the mouse moved event. Depending on the location 
//		this function will either handles button clicks or the canvas.
//
//	@Param	const int&	mouse location on the x-axis.
//	@Param	const int&	mouse location on the y-axis.
//
//	@return  n/a
void Graph::MouseMoved(const int& _mouseX, const int& _mouseY)
{
	if (!(_mouseX < 1000 && _mouseY < 600))
	{	//Mouse location not on the canvas:
		m_btnNode->ToggleHover(_mouseX, _mouseY);
		m_btnUEdge->ToggleHover(_mouseX, _mouseY);
		m_btnBEdge->ToggleHover(_mouseX, _mouseY);
		m_btnClearEdge->ToggleHover(_mouseX, _mouseY);
		m_btnClearAll->ToggleHover(_mouseX, _mouseY);
		m_btnBFS->ToggleHover(_mouseX, _mouseY);
		m_btnDFS->ToggleHover(_mouseX, _mouseY);
		m_btnSlow->ToggleHover(_mouseX, _mouseY);
		m_btnQuit->ToggleHover(_mouseX, _mouseY);
	}
	else
	{	//Mouse location on the canvas:
		mousePos.x = _mouseX;
		mousePos.y = _mouseY;
	}
}


// MousePressed( const int& _mouseX, const int& _mouseY )
//
// Description:
//		Handles the mouse pressed event. Depending on the location 
//		this function will either handles button clicks or the canvas.
//
//	@Param	const int&	mouse location on the x-axis.
//	@Param	const int&	mouse location on the y-axis.
//
//	@return  n/a
void Graph::MousePressed(const int& _mouseX, const int& _mouseY)
{
	GNode* start;
	if (_mouseX < 1000 && _mouseY < 600) 
	{ //Mouse is within the canvas.
		switch (m_mode) //Depending on which mode:
		{
		default:
		case Graph::NONE:
			break;

		case Graph::NODE:
			CreateNode(_mouseX, _mouseY);
			break;

		case Graph::U_EDGE:
			//Display edge being created...
			if (tempEdge->from == 0)
			{
				tempEdge->from = GetFirstNodeOverlapping(sf::Vector2f(_mouseX, _mouseY));
				tempEdge->isUnidirectional = true;
			}
			else
			{
				tempEdge->to = GetFirstNodeOverlapping(sf::Vector2f(_mouseX, _mouseY));
				if (tempEdge->from == tempEdge->to)
					tempEdge->to = 0;
			}
			//or create edge:
			if (tempEdge->from != 0 && tempEdge->to != 0)
			{
				CreateEdge();
			}
			break;

		case Graph::B_EDGE:
			//Display edge being created...
			if (tempEdge->from == 0)
			{
				tempEdge->from = GetFirstNodeOverlapping(sf::Vector2f(_mouseX, _mouseY));
				tempEdge->isUnidirectional = false;
			}
			else
			{
				tempEdge->to = GetFirstNodeOverlapping(sf::Vector2f(_mouseX, _mouseY));
			}
			//or create edge:
			if (tempEdge->from != 0 && tempEdge->to != 0)
			{
				CreateEdge();
			}
			break;

		case Graph::BFS: //Only start if mouse is overlapping a node...
			start = GetFirstNodeOverlapping(sf::Vector2f(_mouseX, _mouseY));
			if (start != 0)
			{
				StartBFS(start);
			}
			break;

		case Graph::DFS://Only start if mouse is overlapping a node...
			start = GetFirstNodeOverlapping(sf::Vector2f(_mouseX, _mouseY));
			if (start != 0)
			{
				StartDFS(start);
			}
			break;

		}
	}
	else //Else mouse location is not on canvas, check buttons:
	{
		//Create Node button:
		if (m_btnNode->MouseClickEvent(_mouseX, _mouseY))
		{
			CleanNodes();
			DeactivateButtons();
			m_btnNode->SetActive(true);
			m_displayResult = false;
			tempEdge->from = 0;
			tempEdge->to = 0;
			m_mode = CreateMode::NODE;
		}

		//Create U.Edge button:
		if (m_btnUEdge->MouseClickEvent(_mouseX, _mouseY))
		{
			CleanNodes();
			DeactivateButtons();
			m_btnUEdge->SetActive(true);
			m_displayResult = false;
			tempEdge->from = 0;
			tempEdge->to = 0;
			m_mode = CreateMode::U_EDGE;
		}

		//Create B.Edge button:
		if (m_btnBEdge->MouseClickEvent(_mouseX, _mouseY))
		{
			CleanNodes();
			DeactivateButtons();
			m_btnBEdge->SetActive(true);
			m_displayResult = false;
			tempEdge->from = 0;
			tempEdge->to = 0;
			m_mode = CreateMode::B_EDGE;
		}

		//Clear Edges button:
		if (m_btnClearEdge->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_displayResult = false;
			CleanNodes();
			ClearEdges();
			system("CLS");
			OutputGraph();
			m_mode = CreateMode::NONE;
		}

		//Clear All button:
		if (m_btnClearAll->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_displayResult = false;
			m_nextNodeName = 'A';
			ClearEdges();
			ClearNodes();
			system("CLS");
			OutputGraph();
			m_mode = CreateMode::NONE;
		}

		//Run BFS button:
		if (m_btnBFS->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_displayResult = false;
			m_btnBFS->SetActive(true);
			tempEdge->from = 0;
			tempEdge->to = 0;
			m_search.clear();
			CleanNodes();
			m_mode = CreateMode::BFS;
		}

		//Run DFS button:
		if (m_btnDFS->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_displayResult = false;
			m_btnDFS->SetActive(true);
			tempEdge->from = 0;
			tempEdge->to = 0;
			m_search.clear();
			CleanNodes();
			m_mode = CreateMode::DFS;
		}

		//Slow mode button:
		if (m_btnSlow->ToggleMouseClickEvent(_mouseX, _mouseY))
		{
			if (m_btnSlow->IsActive())
			{
				delayUpdate = delayMax;
			}
			else
			{
				delayUpdate = 0;
			}
		}

		//Quit button:
		if (m_btnQuit->ToggleMouseClickEvent(_mouseX, _mouseY))
		{
			Quit();
		}
	}
}

// MouseReleased( const int& _mouseX, const int& _mouseY )
//
// Description:
//		Handles the mouse released event. Depending on the location 
//		this function will either handles button clicks or the canvas.
//
//	@Param	const int&	mouse location on the x-axis.
//	@Param	const int&	mouse location on the y-axis.
//
//	@return  n/a
void Graph::MouseReleased(const int& _mouseX, const int& _mouseY)
{
	//Do Nothing
}

// CreateNode( int _mouseX, int _mouseY )
//
// Description:
//		Creates a node with a center location at the mouse position.
//
//	@Param	int	mouse location on the x-axis.
//	@Param	int	mouse location on the y-axis.
//
//	@return  n/a
void Graph::CreateNode(int _mouseX, int _mouseY)
{
	//Create a temporary node:
	GNode* temp = new GNode();
	temp->worldPos = sf::Vector2f(_mouseX - NODE_SIZE, _mouseY - NODE_SIZE);
	temp->name = m_nextNodeName;
	temp->state = GNodeState::CLEAN;

	//Add temp node to the list of nodes:
	m_nodes.push_back(temp);

	//Update next node's name:
	if (m_nextNodeName.at(m_nextNodeName.length() - 1) == 'Z') //Last node finished with a 'Z'
	{
		//Increase length by 1 and reset to A;
		m_nextNodeName.at(m_nextNodeName.length() - 1) = 'A';
		m_nextNodeName = m_nextNodeName + "A";
	}
	else
	{
		//Increment name:
		m_nextNodeName.at(m_nextNodeName.length() - 1) += 1;
	}

	//Debug output:
	system("CLS");
	OutputGraph();
}

// CreateEdge( )
//
// Description:
//		Creates an edge from the tempEdge pointer.
//
//	@Param	int	mouse location on the x-axis.
//	@Param	int	mouse location on the y-axis.
//
//	@return  n/a
void Graph::CreateEdge()
{
	//Create another temportary edge (copying variables from tempEdge):
	GEdge* temp = new GEdge();
	temp->isUnidirectional = (m_mode == CreateMode::U_EDGE);
	temp->from = tempEdge->from;
	temp->to = tempEdge->to;

	tempEdge->from->edges.push_back(temp);
	if (!temp->isUnidirectional)
	{	//Add an edge back to -> from
		tempEdge->to->edges.push_back(temp);
	}

	//Clear tempEdge:
	tempEdge->from = 0;
	tempEdge->to = 0;

	//Add edge to list of edges:
	this->m_edges.push_back(temp);

	system("CLS");
	OutputGraph();
}

// Process( float dT )
//
// Description:
//		Process per frame of the window.
//		Calls the running function for both BFS and DFS.
//		Enables BFS and DFS buttons if a node exists.
//
//	@Param	float	Change in time since the last frame.
//
//	@return  n/a
void Graph::Process(float dT)
{
	//If a node exists:
	if (m_nodes.size() > 0)
	{
		m_btnUEdge->SetEnable(true);
		m_btnBEdge->SetEnable(true);
		m_btnBFS->SetEnable(true);
		m_btnDFS->SetEnable(true);
	}
	else //a node doesn't exists:
	{
		m_btnUEdge->SetEnable(false);
		m_btnBEdge->SetEnable(false);
		m_btnBFS->SetEnable(false);
		m_btnDFS->SetEnable(false);
	}

	//decrement delay:
	delayCurr -= dT;
	if (delayCurr <= 0)
	{	
		//Do a process of either DFS or BFS.
		delayCurr = delayUpdate;
		if (m_BFSRunning)
		{
			RunBFS();
		}
		else if (m_DFSRunning)
		{
			RunDFS();
		}
	}
}

//DrawContents ( BackBuffer& buffer )
//
// Description:	
//		Draws all of the frame's contents using the backbuffer before each
//		frame is rendered.
//
// @Param	BackBuffer&		Buffer to draw the frames too.
//
// @return	n/a
void Graph::DrawContents(BackBuffer& buffer)
{
	//Buttons:
	m_btnNode->Draw(buffer);
	m_btnUEdge->Draw(buffer);
	m_btnBEdge->Draw(buffer);
	m_btnClearEdge->Draw(buffer);
	m_btnClearAll->Draw(buffer);
	m_btnBFS->Draw(buffer);
	m_btnDFS->Draw(buffer);
	m_btnSlow->Draw(buffer);
	m_btnQuit->Draw(buffer);

	//Background:
	buffer.SetColour(sf::Color::White);
	buffer.DrawRectangle(0, 0, 1000, 600);

	buffer.SetColour(sf::Color::Black);

	//Edges:
	for (GEdge* e : m_edges)
	{
		DrawEdge(buffer, *e);
	}
	//Nodes: 
	for (GNode* n : m_nodes)
	{
		DrawNode(buffer, *n);
	}
	
	//Temporary edge:
	if (tempEdge->from != 0 && tempEdge->to == 0)
	{
		sf::Vector2f from(tempEdge->from->worldPos.x + NODE_SIZE, tempEdge->from->worldPos.y + NODE_SIZE);
		buffer.DrawLine(from, mousePos);
	}

	//Display status of BFS/DFS:
	buffer.SetColour(sf::Color::White);
	if (m_mode == CreateMode::BFS || m_mode == CreateMode::DFS)
	{
		if (!m_BFSRunning && !m_DFSRunning)
		{
			buffer.SetFontSize(NODE_SIZE * 1.5);
			buffer.SetFontAlign(Align::Left);
			buffer.DrawText("Select a node to start from...", 30, 610);
		}
	}
	else if (m_displayResult)//Display Done:
	{
		buffer.SetFontSize(NODE_SIZE * 1.5);
		buffer.SetFontAlign(Align::Left);
		buffer.DrawText("Done!", 30, 610);
	}

	//Display Output:
	DrawOverlay(buffer);

	//Reset colour:
	buffer.SetColour(sf::Color::Black);
}

//DrawNode ( BackBuffer& buffer,  GNode& n)
//
// Description:	
//		Draw a circle with the node name in the middle.
//		Green: searched. Blue: added to stack/queue. White: Clean. 
//
// @Param	BackBuffer&		Buffer to draw the frames too.
// @Param	GNode&			Node to be drawed.
//
// @return	n/a
void Graph::DrawNode(BackBuffer& buffer, GNode& n)
{
	switch (n.state) //Colour depends on state:
	{
	default:
	case CLEAN:
		buffer.DrawCircle(n.worldPos.x, n.worldPos.y, NODE_SIZE, false);
		break;

	case QUEUED:
	case STACKED:
		buffer.SetColour(sf::Color::Red);
		buffer.DrawCircle(n.worldPos.x, n.worldPos.y, NODE_SIZE);
		break;
	case SEARCHED:
		buffer.SetColour(sf::Color::Green);
		buffer.DrawCircle(n.worldPos.x, n.worldPos.y, NODE_SIZE);
		break;
	}

	//Draw name:
	buffer.SetColour(sf::Color::Black);
	buffer.SetFontAlign(Align::Centre);
	buffer.DrawText(n.name, n.worldPos.x + NODE_SIZE, n.worldPos.y);
	buffer.SetFontAlign(Align::Left);
}

//DrawEdge ( BackBuffer& buffer,  GEdge& n)
//
// Description:	
//		Draw a line between two nodes.
//		Green & Green: Both directions are accessable.
//		Green & Red: the path can't not be traversed into the red side.
//
// @Param	BackBuffer&		Buffer to draw the frames too.
// @Param	GEdge&			Edge to be drawed.
//
// @return	n/a
void Graph::DrawEdge(BackBuffer& buffer, GEdge& e)
{
	//Get start and finish location:
	sf::Vector2f from(e.from->worldPos.x + NODE_SIZE, e.from->worldPos.y + NODE_SIZE);
	sf::Vector2f to(e.to->worldPos.x + NODE_SIZE, e.to->worldPos.y + NODE_SIZE);

	//Colour depends on what type of edge (red = Can't enter that direction): 
	if (e.isUnidirectional)
	{
		buffer.DrawLine(from, to, sf::Color::Red, sf::Color::Green);
	}
	else
	{
		buffer.DrawLine(from, to, sf::Color::Green, sf::Color::Green);
	}
}

//DrawOverlay ( BackBuffer& buffer )
//
// Description:	
//		Draws the output of each node's name in the search list.
//
// @Param	BackBuffer&		Buffer to draw the frames too.
//
// @return	n/a
void Graph::DrawOverlay(BackBuffer& buffer)
{
	std::string outMsg = "Output: ";
	for (int i = 0; i < m_search.size(); i++)
	{
		if (i < m_search.size() - 1)
			outMsg += m_search.at(i)->name + ", ";
		else
			outMsg += m_search.at(i)->name;
	}
	buffer.DrawText(outMsg, 30, 630);

	std::string containMsg = "Container: ";
	for (GNode* n : m_displayList)
	{
		containMsg += n->name+", ";
	}
	buffer.DrawText(containMsg, 30, 660);
}

//OutputGraph ( BackBuffer& buffer )
//
// Description:	
//		Outputs the current system to the console for debuging.
//
// @Param	n/a
//
// @return	n/a
void Graph::OutputGraph()
{
	std::cout << "   <Debug view>   " << std::endl;
	std::cout << "Nodes: ";
	if (m_nodes.size() == 0)
		std::cout << "0" << std::endl;
	
	//Nodes of the system:
	for (int i = 0; i < m_nodes.size(); i++)
	{
		if (i != m_nodes.size() - 1)
			std::cout << m_nodes.at(i)->name + ", ";
		else
			std::cout << m_nodes.at(i)->name << std::endl;
	}

	//Edges of the system:
	std::cout << "Edge Count: " << m_edges.size() << std::endl;
	for (int i = 0; i < m_edges.size(); i++)
	{
		std::cout << "Edge " + std::to_string(i) + ": ";

		std::cout << m_edges.at(i)->from->name;

		if (m_edges.at(i)->isUnidirectional)
		{
			std::cout << " -> ";
		}
		else
		{
			std::cout << " <-> ";
		}
		std::cout << m_edges.at(i)->to->name << std::endl;
	}
}

//CleanNodes (  )
//
// Description:	
//		Updates all nodes within the system to GNodeState::CLEAN.
//
// @Param	n/a
//
// @return	n/a
void Graph::CleanNodes()
{
	for (GNode* n : m_nodes)
	{
		n->state = GNodeState::CLEAN;
	}
	m_search.clear();
}

//ClearNodes (  )
//
// Description:	
//		Removes all nodes within the system, which also removes all edges.
//
// @Param	n/a
//
// @return	n/a
void Graph::ClearNodes()
{
	std::vector<GNode*>::iterator iter = m_nodes.begin();
	ClearEdges();

	while (iter != m_nodes.end())
	{
		delete * iter;
		iter++;
	}
	m_nodes.clear();
}

//ClearEdges (  )
//
// Description:	
//		Removes all edges and edges connecting between nodes.
//
// @Param	n/a
//
// @return	n/a
void Graph::ClearEdges()
{
	//For each node, clear all edges.
	for (GNode* n : m_nodes)
	{
		n->edges.clear();
	}

	//Clear all edges within the system
	std::vector<GEdge*>::iterator iter = m_edges.begin();
	while (iter != m_edges.end())
	{
		delete * iter;
		iter++;
	}

	//Clear tempEdge
	if (tempEdge != 0)
	{
		tempEdge->from = 0;
		tempEdge->to = 0;
	}

	m_edges.clear();
}

//GetFirstNodeOverlapping ( sf::Vector2f postion )
//
// Description:	
//		Returns the node that is overlapping with the position provided.
//		May return nullptr if no node overlaps.
//
// @Param	sf::Vector2f position of the overlap.
//
// @return	GNode*	Node overlapping with the position.
GNode* Graph::GetFirstNodeOverlapping(sf::Vector2f postion)
{
	for (GNode* n : m_nodes)
	{
		//Circle vs Circle(circle with 0 width) collision.
		sf::Vector2f loc(n->worldPos.x + NODE_SIZE, n->worldPos.y + NODE_SIZE);
		if (std::powf(loc.x - postion.x, 2) + std::powf(loc.y - postion.y, 2) <= std::powf(NODE_SIZE + 0, 2))
		{
			return n;
		}
	}
	return 0;
}

//DoesEdgeExist ( GNode& from, GNode& to )
//
// Description:	
//		Returns the edge that is from and to the nodes provided.
//		May return nullptr if no edge exists.
//
// @Param	GNode&	Outbound node (from).
// @Param	GNode&	Inbound node (to).
//
// @return	GEdge*	First Edge found with both from and to matching.
GEdge* Graph::DoesEdgeExist(GNode& from, GNode& to)
{
	for (GEdge* e : m_edges)
	{
		if (e->from == &from && e->to == &to)
		{
			return e;
		}
	}
	return 0;
}


//StartBFS ( GNode* start )
//
// Description:	
//		Sets up the conditions for the system to start running breath-first search.
//
// @Param	GNode* Node inwhich to start the breath-first search from.
//
// @return	n/a
void Graph::StartBFS(GNode* start)
{
	CleanNodes();
	m_BFSRunning = true;
	SetEnableButtons(false);
	m_displayList.clear();

	start->state = GNodeState::QUEUED;
	m_BFS.push(start);
	m_displayList.push_back(start);
}

//RunBFS (  )
//
// Description:	
//		Main process of the breath-first search, called per frame.
//
// @Param	n/a
//
// @return	n/a
void Graph::RunBFS()
{
	//Pop the front of the queue and save it temporarily
	GNode* temp = m_BFS.front();
	m_BFS.pop();
	m_displayList.pop_front();

	//If the popped node hasn't been searched...
	if (temp->state != SEARCHED)
	{
		//Add to search and update the node's status.
		temp->state = GNodeState::SEARCHED;
		m_search.push_back(temp);
	}
	
	//For each edge from the popped node...
	for (GEdge* e : temp->edges)
	{
		//Get the other node on the other end of this edge:
		GNode* next = (temp == e->from) ? e->to : e->from;

		//Check if hasn't already searched or queued
		if (next->state != GNodeState::SEARCHED && next->state != GNodeState::QUEUED)
		{
			//Add it to the back of the queue.
			next->state = GNodeState::QUEUED;
			m_BFS.push(next);
			m_displayList.push_back(next);
		}
		//Else it is not added to the queue.
	}

	//Check if the BFS is empty (AKA it is done):
	if (m_BFS.empty())
	{
		StopBFS();
	}
}

//StopBFS (  )
//
// Description:	
//		Resets the system and notifies the user that the BFS is done.
//
// @Param	n/a
//
// @return	n/a
void Graph::StopBFS()
{
	SetEnableButtons(true);
	m_btnBFS->SetActive(false);
	m_displayResult = true;
	m_BFSRunning = false;
	m_mode = CreateMode::NONE;
}

//StartDFS ( GNode* start )
//
// Description:	
//		Sets up the conditions for the system to start running depth-first search.
//
// @Param	GNode* Node inwhich to start the depth-first search from.
//
// @return	n/a
void Graph::StartDFS(GNode* start)
{
	CleanNodes();
	m_DFSRunning = true;
	SetEnableButtons(false);
	m_displayList.clear();

	start->state = GNodeState::STACKED;
	m_DFS.push(start);
	m_displayList.push_front(start);
}

//RunDFS (  )
//
// Description:	
//		Main process of the depth-first search, called per frame.
//
// @Param	n/a
//
// @return	n/a
void Graph::RunDFS()
{
	//Temporary save the top of the stack.
	GNode* temp = m_DFS.top();

	//The top of the stack hasn't been searched before
	if (temp->state != GNodeState::SEARCHED)
	{
		//Update the node to searched list.
		temp->state = GNodeState::SEARCHED;
		m_search.push_back(temp);
	}

	//For each edge from the top node.
	for (GEdge* e : temp->edges)
	{	
		//Get the node on the other end of the edge.
		GNode* next = (temp == e->from) ? e->to : e->from;
		//If other node hasn't been searched or stacked before.
		if (next->state != GNodeState::SEARCHED && temp->state != GNodeState::STACKED)
		{
			//Add to stack
			m_DFS.push(next);
			m_displayList.push_front(next);
			next->state = GNodeState::STACKED;
			return; //Stop the loop.
		}
		//Else the other node has already been searched/Stacked.
	}

	//No new children, remove from the stack:
	temp->state = GNodeState::SEARCHED;
	m_DFS.pop();
	m_displayList.pop_front();

	//Check if the DFS is empty (AKA it is done):
	if (m_DFS.empty())
	{
		StopDFS();
	}
}

//StopDFS (  )
//
// Description:	
//		Resets the system and notifies the user that the DFS is done.
//
// @Param	n/a
//
// @return	n/a
void Graph::StopDFS()
{
	SetEnableButtons(true);
	m_btnDFS->SetActive(false);
	m_DFSRunning = false;
	m_displayResult = true;
	m_mode = CreateMode::NONE;
}

// DeactivateButtons (  )
//
// Description:	
//		Deactivate all buttons (except the toggle buttons).
//
// @return	n/a
void Graph::DeactivateButtons()
{
	m_btnNode->SetActive(false);
	m_btnUEdge->SetActive(false);
	m_btnBEdge->SetActive(false);
	m_btnClearEdge->SetActive(false);
	m_btnClearAll->SetActive(false);
	m_btnBFS->SetActive(false);
	m_btnDFS->SetActive(false);
}

// SetEnableButtons ( bool status )
//
// Description:	
//		Updates all button's enable variable (except toggle buttons and quit button).
//
// @Param	bool	Sets each button's enable variable.
//
// @return	n/a
void Graph::SetEnableButtons(bool status)
{
	m_btnNode->SetEnable(status);
	m_btnUEdge->SetEnable(status);
	m_btnBEdge->SetEnable(status);
	m_btnClearEdge->SetEnable(status);
	m_btnClearAll->SetEnable(status);
	m_btnBFS->SetEnable(status);
	m_btnDFS->SetEnable(status);
}