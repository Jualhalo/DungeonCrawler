#pragma once
#include <vector>
using namespace std;

class Board
{
private:
	unsigned width;
	unsigned height;
	int playerPosition;
	int spawnCounter;
	int floor;
	vector<char> world;
	
public:
	Board();
	Board(int pWidth, int pHeight);
	~Board();
	void Init();
	void PlaceWalls();
	void PlaceExit();
	void PlaceMonsters();
	void Draw();
	void UpdatePlayerPosition(string direction);
	void IncreaseSpawnCounter();
	int CountMonsters();
	void SpawnMonster();
	bool CheckIfMoveIsPossible(int pDestination);
	char CheckDestination(string direction);

	int GetFloor();
};