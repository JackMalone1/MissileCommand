// author Jack Malone
//got help from Ben Miller on how to do collision between asteroid and the explosoion
//got help from Josh
//got help from Aaron
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
	createAsteroid();
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
			if (!m_exploding) {
				processMouseEvents(event);
			}
			
		}
	}
}

void Game::processMouseEvents(sf::Event t_event) {
	if (sf::Mouse::Left == t_event.mouseButton.button) {
		if (t_event.mouseButton.y < 500) {
			m_mouseClick = sf::Vector2f{ static_cast<float>(t_event.mouseButton.x), static_cast<float>(t_event.mouseButton.y) };
			m_laserEnd = sf::Vector2f{ 400,420 };
			m_updateLaser = true;
			m_explosionRadius = 10;
			m_laserLength = vectorLength(m_mouseClick - m_laserStart);
		}
	}
}

void Game::moveLaser() {
	m_laser.append(m_laserStartPoint);
	sf::Vector2f m_unitVector = vectorUnitVector(m_mouseClick - m_laserEnd);
	m_currentLaserLength = vectorLength(m_laserEnd - m_laserStart);
	if (m_currentLaserLength < m_laserLength) {
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
		m_explosionRadius += 0.5;
		m_explosion.setRadius(float(m_explosionRadius));
		m_explosion.setOrigin(m_explosion.getRadius() / 2, m_explosion.getRadius() / 2);
		m_explosion.setFillColor(sf::Color::Red);
		sf::Vector2f m_explosionPosition{ m_laserEnd };
		m_explosion.setPosition(m_explosionPosition);
	}
	else if(m_explosionRadius >= 39){
		sf::Vector2f m_explosionPosition{ 1000,1000 };
		m_exploding = false;
	}
}

void Game::createAsteroid() {
	m_asteroid.clear();
	sf::Vector2f asteroidStartPoint(static_cast<float>(rand() % 800), static_cast<float>(0));
	sf::Vertex asteroidStart(m_asteroidStartPoint, sf::Color::Black);	
	m_currentAsteroidEnd = asteroidStartPoint;
	m_asteroidStartPoint = asteroidStartPoint;
	sf::Vertex asteroidEndCurrent{ m_currentAsteroidEnd,sf::Color::Black };
	m_asteroid.append(asteroidEndCurrent);
	m_asteroidEndPoint = sf::Vector2f(static_cast<float>(rand() % 800), static_cast<float>(500));
	//sf::Vector2f m_asteroidEndPoint(float(rand() % 800),float(500));
	sf::Vertex m_asteroidEnd(m_asteroidEndPoint, sf::Color::Black);
	m_asteroidLength = vectorLength(m_asteroidEndPoint - m_asteroidStartPoint);
	m_setUpAsteroid = false;
	m_moveAsteroid = true;
	m_asteroid.append(m_asteroidStart);
}
void Game::moveAsteroid() {
	m_asteroid.clear(); 
	sf::Vertex m_asteroidStart(m_asteroidStartPoint, sf::Color::Black);
	m_asteroid.append(m_asteroidStart);
	sf::Vector2f m_unitVectorAsteroid = vectorUnitVector(m_asteroidEndPoint - m_currentAsteroidEnd);
	m_asteroidCurrentLength = vectorLength(m_currentAsteroidEnd - m_asteroidStartPoint);
	if (m_currentAsteroidEnd.y < 500) {
		m_unitVectorAsteroid = { m_unitVectorAsteroid.x * m_velocityLaser,m_unitVectorAsteroid.y * m_velocityLaser };
		m_currentAsteroidEnd = m_currentAsteroidEnd + (m_unitVectorAsteroid);
		sf::Vertex m_asteroidEndCurrent{ m_currentAsteroidEnd,sf::Color::Black };
		m_asteroid.append(m_asteroidEndCurrent);
	}
	else {
		m_setUpAsteroid = true;
		m_moveAsteroid = false;
		m_asteroidCurrentLength = 0.0f;
		m_asteroidLength = 0.0f;
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
	if (m_updateLaser) {
		moveLaser();
	}
	if (m_exploding) {
		drawExplosion();
	}
	if (m_setUpAsteroid) {
		createAsteroid();
	}
	if (m_moveAsteroid) {
		moveAsteroid();
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
	else if(m_exploding) {
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
{
}

void Game::setUpScene() {
	sf::Vector2f groundSize(800, 100);
	sf::Vector2f powerbarSize(200,50);
	sf::Vector2f playerBaseSize(80,80);
	m_ground.setSize(groundSize);
	m_ground.setFillColor(sf::Color::Green);
	m_ground.setPosition(0, 500);
	m_powerbar.setSize(powerbarSize);
	m_powerbar.setFillColor(sf::Color::Red);
	m_powerbar.setPosition(50, 525);
	m_playerBase.setSize(playerBaseSize);
	m_playerBase.setFillColor(sf::Color::Red);
	m_playerBase.setPosition((float)(m_window.getSize().x / 2.0 - 40), 420);
	m_explosion.setOrigin(m_explosion.getRadius() / 2, m_explosion.getRadius() / 2);
	sf::Vector2f m_explosionPosition{ -1000,1000 };
	m_explosion.setPosition(m_explosionPosition);
}
