//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: AStarDisplay.cpp
// Description   	: A class in charge of calculating and displaying an A* algorithm
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "AStarDisplay.h"

//Local Include
#include "Button.h"

//Constructor
AStarDisplay::AStarDisplay()
	: m_width(1280), m_height(720), m_columns(0), m_rows(0) //Size
	, m_isWalls(false), m_showValues(false), m_isRunningAStar(false), m_isDisplayingPath(false) //Booleans
	, m_userMode(AStarDisplay::Mode::NONE), m_fontSize(15)
	, mousePressed(0), m_pStart(0), m_pEnd(0), m_nodes(0)//Pointers
	, m_btnStart(0), m_btnEnd(0), m_btnWall(0), m_btnRun(0), m_btnShow(0), m_btnClear(0), m_btnClearAll(0), m_btnQuit(0) //Buttons
	, m_time(0.0f), m_displayTime(0.0f) //Time
	, m_PrioQueue(), m_showNoPath(false)
{

}

//Destructor
AStarDisplay::~AStarDisplay()
{
	
}

//Initialise ( unsigned int columns, unsigned int rows )
//
// Description:	
//		Initialises the window for use, this must be called before any
//		other function is called.
//
// @Param	unsigned int	Columns.
// @Param	unsigned int	Rows.
//
// @return	boolean			Status of the initialisation.
bool AStarDisplay::Initialise(unsigned int columns, unsigned int rows)
{
	//Window
	BackBuffer* _b = new BackBuffer();
	_b->Initialise(m_width, m_height);
	_b->LoadFont("Assets/font.ttf");
	_b->SetFontSize(15);
	int xlimit = m_width - 280; // 1000
	int ylimit = m_height - 120; //600
	
	//Create the array of nodes:
	m_nodes = new Node*[rows];
	for (unsigned int i = 0; i < rows; i++)
	{
		m_nodes[i] = new Node[columns];
		for (unsigned int j = 0; j < columns; j++)
		{
			m_nodes[i][j].G = 0;
			m_nodes[i][j].H = 0;
			m_nodes[i][j].m_x = 0 + xlimit / static_cast<float>(columns) * j;
			m_nodes[i][j].m_y = 0 + ylimit / static_cast<float>(rows) * i;
			m_nodes[i][j].m_width = xlimit / static_cast<float>(columns);
			m_nodes[i][j].m_height = ylimit / static_cast<float>(rows);
			m_nodes[i][j].m_state = ANodeState::NONE;
			m_nodes[i][j].m_nextNode = 0;
		}
	}

	//Size of the array
	m_columns = columns;
	m_rows = rows;

	//Create Start Node button:
	sf::Sprite* sp = _b->CreateSprite("Assets/btnStart.png");
	sp->setPosition(1030, 20);
	m_btnStart = new Button();
	m_btnStart->Initialise(sp);

	//Create End Node button:
	sp = _b->CreateSprite("Assets/btnEnd.png");
	sp->setPosition(1030, 100);
	m_btnEnd = new Button();
	m_btnEnd->Initialise(sp);

	//Create Wall Node button:
	sp = _b->CreateSprite("Assets/btnWall.png");
	sp->setPosition(1030, 180);
	m_btnWall = new Button();
	m_btnWall->Initialise(sp);

	//Show values button:
	sp = _b->CreateSprite("Assets/btnShow.png");
	sp->setPosition(1030, 260);
	m_btnShow = new Button();
	m_btnShow->Initialise(sp);
	m_btnShow->SetEnable(m_nodes[0][0].m_width > 40 && m_nodes[0][0].m_height > 40);

	//Clear A* button:
	sp = _b->CreateSprite("Assets/btnClear.png");
	sp->setPosition(1030, 340);
	m_btnClear = new Button();
	m_btnClear->Initialise(sp);

	//Clear All button:
	sp = _b->CreateSprite("Assets/btnClearAll.png");
	sp->setPosition(1030, 420);
	m_btnClearAll = new Button();
	m_btnClearAll->Initialise(sp);

	//Run A* button:
	sp = _b->CreateSprite("Assets/btnRun.png");
	sp->setPosition(1030, 500);
	m_btnRun = new Button();
	m_btnRun->Initialise(sp);
	m_btnRun->SetEnable(false);

	//Quit button:
	sp = _b->CreateSprite("Assets/btnQuit.png");
	sp->setPosition(1030, 580);
	m_btnQuit = new Button();
	m_btnQuit->Initialise(sp);

	return Window::Initialise(*_b);
}

//Deinitialise (  )
//
// Description:	
//		Deintialises the window and clears all memory used by this
//		object.
//
// @return	boolean		Status of the deinitialisation.
bool AStarDisplay::Deinitialise()
{
	for (unsigned int i = 0; i < m_rows; ++i) {
		delete[] m_nodes[i];
	}
	delete[] m_nodes;

	m_btnStart->Deinitialise();
	delete m_btnStart;
	m_btnStart = 0;

	m_btnEnd->Deinitialise();
	delete m_btnEnd;
	m_btnEnd = 0;

	m_btnWall->Deinitialise();
	delete m_btnWall;
	m_btnWall = 0;

	m_btnRun->Deinitialise();
	delete m_btnRun;
	m_btnRun = 0;

	m_btnShow->Deinitialise();
	delete m_btnShow;
	m_btnShow = 0;

	m_btnClear->Deinitialise();
	delete m_btnClear;
	m_btnClear = 0;

	m_btnClearAll->Deinitialise();
	delete m_btnClearAll;
	m_btnClearAll = 0;

	m_btnQuit->Deinitialise();
	delete m_btnQuit;
	m_btnQuit = 0;

	delete mousePressed;
	mousePressed = 0;

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
bool AStarDisplay::DoWindowLoop()
{
	m_Clock.restart(); //Clean up clock.
	do
	{
		float deltaTime = m_Clock.getElapsedTime().asSeconds();
		m_Clock.restart();

		m_pInputHandler->HandleMouseInput(*this);

		m_lag += deltaTime;
		m_time += deltaTime; //Timer
		if (m_lag >= sm_frameTime)
		{
			Process(sm_frameTime);
			m_lag -= sm_frameTime;
			Draw(*m_pBackBuffer);
		}
	} while (m_IsRunning);

	return m_HasQuit;
}

// MouseMoved( const int& _mouseX, const int& _mouseY )
//
// Description:
//		Handles the mouse moved event. Depending on the location 
//		this function will either handles button clicks or the canvas.
//
//	@Param	const int&	mouse location on the x-axis.
//	@Param	const int&	mouse location on the y-axis.
//
//	@return  n/a
void AStarDisplay::MouseMoved(const int& _mouseX, const int& _mouseY)
{
	if (!(_mouseX < 1000 && _mouseY < 600))
	{
		m_btnStart->ToggleHover(_mouseX, _mouseY);
		m_btnEnd->ToggleHover(_mouseX, _mouseY);
		m_btnWall->ToggleHover(_mouseX, _mouseY);
		m_btnRun->ToggleHover(_mouseX, _mouseY);
		m_btnShow->ToggleHover(_mouseX, _mouseY);
		m_btnClear->ToggleHover(_mouseX, _mouseY);
		m_btnClearAll->ToggleHover(_mouseX, _mouseY);
		m_btnQuit->ToggleHover(_mouseX, _mouseY);
	}
	else if ((_mouseX < 1000 && _mouseX >= 0 && _mouseY < 600 && _mouseY >= 0))
	{
		if (mousePressed != 0)
		{
			//old (i,j)
			int oI = static_cast<int>(std::floor(mousePressed->y / (600.0f / m_rows)));
			int oJ = static_cast<int>(std::floor(mousePressed->x / (1000.0f / m_columns)));

			//new (i, j)
			int nI = static_cast<int>(std::floor(_mouseY / (600.0f / m_rows)));
			int nJ = static_cast<int>(std::floor(_mouseX / (1000.0f / m_columns)));

			if (oI != nI || oJ != nJ)
			{
				HandleBoardClickEvent(nI, nJ);
				mousePressed->x = static_cast<float>(_mouseX);
				mousePressed->y = static_cast<float>(_mouseY);
			}
		}
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
void AStarDisplay::MousePressed(const int& _mouseX, const int& _mouseY)
{
	if (_mouseX < 1000 && _mouseY < 600)
	{
		mousePressed = new sf::Vector2f(static_cast<float>(_mouseX), static_cast<float>(_mouseY));

		int nI = static_cast<int>(std::floor(_mouseY / (600.0f / m_rows)));
		int nJ = static_cast<int>(std::floor(_mouseX / (1000.0f / m_columns)));
		m_isWalls = !(m_nodes[nI][nJ].m_state == ANodeState::WALL);
		HandleBoardClickEvent(static_cast<int>(std::floor(_mouseY / (600.0f / m_rows))), static_cast<int>(std::floor(_mouseX / (1000.0f / m_columns))));
	}
	else
	{
		//Create start node button:
		if (m_btnStart->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_btnStart->SetActive(true);
			m_userMode = Mode::START;
		}

		//Create end node button:
		if (m_btnEnd->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_btnEnd->SetActive(true);
			m_userMode = Mode::END;
		}

		//Create wall node button:
		if (m_btnWall->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_btnWall->SetActive(true);
			m_userMode = Mode::WALL;
		}
		//Show value button:
		if (m_btnShow->ToggleMouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_showValues = m_btnShow->IsActive();
			if (m_userMode != Mode::RUN)
				m_userMode = Mode::NONE;
		}

		//Run A* button:
		if (m_btnRun->MouseClickEvent(_mouseX, _mouseY))
		{
			m_time = 0; //Reset Time
			SetEnabledButtons(false);
			m_userMode = Mode::RUN;
			StartAStar();
		}

		//Clear A* button:
		if (m_btnClear->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_userMode = Mode::NONE;
			Clear(false);
		}

		//Clear All button:
		if (m_btnClearAll->MouseClickEvent(_mouseX, _mouseY))
		{
			DeactivateButtons();
			m_btnRun->SetEnable(false);
			m_userMode = Mode::NONE;
			Clear(true);
		}

		//Quit button:
		if (m_btnQuit->MouseClickEvent(_mouseX, _mouseY))
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
void AStarDisplay::MouseReleased(const int& _mouseX, const int& _mouseY)
{
	//Forget mouse position.
	if (mousePressed != 0)
	{
		delete mousePressed;
		mousePressed = 0;
	}
}

//DoWindowLoop ( )
//
// Description:	
//		Starts the main window loop, keeping to a 60 frames.
//		Returns a true if the window was forced closed.
//
// @return	boolean		Status of the application if it should quit.
void AStarDisplay::DrawContents(BackBuffer& buffer)
{	
	//Draw all nodes within the system:
	m_pBackBuffer->SetColour(sf::Color::White);
	for (unsigned int i = 0; i < m_rows; i++)
	{
		for (unsigned int j = 0; j < m_columns; j++)
		{
			//Is the node a starting or ending point?
			if (m_pStart == &m_nodes[i][j] || m_pEnd == &m_nodes[i][j])
			{
				m_pBackBuffer->SetColour(sf::Color(255, 215, 0)); //Start/End colour
				m_pBackBuffer->DrawRectangle(m_nodes[i][j].m_x, m_nodes[i][j].m_y, m_nodes[i][j].m_width, m_nodes[i][j].m_height);
			}
			else
			{
				//Select color for node:
				switch (m_nodes[i][j].m_state)
				{
				default:
				case ANodeState::NONE:
					m_pBackBuffer->SetColour(sf::Color::White);
					break;
				case ANodeState::WALL:
					m_pBackBuffer->SetColour(sf::Color(55, 55, 55));
					break;
				case ANodeState::COLLECTED:
					m_pBackBuffer->SetColour(sf::Color(0, 204, 0));
					break;
				case ANodeState::SELECTED:
					m_pBackBuffer->SetColour(sf::Color(178, 34, 34));
					break;
				case ANodeState::PATH:
					m_pBackBuffer->SetColour(sf::Color(65, 185, 255));
					break;
				}
				m_pBackBuffer->DrawRectangle(m_nodes[i][j].m_x, m_nodes[i][j].m_y, m_nodes[i][j].m_width, m_nodes[i][j].m_height);
			}
			m_pBackBuffer->SetColour(sf::Color::White); //Reset Color
		}
	}

	//Draw values on each node:
	if (m_showValues)
	{
		DrawValues(buffer);
	}

	//Draw gridlines between nodes
	m_pBackBuffer->SetColour(sf::Color::Black);
	for (unsigned int i = 0; i < m_rows; i++)
	{
		m_pBackBuffer->DrawRectangle(0, -0.5f + static_cast<float>(m_nodes[0][0].m_height * i), static_cast<float>(m_width), 1);
	}
	for (unsigned int j = 0; j < m_columns; j++)
	{
		m_pBackBuffer->DrawRectangle(-0.5f + static_cast<float>(m_nodes[0][0].m_width * j), 0, 1, static_cast<float>(m_height));
	}

	//Draw Buttons
	m_btnStart->Draw(buffer);
	m_btnEnd->Draw(buffer);
	m_btnWall->Draw(buffer);
	m_btnShow->Draw(buffer);
	m_btnClear->Draw(buffer);
	m_btnClearAll->Draw(buffer);
	m_btnRun->Draw(buffer);
	m_btnQuit->Draw(buffer);

	//Draw overlay
	DrawOverlay(buffer);
}

// DrawValues( BackBuffer& buffer )
//
// Description:
//		Displays the values of each node within the system.
// 
//	@param	BackBuffer&		backbuffer to draw too.
//
//	@return  n/a
void AStarDisplay::DrawValues(BackBuffer& buffer)
{
	m_pBackBuffer->SetColour(sf::Color::Black);

	//For each node:
	for (unsigned int i = 0; i < m_rows; i++)
	{
		for (unsigned int j = 0; j < m_columns; j++)
		{
			if (m_pStart == &m_nodes[i][j])
			{	//Display the start node:
				m_pBackBuffer->SetFontAlign(Align::Centre);
				m_pBackBuffer->DrawText("S", m_nodes[i][j].m_x + m_nodes[i][j].m_width / 2, m_nodes[i][j].m_y + m_nodes[i][j].m_height / 2.0f - 7.5f);
			}
			else if (m_pEnd == &m_nodes[i][j])
			{	//Display the end node:
				m_pBackBuffer->SetFontAlign(Align::Centre);
				m_pBackBuffer->DrawText("E", m_nodes[i][j].m_x + m_nodes[i][j].m_width / 2, m_nodes[i][j].m_y + m_nodes[i][j].m_height / 2.0f - 7.5f);
			}

			if (m_nodes[i][j].m_state != ANodeState::WALL)
			{	//Display the H, G, F values of the node:
				int H = m_nodes[i][j].H;
				int G = m_nodes[i][j].G;
				int F = G + H;

				//F and H
				m_pBackBuffer->SetFontAlign(Align::Right);
				m_pBackBuffer->DrawText(std::to_string(F), m_nodes[i][j].m_x + m_nodes[i][j].m_width, m_nodes[i][j].m_y);
				m_pBackBuffer->DrawText(std::to_string(H), m_nodes[i][j].m_x + m_nodes[i][j].m_width, m_nodes[i][j].m_y + m_nodes[i][j].m_height - 15);

				//G
				m_pBackBuffer->SetFontAlign(Align::Left);
				m_pBackBuffer->DrawText(std::to_string(G), m_nodes[i][j].m_x, m_nodes[i][j].m_y + m_nodes[i][j].m_height - 15);
			}
		}
	}
}

// DrawOverlay( BackBuffer& buffer )
//
// Description:
//		Displays the status of the system before, during and after Running A*
// 
//	@param	BackBuffer&		backbuffer to draw too.
//
//	@return  n/a
void AStarDisplay::DrawOverlay(BackBuffer& buffer)
{
	buffer.SetFontSize(15);
	buffer.SetFontAlign(Align::Left);

	if (m_pStart == 0) //If there is no start node:
	{
		buffer.SetColour(sf::Color::Red);
		buffer.DrawText("No Start location has been placed.", 30, 610);
	}
	else if (m_pEnd == 0) //If there is no end node:
	{
		buffer.SetColour(sf::Color::Red);
		buffer.DrawText("No End location has been placed.", 30, 610);
	}
	else
	{	//Display the time:
		buffer.SetColour(sf::Color::White);
		buffer.DrawText("Time: " + std::to_string(m_displayTime) + "s", 30, 610);

		//Display the priority queue:
		if (m_PrioQueue == 0)
		{
			buffer.DrawText("Collected count: 0", 30, 630);
		}
		else
		{
			buffer.DrawText("Collected count: " + std::to_string(m_PrioQueue->size()), 30, 630);
		}

		//Display the search list:
		buffer.DrawText("Searched count: " + std::to_string(m_searchList.size()), 30, 650);

		//Display the path list:
		if (m_showNoPath)
		{
			//Error: no path exists:
			buffer.SetColour(sf::Color::Red);
			buffer.DrawText("Path length: NO PATH EXISTS", 30, 670);
		}
		else
		{
			buffer.DrawText("Path length: " + std::to_string(m_path.size()), 30, 670);
		}

	}

	//Reset the color:
	buffer.SetColour(sf::Color::Black);
}

// Process( float dT )
//
// Description:
//		Process per frame of the window.
//		Calls the main A* function per frame and displaying the path.
//
//	@Param	float	Change in time since the last frame.
//
//	@return  n/a
void AStarDisplay::Process(float dT)
{
	if (m_isRunningAStar)
	{
		RunAStar();
	}

	if (m_isDisplayingPath)
	{
		StartDisplayingPath();
	}
}

// HandleBoardClickEvent( int i, int j )
//
// Description:
//		Handles the mouse pressed event when it is within the canvas.
//
//	@param	int		Row location of the node to be changed.
//	@param	int		Column location of the node to be changed.
//
//	@return  n/a
void AStarDisplay::HandleBoardClickEvent(int i, int j)
{
	switch (m_userMode)
	{
	case AStarDisplay::Mode::START: //Create/move starting node
		if (m_pStart != 0)
			m_pStart->m_state = ANodeState::NONE;
		m_pStart = &m_nodes[i][j];
		m_nodes[i][j].m_state = ANodeState::PATH;
		m_btnRun->SetEnable(m_pStart != 0 && m_pEnd != 0);
		break;

	case AStarDisplay::Mode::END: //Create/move Ending node
		if (m_pEnd != 0)
			m_pEnd->m_state = ANodeState::NONE;
		m_pEnd = &m_nodes[i][j];
		m_nodes[i][j].m_state = ANodeState::PATH;
		m_btnRun->SetEnable(m_pStart != 0 && m_pEnd != 0);
		break;

	case AStarDisplay::Mode::WALL: //Toggle wall:
		if (m_isWalls)
			m_nodes[i][j].m_state = ANodeState::WALL;
		else
			m_nodes[i][j].m_state = ANodeState::NONE;

		break;
	default:
	case AStarDisplay::Mode::NONE:
		break;
	}
}

//Clear ( bool _all )
//
// Description:	
//		Clears all nodes. If true then it also clears walls and the start/end node.
//
// @param	bool	if walls, start/end node should be cleared too.
//
// @return	n/a
void AStarDisplay::Clear(bool _all)
{
	delete m_PrioQueue;
	m_PrioQueue = 0;

	m_searchList.clear();
	m_path.clear();

	m_displayTime = 0;
	m_time = 0;

	for (unsigned int i = 0; i < m_rows; i++)
	{
		for (unsigned int j = 0; j < m_columns; j++)
		{
			if (_all)
			{
				m_nodes[i][j].m_state = ANodeState::NONE;
			}
			else
			{
				if (m_nodes[i][j].m_state != ANodeState::WALL)
					m_nodes[i][j].m_state = ANodeState::NONE;
			}
			m_nodes[i][j].G = 0;
			m_nodes[i][j].H = 0;
		}
	}

	m_showNoPath = false;

	if (_all)
	{
		m_pStart = 0;
		m_pEnd = 0;
	}
	else
	{
		m_pStart->m_state = ANodeState::PATH;
		m_pEnd->m_state = ANodeState::PATH;
	}

	m_btnRun->SetEnable(m_pStart != 0 && m_pEnd != 0);
}

//StartAStar ( )
//
// Description:	
//		Sets up the conditions for the system to start running A* algorithm.
//
// @return	n/a
void AStarDisplay::StartAStar()
{
	m_isRunningAStar = true;

	Clear(false);

	m_PrioQueue = new std::priority_queue<Node*, std::vector<Node*>, AStarDisplay>();

	m_PrioQueue->push(m_pStart);
	m_pStart->m_nextNode = 0;
}

// RunAStar( )
//
// Description:
//		Single instance of A* star's processing to be called
//		per frame. Additionally it concludes the A* algorithm
//		once either a path is found or is found to not exist.
//
//	@return  n/a
void AStarDisplay::RunAStar()
{
	//Update timer:
	m_displayTime = m_time;

	//Pop the the front of the queue.
	Node* thisNode = m_PrioQueue->top();
	m_PrioQueue->pop();

	//Get array location of this node:
	int i = static_cast<int>(std::floor((thisNode->m_y + thisNode->m_height / 2) / (600.0f / m_rows)));
	int j = static_cast<int>(std::floor((thisNode->m_x + thisNode->m_width / 2) / (1000.0f / m_columns)));

	//Update the node to selected.
	m_nodes[i][j].m_state = ANodeState::SELECTED;

	//For all of the nodes arround it...
	for (int x = -1; x < 2; x++) //-1, 0, +1
	{
		for (int y = -1; y < 2; y++) //-1, 0, +1
		{
			//Add to the queue.
			if (x != 0 && y != 0) //Diagonal
			{
				AddNodeToQueue(thisNode, i + x, j + y, 14);
			}
			else if (!(x == 0 && y == 0)) //Adjacent
			{
				AddNodeToQueue(thisNode, i + x, j + y, 10);
			}
			//else x == 0 and y == 0
		}
	}

	m_searchList.push_back(thisNode);
	bool pathFound = thisNode == m_pEnd;
	m_isRunningAStar = !(pathFound || m_PrioQueue->empty());

	//A* has finished, start displaying the path:
	if (!m_isRunningAStar)
	{
		if(pathFound)
			CreatePath();
		m_isDisplayingPath = true;
		return;
	}
}

// AddNodeToQueue( Node* source, int i, int j, int dist )
//
// Description:
//		A sub-process of A* Algorithm, to add a copy of this node to the
//		priority queue.
//
//	@param	Node*	Source node that the additional node is being added from.
//	@param	int		Row location of the node to be added.
//	@param	int		Column location of the node to be added.
//	@param	int		Distance from the source node.
//
//	@return  n/a
void AStarDisplay::AddNodeToQueue(Node* source, int i, int j, int dist)
{
	if (i >= 0 && i < static_cast<int>(m_rows) && j >= 0 && j < static_cast<int>(m_columns))
	{
		//Check if adjacent is a wall.
		if (!CheckValid(source, i, j))
		{
			return; //Don't add
		}
		//else:

		//Location of the end:
		int Ei = static_cast<int>(std::floor((m_pEnd->m_y + m_pEnd->m_height / 2) / (600.0f / m_rows)));
		int Ej = static_cast<int>(std::floor((m_pEnd->m_x + m_pEnd->m_width / 2) / (1000.0f / m_columns)));
		
		//Straight distance to the end:
		int distI = static_cast<int>(std::sqrt(std::powf(static_cast<float>(Ei - i), 2)) * 10);
		int distJ = static_cast<int>(std::sqrt(std::powf(static_cast<float>(Ej - j), 2)) * 10);
		if (m_pEnd == &m_nodes[i][j])
		{
			distI = 0;
			distJ = 0;
		}

		//Check if the node hasn't already been collected or searched
		if (m_nodes[i][j].m_state != ANodeState::COLLECTED && m_nodes[i][j].m_state != ANodeState::SELECTED)
		{
			if (m_pStart != &m_nodes[i][j])
			{
				m_nodes[i][j].m_state = ANodeState::COLLECTED;
				m_nodes[i][j].G = source->G + dist;
				m_nodes[i][j].H = distI + distJ;
				m_nodes[i][j].m_nextNode = source;
				m_PrioQueue->push(&m_nodes[i][j]);
			}
		}//else update the node's distance and source
		else if (m_nodes[i][j].G > source->G + dist)
		{
			m_nodes[i][j].G = source->G + dist;
			m_nodes[i][j].m_nextNode = source;
		}
	}
}

// CheckValid( Node* source, int i, int j )
//
// Description:
//		Checks if the adjacent nodes between the source and the node at [i][j] is a wall.
// 
//	@param	Node*	Source node that the additional node is being added from.
//	@param	int		Row location of the node to be added.
//	@param	int		Column location of the node to be added.
//
//	@return  bool	if the node is valid to be searched.
bool AStarDisplay::CheckValid(Node* source, int i, int j)
{
	//if either the source or the node in question is a wall itself:
	if (source->m_state == ANodeState::WALL || m_nodes[i][j].m_state == ANodeState::WALL)
	{
		return false;
	}

	//Get row/column coordinates of the source.
	int Ei = static_cast<int>(std::floor((source->m_y + source->m_height / 2) / (600.0f / m_rows)));
	int Ej = static_cast<int>(std::floor((source->m_x + source->m_width / 2) / (1000.0f / m_columns)));

	//Check adjacent1:
	int distI = Ei - i;
	if (m_nodes[i + distI][j].m_state == ANodeState::WALL)
	{
		return false;
	}
	//Check adjacent2:
	int distJ = Ej - j;
	if (m_nodes[i][j + distJ].m_state == ANodeState::WALL)
	{
		return false;
	}
	//Note: Only two nodes can exist between two nodes. 
	return true;
}

// CreatePath(  )
//
// Description:
//		From the end node iterate till the start is found. Adding to a path list
//		then reversing the list so that the start is infront and the end is the last node.
//
//	@return  n/a
void AStarDisplay::CreatePath()
{
	Node* temp = m_pEnd;
	m_path.clear();
	while (temp != 0)
	{
		m_path.push_back(temp);
		temp = temp->m_nextNode;
	}
	std::reverse(m_path.begin(), m_path.end());
}

// StartDisplayingPath(  )
//
// Description:
//		Once per frame converts the nodes within the path from "Searched" to "path".
//		So that the color of the node changes.
//
//	@return  n/a
void AStarDisplay::StartDisplayingPath()
{
	//Update time
	m_displayTime = m_time;

	//If path exists
	if (!m_path.empty())
	{	
		//Find the first non path node within the path list.
		std::vector<Node*>::iterator iter = m_path.begin();
		while (iter != m_path.end())
		{
			if ((*iter)->m_state != ANodeState::PATH)
			{
				//Update it to path.
				(*iter)->m_state = ANodeState::PATH;
				break;
			}
			iter++;
		}

		//If path has finished displaying:
		if (iter == m_path.end())
		{
			m_isDisplayingPath = false;

			SetEnabledButtons(true);
		}
	}
	else //Path doesn't exists, stop the display.
	{
		m_showNoPath = true;
		m_isDisplayingPath = false;
		SetEnabledButtons(true);
	}
}

// DeactivateButtons (  )
//
// Description:	
//		Deactivate all buttons (except the toggle buttons).
//
// @return	n/a
void AStarDisplay::DeactivateButtons()
{
	m_btnStart->SetActive(false);
	m_btnEnd->SetActive(false);
	m_btnWall->SetActive(false);
	m_btnRun->SetActive(false);
	m_btnClear->SetActive(false);
	m_btnClearAll->SetActive(false);
	m_btnQuit->SetActive(false);
}

// SetEnableButtons ( bool status )
//
// Description:	
//		Updates all button's enable variable (except toggle buttons and quit button).
//
// @Param	bool	Sets each button's enable variable.
//
// @return	n/a
void AStarDisplay::SetEnabledButtons(bool isEnabled)
{
	m_btnStart->SetEnable(isEnabled);
	m_btnEnd->SetEnable(isEnabled);
	m_btnWall->SetEnable(isEnabled);
	m_btnRun->SetEnable(isEnabled);
	m_btnClear->SetEnable(isEnabled);
	m_btnClearAll->SetEnable(isEnabled);
}