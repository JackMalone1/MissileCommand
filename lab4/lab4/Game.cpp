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
	setupSprite();
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
			if (sf::Keyboard::T == event.key.code) {
				if (m_gameState == classic) {
					m_gameState = extraFeatures;
				}
				else {
					m_gameState = classic;
				}
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
			m_maxAltitude = float(500 - (m_powerbarWidth * 1.5));
			m_powerbarWidth = 0;
			m_powerbar.setSize(powerbarSize);
		}
	}
}

void Game::moveLaser() {
	m_laser.append(m_laserStartPoint);
	sf::Vector2f m_unitVector = vectorUnitVector(m_mouseClick - m_laserEnd);
	m_currentLaserLength = vectorLength(m_laserEnd - m_laserStart);
	if (m_currentLaserLength < m_laserLength && m_laserEnd.y > m_maxAltitude) {
		m_unitVector = { m_unitVector.x * m_velocityLaser,m_unitVector.y * m_velocityLaser };
		m_laserEnd = m_laserEnd + (m_unitVector);
		sf::Vertex m_laserEndPoint{ m_laserEnd,sf::Color::Yellow };
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
		if (m_gameState == classic) {
			m_explosion.setFillColor(sf::Color::Red);
		}
		else if (m_gameState == extraFeatures) {
			m_explosion.setFillColor(sf::Color::Transparent);
			m_explosion.setOutlineColor(sf::Color::Red);
			m_explosion.setOutlineThickness(0.5);
		}
		sf::Vector2f m_explosionPosition{ m_laserEnd };
		m_explosion.setPosition(m_explosionPosition);
		m_explosionSprite.setPosition(m_explosionPosition);
		checkCollisions();
	}
	else if(m_explosionRadius >= 40){
		sf::Vector2f m_explosionPosition{ 1000,1000 };
		m_explosionSprite.setScale(1.0f, 1.0f);
		m_exploding = false;
	}
}

void Game::createAsteroid() {
	m_asteroid.clear();
	sf::Vector2f asteroidStartPoint(static_cast<float>(rand() % 800), static_cast<float>(0));
	sf::Vertex asteroidStart(m_asteroidStartPoint, sf::Color::Red);	
	m_currentAsteroidEnd = asteroidStartPoint;
	m_asteroidStartPoint = asteroidStartPoint;
	sf::Vertex asteroidEndCurrent{ m_currentAsteroidEnd,sf::Color::Red };
	m_asteroid.append(asteroidEndCurrent);
	m_asteroidEndPoint = sf::Vector2f(static_cast<float>(rand() % 800), static_cast<float>(500));
	sf::Vertex m_asteroidEnd(m_asteroidEndPoint, sf::Color::Red);
	m_setUpAsteroid = false;
	m_moveAsteroid = true;
	m_asteroid.append(m_asteroidStart);
	if (m_asteroidsDestroyed > 0) {
		m_score += 5;
		m_scoreText.setString("Score: " + std::to_string(m_score));
	}
}
void Game::moveAsteroid() {
	m_asteroid.clear(); 
	sf::Vertex m_asteroidStart(m_asteroidStartPoint, sf::Color::Red);
	m_asteroid.append(m_asteroidStart);
	sf::Vector2f m_unitVectorAsteroid = vectorUnitVector(m_asteroidEndPoint - m_currentAsteroidEnd);

	if (m_currentAsteroidEnd.y < 500 ) {
		m_unitVectorAsteroid = { m_unitVectorAsteroid.x * m_velocityAsteroid,m_unitVectorAsteroid.y * m_velocityAsteroid };
		m_currentAsteroidEnd = m_currentAsteroidEnd + (m_unitVectorAsteroid);
		sf::Vertex m_asteroidEndCurrent{ m_currentAsteroidEnd,sf::Color::Red };
		m_asteroid.append(m_asteroidEndCurrent);
	}
	else {
		m_gameOver = true;
	}
}

void Game::checkCollisions() {
	float distanceBetween;//float to get the distance between asteroid current end and centre of explosion
	distanceBetween = vectorLength(m_laserEnd - m_currentAsteroidEnd);
	if (distanceBetween < m_explosionRadius) {
		m_moveAsteroid = false;
		m_waitToMakeAsteroid = 0;
	}
}

void Game::animatePowerBar() {
	if (m_powerbarState == increasing) {
		m_powerbarWidth += 5;
		if (m_powerbarWidth >= 250) {
			m_powerbarState = decreasing;
		}
		else {
			powerbarSize = sf::Vector2f{ m_powerbarWidth, 50.0f };
			m_powerbar.setSize(powerbarSize);
		}
	}
	else if (m_powerbarState == decreasing) {
		m_powerbarWidth -= 5;
		if (m_powerbarWidth <= 0) {
			m_powerbarState = increasing;
		}
		else {
			powerbarSize = sf::Vector2f{ m_powerbarWidth, 50.0f };
			m_powerbar.setSize(powerbarSize);
		}
	}
}

void Game::waitToMakeAsteroid() {
	//only reset to zero if the player destroys the asteroid so will only make a new time to wait then
	if (m_waitToMakeAsteroid == 0) {//only sets a new value for time to wait once the asteroid has been destroyed
		m_timeToWait = (rand() % 120) + 30; // player has a random time between half a second and 2.5 seconds before another asteroid
	}
	//makes sure that the asteroid doesn't stay on the screen after a collision
	m_asteroid.clear();
	m_waitToMakeAsteroid++;//increments the time so that it will set up the asteroid after the wait time
	if (m_waitToMakeAsteroid >= m_timeToWait) {//once it reaches its wait time it will set up the asteroid
		m_asteroidsDestroyed++;//increases the tally for how many they've destroyed as it only creates a new assteroid if they destroyed one 
		m_amountOfAsteroidsDestroyed.setString("Amount of asteroids destroyed: " + std::to_string(m_asteroidsDestroyed));//updates the string
		m_setUpAsteroid = true;//sets up the asteroid so it can move again after it was destroyed
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
	if (!m_gameOver) {
	
		if (m_updateLaser) {//moves the laser after the player clicks somewhere on the screen
			moveLaser();
		}
		if (m_exploding) {//explodes the laser once it reaches its end point
			drawExplosion();
		}
		if (m_setUpAsteroid) {//if it is not waiting to make asteroid it will set it up with random values before moving it
			createAsteroid();
		}
		else if(!m_setUpAsteroid && !m_moveAsteroid) {//only waits if asteroid is not firing and it is not being set up
			waitToMakeAsteroid();
		}
		if (m_moveAsteroid) {//moves asteroid after it has been set up with default values
			moveAsteroid();
		}
		if (!m_updateLaser) {//updates the power bar only if the laser is not updating
			animatePowerBar();
		}
	}
}

/// <summary>
/// draw the frame and then switch bufers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	if (m_gameState == classic) {
		m_window.draw(m_ground);
		m_window.draw(m_powerbar);
		m_window.draw(m_playerBase);
		m_window.draw(m_asteroid);
	}
	if (m_gameState == extraFeatures) {
		m_window.draw(m_onScreenArea);
		m_window.draw(m_groundSprite);
		m_window.draw(m_powerbar);
		m_window.draw(m_asteroid);
		m_window.draw(m_playerBaseSprite);
	}
	if (!m_gameOver) {
		if (m_updateLaser) {
			m_window.draw(m_laser);
		}
		else if (m_exploding) {
			if (m_gameState == classic) {
				m_window.draw(m_explosion);
			}
			else if(m_gameState == extraFeatures){
				m_window.draw(m_explosion);
				m_window.draw(m_explosionSprite);
			}
		}
		m_window.draw(m_altitudeText);
	}
	else {
		m_window.draw(m_gameOverText);
		m_window.draw(m_scoreText);
		m_window.draw(m_amountOfAsteroidsDestroyed);
	}
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
	m_altitudeText.setCharacterSize(24);
	m_altitudeText.setString("Altitude");
	m_altitudeText.setPosition(50.0f, 525.0f);
	m_altitudeText.setFillColor(sf::Color::White);

	m_scoreText.setFont(m_ArialBlackfont);
	m_scoreText.setCharacterSize(24);
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_scoreText.setPosition(50.0f, 525.0f);
	m_scoreText.setFillColor(sf::Color::White);

	m_gameOverText.setFont(m_ArialBlackfont);
	m_gameOverText.setCharacterSize(48);
	m_gameOverText.setString("Game Over");
	m_gameOverText.setPosition((float)m_window.getSize().x / 2, (float)m_window.getSize().y / 2);
	m_gameOverText.setFillColor(sf::Color::Yellow);

	m_amountOfAsteroidsDestroyed.setFont(m_ArialBlackfont);
	m_amountOfAsteroidsDestroyed.setCharacterSize(24);
	m_amountOfAsteroidsDestroyed.setString("Amount of asteroids destroyed: " + std::to_string(m_asteroidsDestroyed));
	m_amountOfAsteroidsDestroyed.setPosition(350.0f,525.0f);
	m_amountOfAsteroidsDestroyed.setFillColor(sf::Color::White);
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\background.png");
	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\background.png"))
	{
		// error...
	}
	m_onScreenArea.setTexture(m_backgroundTexture);

	m_playerBaseTexture.loadFromFile("ASSETS\\IMAGES\\playerTurret.png");
	if (!m_playerBaseTexture.loadFromFile("ASSETS\\IMAGES\\playerTurret.png")) {
		//error....
	}
	m_playerBaseSprite.setTexture(m_playerBaseTexture);
	m_playerBaseSprite.setPosition((float)(m_window.getSize().x / 2.0 - 40), 420);

	m_groundTexture.loadFromFile("ASSETS\\IMAGES\\ground.png");
	if (!m_groundTexture.loadFromFile("ASSETS\\IMAGES\\ground.png")) {
		//error....
	}
	m_groundSprite.setTexture(m_groundTexture);
	m_groundSprite.setPosition(0, 500);

	m_groundTexture.loadFromFile("ASSETS\\IMAGES\\ground.png");
	if (!m_groundTexture.loadFromFile("ASSETS\\IMAGES\\ground.png")) {
		//error....
	}
	m_groundSprite.setTexture(m_groundTexture);
	m_groundSprite.setPosition(0, 500);

	m_explosionTexture.loadFromFile("ASSETS\\IMAGES\\explosion.png");
	if (!m_explosionTexture.loadFromFile("ASSETS\\IMAGES\\explosion.png")) {
		//error....
	}
	m_explosionSprite.setTexture(m_explosionTexture);
	m_explosionSprite.setPosition(m_explosionPosition);
}

void Game::setUpScene() {
	sf::Vector2f groundSize(800, 100);
	sf::Vector2f playerBaseSize(80,80);
	m_ground.setSize(groundSize);
	m_ground.setFillColor(sf::Color::Green);
	m_ground.setPosition(0, 500);
	m_powerbar.setSize(powerbarSize);
	m_powerbar.setFillColor(sf::Color::Red);
	m_powerbar.setPosition(200, 525);
	m_playerBase.setSize(playerBaseSize);
	m_playerBase.setFillColor(sf::Color::Red);
	m_playerBase.setPosition((float)(m_window.getSize().x / 2.0 - 40), 420);
	m_explosion.setOrigin(m_explosion.getRadius() / 2, m_explosion.getRadius() / 2);
	sf::Vector2f m_explosionPosition{ -1000,1000 };
	m_explosion.setPosition(m_explosionPosition);
}
