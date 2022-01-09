#include "pch.h"
#include "Character.h"
#include <stdlib.h> 
#include <time.h> 
#include <iostream>
#include <string>
using namespace std;

Character::Character()
{
	name = "character";
	level = 1;
	strength = 5;
	agility = 5;
	endurance = 5;
	magic = 5;
	maxHealth = endurance * 10;
	health = maxHealth;
	maxMana = magic * 5;
	mana = maxMana;
	dotCounter = 0;
	expAmount = 0;
	dead = false;
}

Character::Character(string pName, int pLvl, int pStr, int pAgi, int pEnd, int pMag, int pExpAmount)
{
	name = pName;
	level = pLvl;
	strength = pStr;
	agility = pAgi;
	endurance = pEnd;
	magic = pMag;
	maxHealth = endurance * 10;
	health = maxHealth;
	maxMana = magic * 5;
	mana = maxMana;
	dotCounter = 0;
	expAmount = pExpAmount;
	dead = false;
}

Character::~Character() {}

void Character::TakeDamage(int damage)
{
	//vähennetään saadun damagen määrä healthista, 
	//jos parametrinä on negatiivinen arvo, healthia tulee lisää (healing)
	health -= damage;

	//jos mennään minimin ali tai maksimin yli, säädetään health rajalle
	if (health <= 0)
	{
		health = 0;
		//koska health on loppu, hahmo kuolee
		dead = true;
	}

	if (health > maxHealth)
	{
		health = maxHealth;
	}
}

void Character::IncreaseStats(string pStat)
{
	//parametrinä annetaan nostettavan statin nimi
	if (pStat == "Strength")
	{
		strength++;
	}
	if (pStat == "Agility")
	{
		agility++;
	}

	//asetetaan maksimit uudelleen END ja MAG nostettaessa
	if (pStat == "Endurance")
	{
		endurance++;
		maxHealth = endurance * 10;
	}
	if (pStat == "Magic")
	{
		magic++;
		maxMana = magic * 5;
	}
}

void Character::InitStats()
{	
	srand((unsigned int)time(0));
	//statit arvotaan hahmolle jokaista leveliä kohden, jos hahmon lvl on suurempi kuin 1
	if (level > 1)
	{ 
		for (int i = 0; i < level; i++)
		{
			if ((rand() % 4 + 1) == 1)
			{
				IncreaseStats("Strength");
			}
			else if ((rand() % 4 + 1) == 2)
			{
				IncreaseStats("Agility");
			}
			else if ((rand() % 4 + 1) == 3)
			{
				IncreaseStats("Endurance");
			}
			else
			{
				IncreaseStats("Magic");
			}
		}
	}

	//koska end ja mag ovat voineet nousta, nostetaan arvot uudelleen maksimiin
	health = maxHealth;
	mana = maxMana;
}

void Character::Attack(Character* target)
{
	//lasketaan hyökkäyksen tekemä damage, 
	//normihyökkäyksen damage = str * 1-5
	int damageDone = CalculateDamage(strength, attackMax, attackMin);
	cout << GetName() << " hits " << target->GetName() << " for " << damageDone << " damage." << endl;
	target->TakeDamage(damageDone);
}

int Character::CalculateDamage(int pStat, int pMax, int pMin)
{
	srand((unsigned int)time(0));
	//kerrotaan statti random luvulla joka määräytyy parametrien multiplier ja minimin mukaan
	int damage = pStat * (rand() % pMax + pMin);
	return damage;
}

void Character::DeductMana(int amount)
{
	//vähennetään manaa (tai nostetaan jos parametri on negatiivinen arvo)
	mana -= amount;
	//nostettaessa ei anneta mennä maksimin yli
	if (mana > maxMana)
	{
		mana = maxMana;
	}
}

void Character::PrintStats()
{
	cout << name << endl;
	cout << "LVL: " << level << " HP: " << health << "/" << maxHealth
		<< " MP: " << mana << "/" << maxMana
		<< " STR: " << strength << " AGI: " << agility
		<< " END: " << endurance << " MAG: " << magic << endl;
}

void Character::AddDamageOverTime(int counters, int damage)
{
	//otetaan damage over time - lähteen tekemä damage
	dotCounter += counters;
	if (dotCounter > 3)
	{
		dotCounter = 3;
	}
	if (dotCounter < 0)
	{
		dotCounter = 0;
	}
	dotDmg = damage / dotCounter;
}

void Character::CheckDamageOverTime()
{
	//jos on damage over time -countereita, tehdään damagea ja vähennetään yksi counter
	if (dotCounter > 0)
	{		
		TakeDamage(dotDmg);
		cout << GetName() << " receives " << dotDmg << " damage from burning." << endl;
		dotCounter--;
	}
}

int Character::CalculateReceivedExp()
{
	expAmount = expAmount * level;
	return expAmount;
}

string Character::GetName()
{
	return name;
}

int Character::GetAgility()
{
	return agility;
}

bool Character::CheckIfCharacterIsDead()
{
	if (dead)
	{
		return true;
	}
	else
	{
		return false;
	}
}