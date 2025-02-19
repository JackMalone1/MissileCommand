/// <summary>
/// Empty Project for SFML[2.5.0]
/// 
/// 
/// @author Jack Malone
/// @date November 2018
/// </summary>

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include "game.h"



/// <summary>
/// main enrtry point
/// </summary>
/// <returns>zero</returns>
int main()
{
#ifdef _DEBUG
	srand(27u);
#else
	srand(static_cast<unsigned int>(time(nullptr)));
#endif // _Debug
	Game game;
	game.run();
	return 0;
}