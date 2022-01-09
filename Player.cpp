#include "pch.h"
#include "Player.h"
#include <Windows.h>
#include <time.h> 
#include <iostream>
#include <string>
using namespace std;

Player::Player()
{
	name = "you";
	level = 1;
	strength = 5;
	agility = 5;
	endurance = 5;
	magic = 5;
	maxHealth = endurance * 10;
	health = maxHealth;
	experience = 0;
	nextLvl = 50;
}

Player::~Player() {}

void Player::Attack(Character* target)
{
	//pelaajan Attack metodi jossa on erilaiset tulosteet
	cout << "You hit " << target->GetName() << "." << endl;
	int damageDone = CalculateDamage(strength, 5, 1);
	cout << target->GetName() << " receives " << damageDone << " damage." << endl;
	target->TakeDamage(damageDone);
}

void Player::GainExp(int amount)
{
	experience += amount;
	cout << "You received " << amount << " experience." << endl;
}

void Player::CheckLevelUp() 
{	
	bool tryToLvlUp = true;	
	/*tarkistetaan riittääkö pelaajan exp seuraavaan leveliin
	loopataan niin kauan kunnes pelaajalla ei riitä exp leveloimiseen
	Tämä sitä varten jos pelaaja saa tarpeeksi expaa useampaan kuin yhteen
	leveliin kerralla.
	*/
	while (tryToLvlUp)
	{
		if (experience >= nextLvl)
		{
			LevelUp();
		}
		else
		{
			tryToLvlUp = false;
		}		
		clock_t time =  clock();
		while (clock() < time + 200);
	}				
}

void Player::LevelUp()
{
	//nostetaan pelaajan leveliä ja suurennetaan seuraavalle levelille tarvittavan expan määrää
	level++;
	nextLvl = nextLvl * 2.5;
	cout << "You have reached level " << level << "." << endl;
	LevelUpQuery();
}

void Player::LevelUpQuery()
{
	//annetaan pelaajalle lista valittavista stateista joita nostaa
	bool waitChoiceInput = true;
	cout << "----------------" << endl;
	cout << "Choose a stat to increase:" << endl;
	cout << "1) Strength" << endl;
	cout << "2) Agility" << endl;
	cout << "3) Endurance" << endl;
	cout << "4) Magic" << endl;

	while (waitChoiceInput)
	{
		//1
		if (GetKeyState(0x31) & 0x8000)
		{
			IncreaseStats("Strength");
			cout << "You gained 1 point of Strength" << endl;
			waitChoiceInput = false;
		}
		//2
		if (GetKeyState(0x32) & 0x8000)
		{
			IncreaseStats("Agility");
			cout << "You gained 1 point of Agility" << endl;
			waitChoiceInput = false;
		}
		//3
		if (GetKeyState(0x33) & 0x8000)
		{
			IncreaseStats("Endurance");
			cout << "You gained 1 point of Endurance" << endl;
			waitChoiceInput = false;
		}
		//4
		if (GetKeyState(0x34) & 0x8000)
		{
			IncreaseStats("Magic");
			cout << "You gained 1 point of Magic" << endl;
			waitChoiceInput = false;
		}
	}
}

void Player::Fireball(Character* target)
{
	//vähennetään pelaajan manaa
	DeductMana(fireballCost);

	cout << "You cast Fireball." << endl;
	int damageDone = CalculateDamage(magic, fireballMax, fireballMin);
	cout << "Fireball hits " << target->GetName() << " for " << damageDone << " damage." << endl;
	target->TakeDamage(damageDone);

	//fireball tekee ajallista damagea viholliselle kolmen seuraavan vuoron ajan, 
	//ajallisesti saatu damage jaetaan kolmella fireballin tekemästä damagesta
	target->AddDamageOverTime(3, damageDone);
	cout << target->GetName() << " is burning." << endl;
}

void Player::Heal()
{
	//vähennetään pelaajan manaa
	DeductMana(healingCost);
	int healing = CalculateHeal();
	cout << "You cast Heal." << endl;
	//annetaan TakeDamage funktiolle negatiivinen arvo, jotta hp tulee lisää vähennyksen sijaan
	TakeDamage(-healing);
	cout << "You healed for " << healing << " HP." << endl;
}

int Player::CalculateHeal()
{
	//lasketaan palautettavan healthin määrä, 75% kokonaismäärästä
	int healing = maxHealth / 4 * 3;
	return healing;
}

bool Player::CalculateFleeSuccess(Character* enemy)
{
	//jos pelaajan agility on enemmän kuin vihollisen, annetaan pelaajalle 50% mahdollisuus paeta
	//muussa tapauksessa mahdollisuus on 25%
	srand((unsigned int)time(0));
	int escapeChance = rand() % 100 + 1;
	if (agility > enemy->GetAgility())
	{
		if (escapeChance > 50)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (escapeChance > 75)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Player::CheckIfEnoughMana(int cost)
{
	if (cost > mana)
	{
		cout << "Not enough mana." << endl;
		system("pause");
		return false;
	}
	else
	{
		return true;
	}
}

void Player::PrintStats()
{
	int nextLvlDisplay = nextLvl - experience;
	if (nextLvlDisplay < 0)
		nextLvlDisplay = 0;

	cout << "LVL: " << level << " HP: " << health << "/" << maxHealth
		<< " MP: " << mana << "/" << maxMana
		<< " STR: " << strength << " AGI: " << agility
		<< " END: " << endurance << " MAG: " << magic
		<< " NextLVL:" << nextLvlDisplay << endl;
}

int Player::GetFireballCost()
{
	return fireballCost;
}

int Player::GetHealingCost()
{
	return healingCost;
}



