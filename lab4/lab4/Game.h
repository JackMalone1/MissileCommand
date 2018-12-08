// author Jack Malone
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>
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

	void setupFontAndText();
	void setupSprite();
	void setUpScene();
	void moveLaser();
	void drawExplosion();
	void createAsteroid();
	void moveAsteroid();
	void checkCollisions();
	void animatePowerBar();
	void waitToMakeAsteroid();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message

	sf::Text m_altitudeText;
	sf::Text m_scoreText;
	sf::Text m_gameOverText;
	sf::Text m_amountOfAsteroidsDestroyed;

	sf::RectangleShape m_ground;
	sf::RectangleShape m_powerbar;
	sf::RectangleShape m_playerBase;

	sf::CircleShape m_explosion;
	sf::Vector2f m_explosionPosition{ -1000,1000 };

	sf::Sprite m_groundSprite;
	sf::Sprite m_playerBaseSprite;
	sf::Sprite m_onScreenArea;
	sf::Sprite m_explosionSprite;

	sf::Texture m_backgroundTexture;
	sf::Texture m_playerBaseTexture;
	sf::Texture m_groundTexture;
	sf::Texture m_explosionTexture;

	sf::VertexArray m_laser{ sf::Lines };
	sf::VertexArray m_asteroid{ sf::Lines };

	sf::Vector2f m_unitVector;
	sf::Vector2f m_laserEnd;
	sf::Vertex m_laserEndPoint;
	sf::Vector2f m_mouseClick;
	sf::Vector2f m_laserStart = sf::Vector2f{ 400, 420 };
	sf::Vertex m_laserStartPoint{ m_laserStart, sf::Color::Yellow }; // start point of line

	sf::Vector2f m_currentAsteroidEnd;
	sf::Vertex m_asteroidEndCurrent{ m_currentAsteroidEnd,sf::Color::Red };
	sf::Vector2f m_asteroidEndPoint = sf::Vector2f{ 0.0f, 0.0f };
	sf::Vertex m_asteroidEnd{ m_asteroidEndPoint, sf::Color::Red };
	sf::Vector2f asteroidStartPoint;
	sf::Vector2f m_asteroidStartPoint;
	sf::Vertex m_asteroidStart;

	float m_powerbarWidth = 0;
	sf::Vector2f powerbarSize{ m_powerbarWidth, 50.0f };

	enum PowerBar{increasing,decreasing};//used to store whether to increase the size of the powerbar or to decrease it
	PowerBar m_powerbarState = increasing;//the power bar starts off as increasing at the start of the game

	enum GameState{classic,extraFeatures};//keeps track of whether or not it is using extra features or not after the player presses T
	GameState m_gameState = classic;//defaults to classic game when the game is loaded

	float m_maxAltitude = 0.0f;
	float m_laserLength = 0.0f;
	float m_currentLaserLength = 0.0f;

	int m_score = 0;
	int m_asteroidsDestroyed = 0;

	int m_waitToMakeAsteroid = 0;
	int m_timeToWait = 0;

	
	double m_explosionRadius = 5;
	float m_velocityLaser = 5.0f;
	float m_velocityAsteroid = 2.0f;

	bool m_exitGame; // control exiting game
	bool m_updateLaser = false;
	bool m_exploding = false;
	bool m_setUpAsteroid = false;
	bool m_moveAsteroid = false;
	bool m_gameOver = false;
};

#endif // !GAME

