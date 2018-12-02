// author Jack Malone
//got help from Ben Miller on how to do collision between asteroid and the explosoion
//got help from Josh
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
			m_laser.clear();
			processMouseEvents(event);
		}
	}
}

void Game::processMouseEvents(sf::Event t_event) {
	if (sf::Mouse::Left == t_event.mouseButton.button) {
		if (t_event.mouseButton.y < 500) {
			m_mouseClick = sf::Vector2f{ static_cast<float>(t_event.mouseButton.x), static_cast<float>(t_event.mouseButton.y) };
			m_laserEnd = sf::Vector2f{ 400,420 };
			m_updateLaser = true;
			m_exploding = false;
			m_explosionRadius = 10;
			m_laserlength = vectorLength(m_mouseClick - m_laserStart);
		}
	}
}

void Game::moveLaser() {
	m_laser.append(m_laserStartPoint);
	sf::Vector2f m_unitVector = vectorUnitVector(m_mouseClick - m_laserEnd);
	m_currentLaserLength = vectorLength(m_laserEnd - m_laserStart);
	if (m_currentLaserLength < m_laserlength) {
		m_unitVector = { m_unitVector.x * m_velocityLaser,m_unitVector.y * m_velocityLaser };
		m_laserEnd = m_laserEnd + (m_unitVector);
		sf::Vertex m_laserEndPoint{ m_laserEnd,sf::Color::Black };
		m_laser.append(m_laserEndPoint);
	}
	else {
		m_updateLaser = false;		
		m_exploding = true;
	}
}

void Game::drawExplosion() {
	m_laser.clear();
	if (m_explosionRadius < 40) {
		m_explosionRadius += 1;
		m_explosion.setRadius(m_explosionRadius);
	}
	else if(m_explosionRadius >= 40){
		m_explosion.setPosition(1000, 1000);
	}
	m_explosion.setFillColor(sf::Color::Red);
	sf::Vector2f m_explosionPosition{ m_laserEnd };
	m_explosion.setPosition(m_explosionPosition);

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
	
	if (m_updateLaser) {
		moveLaser();
	}
	if (m_exploding) {
		drawExplosion();
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
	if (m_updateLaser) {
		m_window.draw(m_laser);
	}
	else{
		m_window.draw(m_explosion);
	}
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
{/*
	if (!m_logoTexture.loadFromFile("ASSETS\\IMAGES\\SFML-LOGO.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading logo" << std::endl;
	}
	m_logoSprite.setTexture(m_logoTexture);
	m_logoSprite.setPosition(300.0f, 180.0f);
	*/
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
	m_explosion.setOrigin(m_explosion.getRadius() / 2, m_explosion.getRadius() / 2);
	sf::Vector2f m_explosionPosition{ -1000,1000 };
	m_explosion.setPosition(m_explosionPosition);
}
