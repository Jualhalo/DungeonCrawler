#include "pch.h"
#include "Game.h"
#include "Board.h"
#include "Player.h"
#include <iostream>
using namespace std;

int main()
{
	Game* game = new Game();

	game->Init();

	//ajetaan pelilooppia kunnes peli päättyy
	while (game->IsGameRunning())
	{
		game->Update();
	}
	delete game;
}
