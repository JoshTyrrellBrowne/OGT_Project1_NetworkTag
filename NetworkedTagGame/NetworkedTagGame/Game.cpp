#include "Game.h"

Game::Game()
{
	m_window = new sf::RenderWindow(sf::VideoMode(3000, 1500), "SFML works!");
	m_gameState = GameState::HostOrJoin; //initial state
	m_playerType = PlayerType::Undecided; //initial state
	m_chooseScreen = new ChoosingScreen(*m_window);
	m_clientPtr = nullptr; //null initially until player decides
	m_serverPtr = nullptr;
	//std::thread listenThread(ListenForNewConnectionThread, std::ref(serverPtr));
	/*std::thread listenThread(ListenForNewConnectionThread, std::ref(*serverPtr));
	listenThread.detach();*/
}

void Game::init(Client& t_client)
{
	m_clientPtr = &t_client;
}

void Game::update()
{
	switch (m_gameState)
	{
	case GameState::HostOrJoin:
		switch (m_chooseScreen->update())
		{
		case 0: //normal return, no choice made yet
			break;
		case 1: //player wants to be host
		{
			try {
				m_serverPtr = new Server(1111);
				std::thread listenThread(ListenForNewConnectionThread, std::ref(*m_serverPtr));
				listenThread.detach();
				m_clientPtr = new Client("127.0.0.1", 1111, *this);
				if (!m_clientPtr->Connect())
				{
					std::cout << "Failed to connect to server..." << std::endl;
				}
				m_playerType = PlayerType::Host;
				m_gameState = GameState::Play;
			}
			catch (...) {
				// Server failed to create, must be full!
				std::cout << "Server failed to create, must be in use, maybe try join a game." << std::endl;
				break;
			}
			break;
		}
		case 2: //player wants to join
			try {
				m_clientPtr = new Client("127.0.0.1", 1111, *this);
				if (!m_clientPtr->Connect())
				{
					std::cout << "Failed to connect to server..." << std::endl;
				}
				m_playerType = PlayerType::Client;
				m_gameState = GameState::Play;
			}
			catch (...) {
				//failed to connect, no server or maybe full server
				std::cout << "failed to connect, no server or maybe full server. Try Host!" << std::endl;
				break;
			}
			break;
		default:
			break;
		}
		break;
	case GameState::Play:
		HandleControls(); //check for key input and do actions
		break;
	default:
		break;
	}
}

void Game::render()
{
	m_window->clear();
	switch (m_gameState)
	{
	case GameState::HostOrJoin:
		m_chooseScreen->render();
		break;
	case GameState::Play:
		//m_window->draw(m_player.getCircle());
		for (auto& player : m_allPlayers)
		{
			m_window->draw(player.getCircle());
		}
		break;
	default:
		break;
	}
	m_window->display();
}

void Game::setUpWithID(int t_ID)
{


	for (int i = 0; i < 3; i++)
	{
		Player p = *new Player();
		p.setId(i);
		p.setPosition(sf::Vector2f(200 * i, 200 * i));
		m_allPlayers.push_back(p);
		m_myPlayerID = t_ID;
		/*if (i == t_ID)
		{

			m_player = &p;
		}*/
	}
	//m_player.setId(t_ID);

	//switch (t_ID)	
	//{
	//case 0: //Player 1 (host)
	//	m_player.setColor(sf::Color::Green);
	//	m_player.setPosition(sf::Vector2f(200,200));
	//	break;
	//case 1: //Player 2
	//	m_player.setColor(sf::Color::Blue);
	//	m_player.setPosition(sf::Vector2f(200, 200));
	//	break;
	//case 3: //Player 3
	//	m_player.setColor(sf::Color::White);
	//	m_player.setPosition(sf::Vector2f(200, 200));
	//	break;
	//default:
	//	break;
	//}
}

void Game::setPosition(int id, sf::Vector2f t_pos)
{
	for (Player& p : m_allPlayers)
	{
		if (p.getID() == id)
		{
			p.setPosition(t_pos);
		}
	}


	//if (m_player->getID() == id) //check if for player
	//{
	//	m_player->setPosition(t_pos);
	//	return;
	//}
	//for (Player enemy : m_allPlayers) // check enemies
	//{
	//	if (enemy.getID() == id)
	//	{
	//		enemy.setPosition(t_pos);
	//	}
	//}
}

void Game::tagThisPlayer()
{
	//m_player.tag();
	//for (Player enemy : m_allPlayers) //untag enemies
	//{
	//	enemy.unTag();
	//}
}

void Game::tagEnemyPlayer(int t_IdToTag)
{
	//m_player.unTag();
	//for (Player enemy : m_allPlayers) //tag the tagged enemy, untag other
	//{
	//	if (enemy.getID() == t_IdToTag)
	//		enemy.tag();
	//	else
	//		enemy.unTag();
	//}
}

void Game::HandleControls()
{
	//sf::Event Event;
	while (m_window->pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // window message
		{
			m_window->close();
		}
		if (sf::Event::KeyPressed == event.type) //user key press
		{
			if (sf::Keyboard::Up == event.key.code)
			{
				for (Player& player : m_allPlayers)
				{
					if (player.getID() == m_myPlayerID)
					{
						player.setPosition(player.getPosition() + sf::Vector2f(0, -10));
						PacketStructs::SetPosition setPosObj(player.getID(), player.getPosition().x, player.getPosition().y);
						Packet p = setPosObj.toPacket();
						//m_clientPtr->SendPacketType(PacketType::SetPosition);
						m_clientPtr->sendAll(p.buffer, p.size);
					}
				}
				
			}
			if (sf::Keyboard::Down == event.key.code)
			{
				for (Player& player : m_allPlayers)
				{
					if (player.getID() == m_myPlayerID)
					{
						player.setPosition(player.getPosition() + sf::Vector2f(0, 10));
						PacketStructs::SetPosition setPosObj(player.getID(), player.getPosition().x, player.getPosition().y);
						Packet p = setPosObj.toPacket();
						//m_clientPtr->SendPacketType(PacketType::SetPosition);
						m_clientPtr->sendAll(p.buffer, p.size);
					}
				}
			}
			if (sf::Keyboard::Right == event.key.code)
			{
				for (Player& player : m_allPlayers)
				{
					if (player.getID() == m_myPlayerID)
					{
						player.setPosition(player.getPosition() + sf::Vector2f(10, 0));
						PacketStructs::SetPosition setPosObj(player.getID(), player.getPosition().x, player.getPosition().y);
						Packet p = setPosObj.toPacket();
						//m_clientPtr->SendPacketType(PacketType::SetPosition);
						m_clientPtr->sendAll(p.buffer, p.size);
					}
				}
			}
			if (sf::Keyboard::Left == event.key.code)
			{
				for (Player& player : m_allPlayers)
				{
					if (player.getID() == m_myPlayerID)
					{
						player.setPosition(player.getPosition() + sf::Vector2f(-10, 0));
						PacketStructs::SetPosition setPosObj(player.getID(), player.getPosition().x, player.getPosition().y);
						Packet p = setPosObj.toPacket();
						//m_clientPtr->SendPacketType(PacketType::SetPosition);
						m_clientPtr->sendAll(p.buffer, p.size);
					}
				}
			}
		}
	}
}

void Game::ListenForNewConnectionThread(Server& t_server)
{
	while (true) {
		t_server.ListenForNewConnection();
	}
}

