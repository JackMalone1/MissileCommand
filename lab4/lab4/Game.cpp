// author Jack Malone
//got help from Ben Miller on how to do collision between asteroid and the explosoion
//got help from Josh
//got help from Aaron
//slight delay between the collision and the sound happening. If sound happens before the audio quality is poor.
//when t is pressed the game changes to the sprites instead of the normal shapes from the basic version of the game
//whenever the user shoots a laser a firing sound is played and if the explosion intersects with the asteroid an explosion sound is played
//the aim of the game is to try and stop the asteroid from reaching the earth
//the player can stop the asteroid by shooting at the asteroid with the laser
//they will destroy the laser if the end point of the asteroid hits the explosion
//if the asteroid hits the ground a game over text displays and the user is no longer able to shoot the laser
//also the asteroid no longer moves down the screen
//the game displays the player's score and how many asteroids they destroyed at the end of the game
#include "Game.h"
#include "MyVector2.h"
#include <iostream>


/// <summary>
/// default construcor
/// pass parameters fpr sfml window, setup m_exitGame
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 800, 600, 32 }, "Missile Command Jack Malone" },
	m_exitGame{ false } //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setUpScene();//sets up the basic shapes for the screen
	createAsteroid();//sets up start point and ultimate end point for the asteroid after the wait period for making a new asteroid
	setupSprite();//sets up all the sprites with their textures and position on the screen for if the player presses t for the extra features
	setUpSounds();//loads sounds in and sets up the sound buffers
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
			if (sf::Keyboard::Escape == event.key.code)//if they press escape it will exit the game
			{
				m_exitGame = true;
			}
			if (sf::Keyboard::T == event.key.code) {
				//changes what mode the game is in whenever the user presses t
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
			//it only checks if what mouse button they pressed if the laser isn't exploding
			if (!m_exploding) {
				processMouseEvents(event);
			}
			
		}
	}
}

void Game::processMouseEvents(sf::Event t_event) {
	if (sf::Mouse::Left == t_event.mouseButton.button) {
		/*
		if they clciked above the ground then it will set the end point of the line to wherever they clicked
		it figures out the max altitude of the laser by minusing the width of the power bar multiplied by 1.5 from 500
		it resets the width of the power bar back to zero
		it plays a firing sound and changes the bool to true for controlling whether or not it should update the laser
		*/
		if (t_event.mouseButton.y < 500) {
			m_mouseClick = sf::Vector2f{ static_cast<float>(t_event.mouseButton.x), static_cast<float>(t_event.mouseButton.y) };
			m_laserEnd = sf::Vector2f{ 400,420 };
			m_updateLaser = true;
			m_explosionRadius = 10;
			m_laserLength = vectorLength(m_mouseClick - m_laserStart);
			m_maxAltitude = float(500 - (m_powerbarWidth * 1.5));
			m_powerbarWidth = 0;
			m_powerbar.setSize(powerbarSize);
			m_fireSound.play();
		}
	}
}

void Game::moveLaser() {
	/*
	appends the start of the laser every time to make sure it knows what point to start off from 
	it gets the unit vector for the line so it knows what direction to go in
	finds the length of the current line and if it less then the overall length of line it will increment
	also checks to make sure that it hasn't reached its max velocity yet
	if either of the conditions are false then it will stop drawing the laser and start drawing the explosion
	it increments the line by adding the unit vector multiplied by a velocity to the current end point of the line
	the current end point of the line starts off as the same as the start position of the line
	*/
	m_laser.append(m_laserStartPoint);
	sf::Vector2f m_unitVector = vectorUnitVector(m_mouseClick - m_laserEnd);
	m_currentLaserLength = vectorLength(m_laserEnd - m_laserStart);
	if (m_currentLaserLength < m_laserLength && m_laserEnd.y > m_maxAltitude) {
		m_unitVector = { m_unitVector.x * m_velocityLaser,m_unitVector.y * m_velocityLaser };
		m_laserEnd = m_laserEnd + (m_unitVector);
		sf::Vertex m_laserEndPoint{ m_laserEnd,sf::Color::Blue };
		m_laser.append(m_laserEndPoint);
	}
	else {
		m_updateLaser = false;		
		m_exploding = true;
	}
}

void Game::drawExplosion() {
	/*
	when the explosion is happening it clears the line and if it hasn't reached its max radius it increments it
	it changes its origin continuosly so it stays in the same position on the screen
	depending on what mode the game it is in it will either display the circle 
	or the sprite with a transparent circle with an outline around it
	it sets its position and then checks for collision
	it it gets to its max radius it stops the explosion and moves it off the screen
	*/
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
	//clears the line to get rid of the previous values
	m_asteroid.clear();
	/*
	gives the asteroid a random start and end then puts these into verteces
	these are then appended into the vertex array
	increments the score if they have destroyed at least one asteroid and then changes the values of the bools so 
	it starts to increment the asteroid
	*/
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
	m_asteroid.clear();//makes sure that there are no points from the previous increment in the vertex array that could change the look of the line
	sf::Vertex m_asteroidStart(m_asteroidStartPoint, sf::Color::Red);
	m_asteroid.append(m_asteroidStart);//re-adds the start point to the asteroid after it's been cleared
	//the unit vector is used to add it to the last end point and it will keep going in the same direction
	sf::Vector2f m_unitVectorAsteroid = vectorUnitVector(m_asteroidEndPoint - m_currentAsteroidEnd);

	if (m_currentAsteroidEnd.y < 500 ) {//if the asteroid hasn't got to the bottom of the screen yet it will increment the asteroid
		//unit vector is multiplied by a velocity to increase the speed it's going down the screen at
		m_unitVectorAsteroid = { m_unitVectorAsteroid.x * m_velocityAsteroid,m_unitVectorAsteroid.y * m_velocityAsteroid };
		//this is added to the current end to get a new end point and this is added to the vertex array
		m_currentAsteroidEnd = m_currentAsteroidEnd + (m_unitVectorAsteroid);
		sf::Vertex m_asteroidEndCurrent{ m_currentAsteroidEnd,sf::Color::Red };
		m_asteroid.append(m_asteroidEndCurrent);
	}
	else {
		//if it gets to the bottom of the screen then the player has lost the game
		m_gameOver = true;
	}
}

void Game::checkCollisions() {
	float distanceBetween;//float to get the distance between asteroid current end and centre of explosion
	distanceBetween = vectorLength(m_laserEnd - m_currentAsteroidEnd);
	if (distanceBetween < m_explosionRadius) {//if the distance is less than the radius of the explosion they intersected and a new asteroid is created
		m_moveAsteroid = false;
		m_waitToMakeAsteroid = 0;
		
	}
}

void Game::animatePowerBar() {
	//increases the size of the power bar until it gets to its max size or above and then starts decreasing the size again
	if (m_powerbarState == increasing) {
		m_powerbarWidth += 2.5;
		if (m_powerbarWidth >= 250) {
			m_powerbarState = decreasing;
		}
		else {
			powerbarSize = sf::Vector2f{ m_powerbarWidth, 50.0f };
			m_powerbar.setSize(powerbarSize);
		}
	}
	else if (m_powerbarState == decreasing) {//decreases the size of the power bar until it gets to zero or below and then increases the size again
		m_powerbarWidth -= 2.5;
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
		m_collisionSound.play();//plays the collision sound before it creates the next asteroid
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
	if (m_gameState == classic) {//draws the basic version of each of the objects before they press t
		m_window.draw(m_ground);
		m_window.draw(m_powerbar);
		m_window.draw(m_playerBase);
		m_window.draw(m_asteroid);
	}
	if (m_gameState == extraFeatures) {// after they press t the textures will be displayed
		m_window.draw(m_onScreenArea);
		m_window.draw(m_groundSprite);
		m_window.draw(m_powerbar);
		m_window.draw(m_asteroid);
		m_window.draw(m_playerBaseSprite);
	}
	if (!m_gameOver) {
		if (m_updateLaser) {//when the laser is updating it will draw the laser but if the player didn't click anywhere it won't draw it
			m_window.draw(m_laser);
		}
		else if (m_exploding) {
			if (m_gameState == classic) {//it will draw the normal explosion if it is in regular mode
				m_window.draw(m_explosion);
			}
			else if(m_gameState == extraFeatures){//it will draw the explosion with the sprite and transparent circle in the extra features mode
				m_window.draw(m_explosion);
				m_window.draw(m_explosionSprite);
			}
		}
		m_window.draw(m_altitudeText);//it always draws the altitude text as long as the game is not over
	}
	else {//displays the game over text, score text and how many asteroids the player destroyed when the game is over
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
	//loads in the font that is used for the text
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

	//sets up the altitude text and gives it a position on the screen
	m_altitudeText.setFont(m_ArialBlackfont);
	m_altitudeText.setCharacterSize(24);
	m_altitudeText.setString("Altitude");
	m_altitudeText.setPosition(50.0f, 525.0f);
	m_altitudeText.setFillColor(sf::Color::White);

	//sets up the score text with its font and original string as well as where it is displayed on the screen
	m_scoreText.setFont(m_ArialBlackfont);
	m_scoreText.setCharacterSize(24);
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_scoreText.setPosition(50.0f, 525.0f);
	m_scoreText.setFillColor(sf::Color::White);

	//sets up the game over text with its font and original string as well as where it is displayed on the screen
	m_gameOverText.setFont(m_ArialBlackfont);
	m_gameOverText.setCharacterSize(48);
	m_gameOverText.setString("Game Over");
	m_gameOverText.setPosition((float)m_window.getSize().x / 2, (float)m_window.getSize().y / 2);
	m_gameOverText.setFillColor(sf::Color::Yellow);

	//sets up the asteroids destroyed text with its font and original string as well as where it is displayed on the screen
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
	//loading in the texture for the background when it displays the sprite
	m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\background.png");
	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\background.png"))
	{
		// error...
	}
	m_onScreenArea.setTexture(m_backgroundTexture);

	//loading in the texture for the sprite for the player and giving it the same position as the original player
	m_playerBaseTexture.loadFromFile("ASSETS\\IMAGES\\playerTurret.png");
	if (!m_playerBaseTexture.loadFromFile("ASSETS\\IMAGES\\playerTurret.png")) {
		//error....
	}
	m_playerBaseSprite.setTexture(m_playerBaseTexture);
	m_playerBaseSprite.setPosition((float)(m_window.getSize().x / 2.0 - 40), 420);

	//loading in the texture for the ground and giving it the same position as the original ground
	m_groundTexture.loadFromFile("ASSETS\\IMAGES\\ground.png");
	if (!m_groundTexture.loadFromFile("ASSETS\\IMAGES\\ground.png")) {
		//error....
	}
	m_groundSprite.setTexture(m_groundTexture);
	m_groundSprite.setPosition(0, 500);

	//loading in the texture for the explosion and giving it the same position as the original explosion
	m_explosionTexture.loadFromFile("ASSETS\\IMAGES\\explosion.png");
	if (!m_explosionTexture.loadFromFile("ASSETS\\IMAGES\\explosion.png")) {
		//error....
	}
	m_explosionSprite.setTexture(m_explosionTexture);
	m_explosionSprite.setPosition(m_explosionPosition);
}

void Game::setUpScene() {
	//giving the sizes for both the ground and the player
	sf::Vector2f groundSize(800, 100);
	sf::Vector2f playerBaseSize(80,80);

	//setting the size for the ground, its colour and where it is placed on the screen
	m_ground.setSize(groundSize);
	m_ground.setFillColor(sf::Color::Green);
	m_ground.setPosition(0, 500);

	//setting the size of the powerbar, its colour and where it is on the screen
	m_powerbar.setSize(powerbarSize);
	m_powerbar.setFillColor(sf::Color::Red);
	m_powerbar.setPosition(200, 525);

	//setting the size of the playerr, they're colour and where they are on the screen
	m_playerBase.setSize(playerBaseSize);
	m_playerBase.setFillColor(sf::Color::Red);
	m_playerBase.setPosition((float)(m_window.getSize().x / 2.0 - 40), 420);

	//setting the origin of the explosion to its centre so it stays in place when it is expanding instead of moving down the screen
	m_explosion.setOrigin(m_explosion.getRadius() / 2, m_explosion.getRadius() / 2);
	sf::Vector2f m_explosionPosition{ -1000,1000 };//its position is by default off the screen
	m_explosion.setPosition(m_explosionPosition);
}

void Game::setUpSounds() {
	//loading the sound for when the player fires the laser
	m_fireBuffer.loadFromFile("ASSETS\\SOUNDS\\fire.wav");
	m_fireSound.setBuffer(m_fireBuffer);

	//loading the sound for when the explosion collides with the end point of the asteroid
	m_collisionBuffer.loadFromFile("ASSETS\\SOUNDS\\explosion.wav");
	m_collisionSound.setBuffer(m_collisionBuffer);
}
