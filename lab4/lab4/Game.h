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

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message

	sf::Text m_altitudeText;
	sf::RectangleShape m_ground;
	sf::RectangleShape m_powerbar;
	sf::RectangleShape m_playerBase;
	sf::CircleShape m_explosion;
	sf::Vector2f m_explosionPosition{ -1000,1000 };
	sf::VertexArray m_laser{ sf::Lines };
	sf::VertexArray m_asteroid{ sf::Lines };
	sf::Vector2f m_unitVector;
	sf::Vector2f m_laserEnd;
	sf::Vertex m_laserEndPoint;
	sf::Vector2f m_mouseClick;
	sf::Vector2f m_laserStart = sf::Vector2f{ 400, 420 };
	sf::Vertex m_laserStartPoint{ m_laserStart, sf::Color::Black }; // start point of line
	sf::Vector2f m_currentAsteroidEnd;
	sf::Vertex m_asteroidEndCurrent{ m_currentAsteroidEnd,sf::Color::Black };
	sf::Vector2f m_asteroidEndPoint = sf::Vector2f{ 0.0f, 0.0f };
	sf::Vertex m_asteroidEnd{ m_asteroidEndPoint, sf::Color::Black };
	sf::Vector2f asteroidStartPoint;
	sf::Vector2f m_asteroidStartPoint;
	sf::Vertex m_asteroidStart;
	float m_laserLength = 0.0f;
	float m_currentLaserLength = 0.0f;
	int m_score = 0;
	double m_explosionRadius = 5;
	float m_velocityLaser = 5.0f;
	float m_velocityAsteroid = 2.5f;
	bool m_exitGame; // control exiting game
	bool m_updateLaser = false;
	bool m_exploding = false;
	bool m_setUpAsteroid = true;
	bool m_moveAsteroid = false;
	bool m_gameOver = false;
};

#endif // !GAME

