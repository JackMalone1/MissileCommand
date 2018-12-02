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
	void updateLaser(sf::Vector2f t_laserEndPoint);
	void moveLaser(sf::Vector2f t_unitVector, sf::Vector2f t_laserEndPoint, sf::Vector2f t_mouseClick);

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Text m_altitudeText;
	sf::Sprite m_logoSprite; // sprite used for sfml logo
	sf::RectangleShape m_ground;
	sf::RectangleShape m_powerbar;
	sf::RectangleShape m_playerBase;
	sf::VertexArray m_laser{sf::Lines};
	sf::VertexArray m_asteroid{sf::Lines};
	//sf::Vector2f m_laserEnd;
	sf::Vector2f m_unitVector;
	sf::Vector2f m_laserEnd;
	sf::Vertex m_laserEndPoint;
	sf::Vector2f m_mouseClick;
	sf::Vector2f m_laserStart = sf::Vector2f{ 400, 420 };
	sf::Vertex m_laserStartPoint{ m_laserStart, sf::Color::Black }; // start point of line
	enum m_laserState{firing,exploding,notFiring};
	m_laserState laserState = notFiring;
	int velocity = 5;
	bool m_exitGame; // control exiting game
	bool m_updateLaser = false;
};

#endif // !GAME

