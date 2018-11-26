// author Peter Lowe
#ifndef GAME
#define GAME

#include <SFML/Graphics.hpp>

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void update(sf::Time t_deltaTime);
	void render();

	void setupFontAndText();
	void setupSprite();
	void setUpScene();

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
	bool m_exitGame; // control exiting game

};

#endif // !GAME

