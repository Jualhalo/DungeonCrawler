#pragma once
#include "Board.h"
#include "Character.h"
#include "Player.h"
#include <iostream>
using namespace std;

class UI
{
private:
public:
	UI();
	~UI();
	void DrawUI(Board* board, Player* player);
	void PrintInstructions();
	void PrintEnemy(Character* enemy);
	void BattleQuery();
	void SpellQuery();
	void Delay(int timer);
};