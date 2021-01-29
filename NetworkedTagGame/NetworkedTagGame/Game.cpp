#include "Game.h"

Game::Game()
{
	m_window = new sf::RenderWindow(sf::VideoMode(3000, 1500), "SFML works!");
	m_gameState = GameState::HostOrJoin; //initial state
	m_playerType = PlayerType::Undecided; //initial state
	m_chooseScreen = new ChoosingScreen(*m_window);
	m_clientPtr = nullptr; //null initially until player decides
	m_serverPtr = nullptr;
	font.loadFromFile("Lato-Regular.ttf");
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
		if (m_myPlayerID == 0)
			HandleCollisions();
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
		m_window->draw(m_playerText);
		break;
	default:
		break;
	}
	m_window->display();
}

void Game::setUpWithID(int t_ID)
{
	m_myPlayerID = t_ID;
	m_playerText.setFont(font);
	m_playerText.setCharacterSize(20);
	m_playerText.setString("You");
	m_playerText.setFillColor(sf::Color::Blue);
	//m_playerText.setOrigin(sf::Vector2f(m_playerText.getGlobalBounds().width / 2, m_playerText.getGlobalBounds().height / 2));
	for (int i = 0; i < 3; i++)
	{
		Player p = *new Player();
		p.setId(i);
		switch (i)
		{
		case 0:
			p.setPosition(sf::Vector2f(200, 200));
			break;
		case 1:
			p.setPosition(sf::Vector2f(2800, 200));
			break;
		case 2:
		{
			p.setPosition(sf::Vector2f(1500, 1300));
			break;
		}
		default:
			break;
		}
		m_allPlayers.push_back(p);
	}
	if (t_ID == 2)
	{
		//After 3rd player joins, tag random player
		PacketStructs::TagPlayer tagRandomPlayer(rand() % 3);
		Packet p = tagRandomPlayer.toPacket();
		m_clientPtr->sendAll(p.buffer, p.size);
	}
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
}

void Game::tagPlayer(int idToTag)
{
	for (Player& p : m_allPlayers)
	{
		if (p.getID() == idToTag)
		{
			p.tag();
		}
		else
			p.unTag();
	}

	//m_player.tag();
	//for (Player enemy : m_allPlayers) //untag enemies
	//{
	//	enemy.unTag();
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
						m_clientPtr->sendAll(p.buffer, p.size);
					}
				}
			}
		}
	}
	for (Player& player : m_allPlayers)
	{
		if (player.getID() == m_myPlayerID)
		{
			m_playerText.setPosition(player.getPosition());
		}
	}
}

void Game::HandleCollisions()
{
	for (Player& player : m_allPlayers)
	{
		if (player.isTagged)
		{
			for (Player& playerCheck : m_allPlayers)
			{
				if (playerCheck.getID() != player.getID())
				{
					if (playerCheck.getCircle().getGlobalBounds().intersects(player.getCircle().getGlobalBounds()))
					{
						PacketStructs::TagPlayer tagPlayer(playerCheck.getID());
						Packet p = tagPlayer.toPacket();
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

