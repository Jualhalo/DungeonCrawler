#include "pch.h"
#include "UI.h"
#include <time.h> 
using namespace std;

UI::UI() {} 
UI::~UI() {}

void UI::DrawUI(Board* board, Player* player)
{
	//tyhjentää peli-ikkunan
	cout << "\x1B[2J\x1B[H";
	cout << "Floor: " << board->GetFloor() << " ";
	//tulostaa pelaajan statsit
	player->PrintStats();
	//piirretään leveli
	board->Draw();
}

void UI::PrintInstructions()
{
	cout << "Move with WASD" << endl;
	cout << "Press H to heal" << endl;
	cout << "Press X to quit" << endl;
}

void UI::PrintEnemy(Character* enemy)
{
	//tulostetaan vihollisen nimi ja statsit
	cout << "----------------" << endl;
	enemy->PrintStats();
	cout << "----------------" << endl;
}

void UI::BattleQuery()
{
	//pelaajan valinnat taistelussa
	cout << "Choose action:" << endl;
	cout << "1) Attack" << endl;
	cout << "2) Cast" << endl;
	cout << "3) Flee" << endl;
}

void UI::SpellQuery()
{
	//pelaajan spell valikko
	cout << "----------------" << endl;
	cout << "Choose spell:" << endl;
	cout << "1) Fireball" << endl;
	cout << "2) Heal" << endl;
	cout << "Press x to cancel" << endl;
}

void UI::Delay(int timer)
{
	//tekee parametriin annetun arvon mittaisen viiveen (ms)
	clock_t time = clock();
	while (clock() < time + timer);
}