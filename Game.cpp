#include "pch.h"
#include "Game.h"
#include "Board.h"
#include "Player.h"
#include <Windows.h>
#include <time.h> 
#include <stdlib.h> 
#include <iostream>
using namespace std;

Game::Game()
{
	gameOver = false;
	playerFled = false;
}

Game::~Game() 
{
	delete board;
	delete player;
	delete ui;
}

void Game::Init()
{
	/*
	otetaan systeemin aika ja seedataan sillä random number generator
	jotta saadaan eri numerot eri pelikerroilla
	*/
	srand((unsigned int)time(0));
	board->Init();
}

void Game::Update() 
{	
	//päivitetään pelin näkymä
	ui->DrawUI(board, player);
	ui->PrintInstructions();
	//odotetaan pelaajan näppäinpainallusta ennenkuin lopetetaan päivitys
	PlayerInput();

	//laitetaan pieni viive ennen uuden päivityksen alkua
	ui->Delay(100);
}

void Game::PlayerInput()
{
	bool waitInput = true;

	//looppi odottaa pelaajalta yhtä näppäinpainallusta
	while (waitInput)
	{
		//w - liikutaan ylöspäin
		if (GetKeyState(0x57) & 0x8000)
		{
			waitInput = false;
			MovePlayer("up");
		}

		//a - liikutaan vasemmalle
		if (GetKeyState(0x41) & 0x8000)
		{
			waitInput = false;
			MovePlayer("left");
		}

		//s - liikutaan alas
		if (GetKeyState(0x53) & 0x8000)
		{
			waitInput = false;
			MovePlayer("down");
		}

		//d - liikutaan oikealle
		if (GetKeyState(0x44) & 0x8000)
		{
			waitInput = false;
			MovePlayer("right");
		}

		//h - cast heal
		if (GetKeyState(0x48) & 0x8000)
		{
			waitInput = false;
			if (player->CheckIfEnoughMana(player->GetHealingCost()))
			{
				player->Heal();
				system("pause");
			}
		}

		//x - lopetetaan peli
		if (GetKeyState(0x58) & 0x8000)
		{
			waitInput = false;
			gameOver = true;
		}
	}
}

void Game::MovePlayer(string pDirection)
{
	//tarkistetaan onko pelaajan sijainnissa monsteri
	if (board->CheckDestination(pDirection) == 'M')
	{
		Battle* battle = new Battle();
		/*
		ajetaan taistelu, voitetusta taistelusta palautuu true,
		jos taistelussa ei voitettu, tarkistetaan onko pelaaja hengissä.
		Jos ei niin lopetetaan peli, jos pelaaja pakeni niin taistelua ei voitettu
		mutta peli jatkuu
		*/
		if (!battle->CarryOutBattle(board, ui, player))
		{
			if (player->CheckIfCharacterIsDead())
			{
				gameOver = true;
			}
			else
			{
				playerFled = true;
			}
		}
		delete battle;
	}
	//päivitetään pelaajan sijainti jos peli on vielä käynnissä
	//jos pelaaja pakeni taistelusta ei liikuta, eli jätetään sijainnissa oleva monsteri paikalleen
	if (!gameOver && !playerFled)
	{
		//jokaisella siirtymisellä annetaan pelaajalle 1 manapiste (negatiivinen arvo)
		player->DeductMana(-1);
		//jos sijainnissa on X eli exit, niin siirrytään uuteen kerrokseen
		if (board->CheckDestination(pDirection) == 'X')
		{
			board->Init();
		}
		else
		{
			board->UpdatePlayerPosition(pDirection);
		}		
	}	
	//muutetaan tämä falseksi että voidaan taas liikkua
	playerFled = false;	
}

bool Game::IsGameRunning()
{
	if (gameOver)
	{
		return false;
	}
	return true;
}