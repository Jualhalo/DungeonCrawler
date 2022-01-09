#pragma once
#include "Board.h"
#include "Character.h"
#include "Player.h"
#include "UI.h"
#include <iostream>
using namespace std;

class Battle
{
private:
	bool battleOver;
	bool playerWon;

	//Monsterityyppien statsit:
	//Goblin
	const int goblin_str = 2;
	const int goblin_agi = 6;
	const int goblin_end = 3;
	const int goblin_mag = 2;
	const int goblin_expYield = 50;
	//Orc
	const int orc_str = 4;
	const int orc_agi = 4;
	const int orc_end = 4;
	const int orc_mag = 1;
	const int orc_expYield = 80;
	//Skeleton
	const int skele_str = 4;
	const int skele_agi = 5;
	const int skele_end = 2;
	const int skele_mag = 3;
	const int skele_expYield = 60;
	//Ogre
	const int ogre_str = 5;
	const int ogre_agi = 2;
	const int ogre_end = 6;
	const int ogre_mag = 1;
	const int ogre_expYield = 120;
	//Slime
	const int slime_str = 1;
	const int slime_agi = 1;
	const int slime_end = 6;
	const int slime_mag = 2;
	const int slime_expYield = 40;
	//Zombie
	const int zombie_str = 3;
	const int zombie_agi = 1;
	const int zombie_end = 5;
	const int zombie_mag = 1;
	const int zombie_expYield = 70;

	Character* GenerateMonster(int pLvl);
	int CalculateMonsterLevel(Board* board);
	bool DecideFirstAttack(Character* enemy, Player* player);
	void BattleLoop(Character* enemy, bool firstAttack, UI* ui, Board* board, Player* player);
	void PlayerAction(Character* enemy, UI* ui, Board* board, Player* player);
	void EnemyAction(Character* enemy, Player* player);
	bool CheckEnemyAliveStatus(Character* enemy, Player* player);
	bool CheckPlayerAliveStatus(Player* player);
public:
	Battle();
	~Battle();
	bool CarryOutBattle(Board* board, UI* ui, Player* player);
};
