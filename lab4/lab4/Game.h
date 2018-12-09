// author Jack Malone
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
const float m_PI = 3.14159265359f;
class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processMouseEvents(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	void setupFontAndText();//sets up all of the text and fonts
	void setUpSounds();//sets up the sounds used in the game
	void setupSprite();//sets up the sprites used in the game
	void setUpScene();//sets up the orignal shapes used in the game
	void moveLaser();//moves the laser after the user clicks somewhere on the screen
	void drawExplosion();//draws the explosion after the line has finished drawing
	void createAsteroid();//sets up the asteroid
	void moveAsteroid();//moves the laser once it has been set up
	void checkCollisions();//checks for collisions between the asteroid and the explosion
	void animatePowerBar();//changes the width of the power bar depending on whether or not it is expanding or decreasing
	void waitToMakeAsteroid();//makes the program wait before making a new asteroid

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message

	sf::Text m_altitudeText;//displays altitude so the player no the power bar controls the altitude of the laser
	sf::Text m_scoreText;//displays the player's score
	sf::Text m_gameOverText;//shows that the game is over
	sf::Text m_amountOfAsteroidsDestroyed;//displays how many asteroids the player was able to destroy

	sf::RectangleShape m_ground;//used as the ground
	sf::RectangleShape m_powerbar;//used as the power bar to show how far up they can shoot on the screen
	sf::RectangleShape m_playerBase;//used as the orignal player base

	sf::SoundBuffer m_fireBuffer;//sound buffer for the firing sound
	sf::SoundBuffer m_collisionBuffer;//sound buffer for the explosion sound

	sf::Sound m_fireSound;//sound for when the player fires a laser
	sf::Sound m_collisionSound;//sound for when the explosion and the asteroid intersect

	sf::CircleShape m_explosion;//used as the base explosion
	sf::Vector2f m_explosionPosition{ -1000,1000 };

	/*
	all of the orignal objects with textures for the mode with extra features
	they each have their own texture which is set up at the start of the game
	*/
	sf::Sprite m_groundSprite;
	sf::Sprite m_playerBaseSprite;
	sf::Sprite m_onScreenArea;
	sf::Sprite m_explosionSprite;
	sf::Texture m_backgroundTexture;
	sf::Texture m_playerBaseTexture;
	sf::Texture m_groundTexture;
	sf::Texture m_explosionTexture;

	//vertex array for the laser and the asteroid
	sf::VertexArray m_laser{ sf::Lines };
	sf::VertexArray m_asteroid{ sf::Lines };

	/*
	unit vector to increment the line
	also has a start point, a current end point for continually increment where it's drawn to
	*/
	sf::Vector2f m_unitVector;
	sf::Vector2f m_laserEnd;
	sf::Vertex m_laserEndPoint;
	sf::Vector2f m_mouseClick;
	sf::Vector2f m_laserStart = sf::Vector2f{ 400, 420 };
	sf::Vertex m_laserStartPoint{ m_laserStart, sf::Color::Yellow }; // start point of line

	sf::Vector2f m_currentAsteroidEnd;//where the asteroid has currently reached
	sf::Vertex m_asteroidEndCurrent{ m_currentAsteroidEnd,sf::Color::Red };
	sf::Vector2f m_asteroidEndPoint = sf::Vector2f{ 0.0f, 0.0f };//randomly decides where the asteroid is going to 
	sf::Vertex m_asteroidEnd{ m_asteroidEndPoint, sf::Color::Red };
	sf::Vector2f asteroidStartPoint;//randomly decides wehre the asteroid starts
	sf::Vector2f m_asteroidStartPoint;
	sf::Vertex m_asteroidStart;

	float m_powerbarWidth = 0;//powerbar starts off with a width of 0
	sf::Vector2f powerbarSize{ m_powerbarWidth, 50.0f };

	enum PowerBar{increasing,decreasing};//used to store whether to increase the size of the powerbar or to decrease it
	PowerBar m_powerbarState = increasing;//the power bar starts off as increasing at the start of the game

	enum GameState{classic,extraFeatures};//keeps track of whether or not it is using extra features or not after the player presses T
	GameState m_gameState = classic;//defaults to classic game when the game is loaded

	float m_maxAltitude = 0.0f;// how far the laser can go up the screen
	float m_laserLength = 0.0f;//overall length of the laser
	float m_currentLaserLength = 0.0f;//current length of the laser to check if it has got to the mouse click or not

	int m_score = 0;//the score that the player has
	int m_asteroidsDestroyed = 0;//how many asteroids they destroyed

	int m_waitToMakeAsteroid = 0;//how much time it has currently waited
	int m_timeToWait = 0;//how much time it has to wait before making a new asteroid. it has a random value in a range of half a second to 2 and a half seconds

	
	double m_explosionRadius = 5;//minimum radius of the explosion
	float m_velocityLaser = 5.0f;//how fast the laser travels
	float m_velocityAsteroid = 2.0f;//how fast the asteroid travels

	bool m_exitGame; // control exiting game
	bool m_updateLaser = false;//true when moving the asteroid
	bool m_exploding = false;//true when there is an explosion
	bool m_setUpAsteroid = false;//true only when setting up the laser
	bool m_moveAsteroid = false;//true when moving the asteroid
	bool m_gameOver = false;//true once the player loses the game
};

#endif // !GAME

