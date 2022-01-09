#include "pch.h"
#include "Board.h"
#include "Player.h"
#include <time.h> 
#include <iostream>
#include <vector>
using namespace std;

Board::Board()
{
	width = 0;
	height = 0;
	spawnCounter = 0;
	floor = 0;
}

Board::Board(int pWidth, int pHeight)
{
	width = pWidth;
	height = pHeight;
}

Board::~Board() {}

void Board::Init()
{
	srand((unsigned int)time(0));
	floor++;

	//täytetään leveli tyhjillä ruuduilla, eli '.'
	world.assign(width * height, '.');

	//asetetaan levelille seinät
	PlaceWalls();
	PlaceExit();
	//asetetaan pelaaja leveliin, vakio sijainti on keskellä ylhäällä
	playerPosition = width + width / 2;
	world[playerPosition] = 'P';
	//asetetaan monsterit leveliin
	PlaceMonsters();
	spawnCounter = 0;
}

void Board::PlaceWalls()
{
	//asetetaan levelin ulkoreunoille seinät
	for (unsigned i = 0; i < world.size(); i++)
	{
		if (i <= width || i >= world.size() - width - 1)
		{
			world[i] = '#';
		}
		else if ((i % width) == 0)
		{
			world[i] = '#';
			world[i - 1] = '#';
		}
	}
}

void Board::PlaceExit()
{
	//arvotaan exitin sijainti ja asetetaan se leveliin
	bool exitPlaced = false;
	int exitPos = 0;
	while (!exitPlaced)
	{
		exitPos = rand() % (world.size() - width - 1) + (width + 1);
		if (world[exitPos] != '#' && world[exitPos] != 'P')
		{
			world[exitPos] = 'X';
			exitPlaced = true;
		}
	}
}

void Board::PlaceMonsters()
{
	//asetetaan monstereiden sijainnit randomilla
	for (unsigned i = 0; i < world.size(); i++)
	{
		if (world[i] != '#' && world[i] != 'P' && world[i] != 'X')
		{
			if ((rand() % 100 + 1) <= 2)
			{
				world[i] = 'M';
			}
		}
	}
}

void Board::Draw()
{
	//päivitetään pelaajan sijainti
	world[playerPosition] = 'P';
	//piirretään leveli
	int widthIndex = 0;
	for (unsigned i = 0; i < height; i++)
	{		
		int widthIncrement = 0;
		for (unsigned j = widthIndex; j < width + widthIndex; j++)
		{
			cout << world[j];
			widthIncrement++;
		}
		widthIndex += widthIncrement;
		cout << endl;
	}
}

void Board::UpdatePlayerPosition(string direction)
{
	//muutetaan pelaajan entinen sijainti tyhjäksi
	//jokaisella onnistuneella liikkeellä nostetaan spawncounteria
	world[playerPosition] = '.';

	if (direction == "up")
	{	
		if (CheckIfMoveIsPossible(playerPosition - width))
		{
			playerPosition = playerPosition - width;
			IncreaseSpawnCounter();
		}
	}
	if (direction == "left")
	{
		if (CheckIfMoveIsPossible(playerPosition - 1))
		{
			playerPosition--;
			IncreaseSpawnCounter();
		}
	}
	if (direction == "down")
	{
		if (CheckIfMoveIsPossible(playerPosition + width))
		{
			playerPosition = playerPosition + width;
			IncreaseSpawnCounter();
		}
	}
	if (direction == "right")
	{
		if (CheckIfMoveIsPossible(playerPosition + 1))
		{
			playerPosition++;
			IncreaseSpawnCounter();
		}
	}
}

void Board::IncreaseSpawnCounter()
{
	/*
	nostetaan spawncounteria.
	kun counter on yli viiden, spawnataan uusi monsteri peliin,
	mutta vain jos pelissä on alle 10 monsteria.
	*/
	spawnCounter++;
	if (CountMonsters() < 10)
	{
		if (spawnCounter > 5)
		{
			SpawnMonster();
		}
	}
}

int Board::CountMonsters()
{
	//laskee pelissä olevien monstereiden määrän
	int numOfMonsters{ 0 };
	for (unsigned i = { 0 }; i < world.size(); i++)
	{
		if (world[i] == 'M')
		{
			numOfMonsters++;
		}
	}
	return numOfMonsters;
}

void Board::SpawnMonster()
{
	//asetetaan yksi monsteri peliin randomilla 
	bool monsterPlaced = false;
	while (!monsterPlaced)
	{
		for (unsigned i = 0; i < world.size(); i++)
		{
			if (world[i] != '#' && world[i] != 'P' && world[i] != 'X' && world[i] != 'M')
			{
				if ((rand() % 200 + 1) <= 1)
				{
					world[i] = 'M';
					monsterPlaced = true;
					break;
				}
			}				
		}
	}
	spawnCounter = 0;
}

bool Board::CheckIfMoveIsPossible(int pDestination)
{
	if (world[pDestination] == '#')
	{
		return false;	
	}
	return true;
}

char Board::CheckDestination(string direction)
{
	//palauttaa liikkumisen sijainnissa olevan merkin
	if (direction == "up")
	{
		return world[playerPosition - width];
	}
	if (direction == "left")
	{
		return world[playerPosition - 1];
	}
	if (direction == "down")
	{
		return world[playerPosition + width];
	}
	if (direction == "right")
	{
		return world[playerPosition + 1];
	}
	return '.';
}

int Board::GetFloor()
{
	return floor;
}