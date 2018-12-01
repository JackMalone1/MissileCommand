// author Jack Malone
//got help from Ben Miller on how to do collision between asteroid and the explosoion
#include "Game.h"
#include "MyVector2.h"
#include <iostream>


/// <summary>
/// default construcor
/// pass parameters fpr sfml window, setup m_exitGame
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800, 600, 32 }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setUpScene();
}

/// <summary>
/// default destructor for game, nothing to release
/// </summary>
Game::~Game()
{
}


/// <summary>
/// game loop running at 60fps
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == event.type) //user key press
		{
			if (sf::Keyboard::Escape == event.key.code)
			{
				m_exitGame = true;
			}
		}
		if (sf::Event::MouseButtonPressed == event.type) {
			processMouseEvents(event);
		}
	}
}

void Game::processMouseEvents(sf::Event t_event) {
	if (sf::Mouse::Left == t_event.mouseButton.button) {
		if (t_event.mouseButton.y < 500) {
			m_laser.clear();
			sf::Vector2f laserEnd = sf::Vector2f{ static_cast<float>(t_event.mouseButton.x), static_cast<float>(t_event.mouseButton.y) };
			updateLaser(laserEnd);
		}
	}
}

void Game::updateLaser(sf::Vector2f t_laserEnd) {

	sf::Vertex newLaserEnd{ t_laserEnd, sf::Color::Black };
	sf::Vector2f laserStart = sf::Vector2f{ 400, 420 };
	sf::Vertex laserStartPoint{ laserStart, sf::Color::Black }; // start point of line
	sf::Vector2f laserEnd = sf::Vector2f{ 400,420 };
	sf::Vertex laserEndPoint{ laserEnd, sf::Color::Black };
	m_laser.append(laserStartPoint);
	m_laser.append(newLaserEnd);
	sf::Vector2f unitVector = vectorUnitVector(t_laserEnd);
	moveLaser(unitVector, laserEnd, t_laserEnd);
}
void Game::moveLaser(sf::Vector2f t_unitVector, sf::Vector2f t_laserEndPoint, sf::Vector2f t_mouseClick) {
	//sf::Vector2f velocity{ 120.0f,120.0f };
	//t_unitVector = { t_unitVector.x / velocity.x,t_unitVector.y / velocity.y };
	if (t_laserEndPoint.y >= t_mouseClick.y) {
		t_laserEndPoint = (t_unitVector) - t_laserEndPoint;
		m_laser.append(t_laserEndPoint);
	}
}
/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}
}

/// <summary>
/// draw the frame and then switch bufers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(m_ground);
	m_window.draw(m_powerbar);
	m_window.draw(m_playerBase);
	m_window.draw(m_laser);
	m_window.draw(m_asteroid);
	m_window.draw(m_altitudeText);
	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_altitudeText.setFont(m_ArialBlackfont);
	m_altitudeText.setString("Altitude");
	m_altitudeText.setPosition(300.0f, 525.0f);
	m_altitudeText.setFillColor(sf::Color::White);
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);
}

void Game::setUpScene() {
	sf::Vector2f groundSize(800, 100);
	sf::Vector2f powerbarSize(200,50);
	sf::Vector2f playerBaseSize(80,80);
	sf::Vector2f asteroidStartPoint(200,50);
	sf::Vector2f asteroidEndPoint(500,500);
	sf::Vertex asteroidStart(asteroidStartPoint,sf::Color::Black);
	sf::Vertex asteroidEnd(asteroidEndPoint,sf::Color::Black);
	m_ground.setSize(groundSize);
	m_ground.setFillColor(sf::Color::Green);
	m_ground.setPosition(0, 500);
	m_powerbar.setSize(powerbarSize);
	m_powerbar.setFillColor(sf::Color::Red);
	m_powerbar.setPosition(50, 525);
	m_playerBase.setSize(playerBaseSize);
	m_playerBase.setFillColor(sf::Color::Red);
	m_playerBase.setPosition((float)(m_window.getSize().x / 2.0 - 40), 420);
	m_asteroid.append(asteroidStart);
	m_asteroid.append(asteroidEnd);
}
