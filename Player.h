#pragma once
#include "Character.h"

class Player : public Character
{
private:
	int magic;
	int experience;	
	int nextLvl;
	const int fireballCost = 8;
	const int healingCost = 6;
	const int fireballMax = 6;
	const int fireballMin = 2;

	void LevelUp();
	void LevelUpQuery();
public:
	Player();
	//Player(int pLvl, int pStr, int pAgi, int pEnd, int pPos);
	~Player();	
	void Attack(Character* target);
	void GainExp(int amount);
	void CheckLevelUp();

	void Fireball(Character* target);
	void Heal();
	int CalculateHeal();
	bool CalculateFleeSuccess(Character* enemy);
	bool CheckIfEnoughMana(int cost);
	void PrintStats();
	//getterit
	int GetFireballCost();
	int GetHealingCost();	
};