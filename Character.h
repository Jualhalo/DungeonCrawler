#pragma once
#include <string>
#include "Board.h"
using namespace std;

class Character
{
protected:
	string name;
	int level;
	int strength;
	int agility;
	int endurance;
	int magic;
	int maxHealth;
	int health;
	int maxMana;
	int mana;
	int dotCounter;
	int dotDmg;
	int expAmount;
	const int attackMax = 5;
	const int attackMin = 1;
	bool dead;

	int CalculateDamage(int pStat, int pMulti, int pMin);
public:
	Character();
	Character(string pName, int pLvl, int pStr, int pAgi, int pEnd, int pMag, int pExpAmount);
	~Character();
	void TakeDamage(int damage);
	void IncreaseStats(string pStat);
	void InitStats();
	void Attack(Character* target);
	void DeductMana(int amount);
	void PrintStats();
	void AddDamageOverTime(int counters, int damage);
	void CheckDamageOverTime();
	int CalculateReceivedExp();
	bool CheckIfCharacterIsDead();

	//getterit
	string GetName();
	int GetAgility();
};