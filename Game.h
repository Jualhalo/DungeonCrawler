#pragma once
#include "Board.h"
#include "Character.h"
#include "Player.h"
#include "UI.h"
#include "Battle.h"
#include <iostream>
using namespace std;

class Game
{
private:
	bool gameOver;
	bool playerFled;

	Player *player = new Player();
	Board *board = new Board(40,20);
	UI *ui = new UI();

	void PlayerInput();
	void MovePlayer(string pDirection);
public:
	Game();
	~Game();
	void Init();
	void Update();
	bool IsGameRunning();	
};