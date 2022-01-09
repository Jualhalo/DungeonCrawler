#include "pch.h"
#include "Battle.h"
#include <Windows.h>
#include <Time.h>

Battle::Battle()
{
	battleOver = false;
	playerWon = false;
}
Battle::~Battle(){}

bool Battle::CarryOutBattle(Board* board, UI* ui, Player* player)
{
	srand((unsigned int)time(0));

	//taistelua ennen luodaan monsteri jonka pelaaja kohtaa
	int mLvl = CalculateMonsterLevel(board);
	Character* enemy = GenerateMonster(mLvl);

	//alustetaan vihollisen statit
	enemy->InitStats();

	//printataan vihollisen tiedot
	cout << "You encountered:" << endl;
	ui->PrintEnemy(enemy);

	//selvitetään järjestys jossa vuorot pelataan taistelussa
	bool firstAttack = false;
	if (DecideFirstAttack(enemy, player))
	{
		firstAttack = true;
	}
	else
	{
		firstAttack = false;
	}

	BattleLoop(enemy, firstAttack, ui, board, player);

	if (playerWon)
	{
		return true;
	}
	else
	{
		return false;
	}
	//taistelun päätyttyä deletoidaan enemy olio
	delete enemy;
}

Character* Battle::GenerateMonster(int pLvl)
{
	//randomoidaan monsterin tyyppi ja asetetaan attribuutit
	//monsterin tyypin mukaan
	if ((rand() % 100 + 1) <= 5)
	{
		Character* ogre = new Character("Ogre", pLvl, ogre_str, ogre_agi, ogre_end, ogre_mag, ogre_expYield);
		return ogre;
	}
	else if ((rand() % 100 + 1) <= 15)
	{
		Character* orc = new Character("Orc", pLvl, orc_str, orc_agi, orc_end, orc_mag, orc_expYield);
		return orc;
	}
	else if ((rand() % 100 + 1) <= 30)
	{
		Character* skeleton = new Character("Zombie", pLvl, zombie_str, zombie_agi, zombie_end, zombie_mag, zombie_expYield);
		return skeleton;
	}
	else if ((rand() % 100 + 1) <= 45)
	{
		Character* skeleton = new Character("Slime", pLvl, slime_str, slime_agi, slime_end, slime_mag, slime_expYield);
		return skeleton;
	}
	else if ((rand() % 100 + 1) <= 65)
	{
		Character* skeleton = new Character("Skeleton", pLvl, skele_str, skele_agi, skele_end, skele_mag, skele_expYield);
		return skeleton;
	}
	else
	{
		Character* goblin = new Character("Goblin", pLvl, 2, 6, 3, 1, 25);
		return goblin;
	}
}

int Battle::CalculateMonsterLevel(Board* board)
{
	//asetetaan monsterin leveli kerroksen mukaan, eka kerros lvl on aina 1
	//ykköskerrosta ylemmäs mentäessä lvl kerrotaan viidellä joka kerros
	int mLvl = 0;
	if (board->GetFloor() > 1)
	{
		mLvl = (board->GetFloor() - 1) * 5;
	}
	else
	{
		mLvl = 1;
	}
	return mLvl;
}

bool Battle::DecideFirstAttack(Character* enemy, Player* player)
{
	/*
	jos pelaajan agility stat on suurempi kuin vihollisen,
	pelaaja saa vuoron ennen vihollista
	jos agility stat on sama, arvotaan kumpi saa ensimmäisen
	vuoron
	*/
	if (player->GetAgility() > enemy->GetAgility())
	{
		cout << "You get the first strike." << endl;
		return true;
	}
	else if (player->GetAgility() == enemy->GetAgility())
	{
		if ((rand() % 100 + 1) <= 50)
		{
			cout << "You get the first strike." << endl;
			return true;
		}
		else
		{
			cout << enemy->GetName() << " gets the first strike." << endl;
			return false;
		}
	}
	else
	{
		cout << enemy->GetName() << " gets the first strike." << endl;
		return false;
	}
}

void Battle::BattleLoop(Character* enemy, bool firstAttack, UI* ui, Board* board, Player* player)
{
	while (!battleOver)
	{
		//Pelaajan ja vihollisen vuorot pelataan, riippuen kumpi sai ensimmäisen vuoron
		//ennen toisen vuoroa ja lopuksi tarkistetaan ollaanko vielä hengissä
		if (firstAttack)
		{
			PlayerAction(enemy, ui, board, player);
			if (!battleOver && CheckEnemyAliveStatus(enemy, player) && CheckPlayerAliveStatus(player))
			{
				EnemyAction(enemy, player);
				CheckPlayerAliveStatus(player);

				/*vihollinen voi kuolla ajalliseen damageen (burning)
				vuoronsa päättyessä, sen vuoksi tarkistetaan onko se vielä hengissä
				*/
				CheckEnemyAliveStatus(enemy, player);
			}
		}
		else
		{
			EnemyAction(enemy, player);
			if (!battleOver && CheckEnemyAliveStatus(enemy, player) && CheckPlayerAliveStatus(player))
			{
				PlayerAction(enemy, ui, board, player);
				CheckEnemyAliveStatus(enemy, player);
			}
		}
		//päivitetään pelinäkymä roundin lopussa
		ui->DrawUI(board, player);
		ui->PrintEnemy(enemy);
	}	
}

void Battle::EnemyAction(Character* enemy, Player* player)
{
	//lyödään pelaajaa
	enemy->Attack(player);
	//tarkistetaan onko vihollisella damage over time- laskureita
	enemy->CheckDamageOverTime();

	//lopuksi odotetaan pelaajalta näppäinpainallusta (jotta pelaaja kerkeää nähdä tulostukset)
	system("pause");
}

void Battle::PlayerAction(Character* enemy, UI* ui, Board* board, Player* player)
{
	//pelaajan vuorolla pelaaja saa valikon mahdollisista toiminnoista joita pelaaja voi tehdä	
	//ui->DrawUI(board, player);
	//ui->PrintEnemy(enemy);
	ui->BattleQuery();
	bool waitChoiceInput = true;

	while (waitChoiceInput)
	{
		//1 - Attack
		if (GetKeyState(0x31) & 0x8000)
		{
			waitChoiceInput = false;
			player->Attack(enemy);
		}

		//2 - Cast		
		if (GetKeyState(0x32) & 0x8000)
		{
			//pelaajalle annetaan valikko spelleistä joita pelaaja voi castaa
			bool waitSpellChoiceInput = true;
			bool cancelSpellChoice = false;

			//tähän viive, jotta edellisen valikon valinta ei tulisi vahingossa myös tähän valikkoon
			ui->Delay(200);

			ui->SpellQuery();

			while (waitSpellChoiceInput)
			{
				/*
				ennen castaamista tarkistetaan onko pelaajalla tarpeeksi manaa
				jos mana ei riitä, annetaan pelaajalle viesti ja palataan takaisin
				edelliseen valikkoon
				*/

				//1 - Fireball
				if (GetKeyState(0x31) & 0x8000)
				{
					if (player->CheckIfEnoughMana(player->GetFireballCost()))
					{
						player->Fireball(enemy);
						waitSpellChoiceInput = false;
					}
					else
					{
						waitSpellChoiceInput = false;
						cancelSpellChoice = true;
					}
				}
				//2 - Heal
				if (GetKeyState(0x32) & 0x8000)
				{
					if (player->CheckIfEnoughMana(player->GetHealingCost()))
					{
						player->Heal();
						waitSpellChoiceInput = false;
					}
					else
					{
						waitSpellChoiceInput = false;
						cancelSpellChoice = true;
					}
				}

				//x - Cancel
				if (GetKeyState(0x58) & 0x8000)
				{
					waitSpellChoiceInput = false;
					cancelSpellChoice = true;
				}
			}
			if (!cancelSpellChoice)
			{
				waitChoiceInput = false;
			}
			else
			{
				/*jos spellin valinta on peruttu, printataan ui uudelleen
				(eli näkymässä palataan edelliseen valikkoon)
				*/
				ui->DrawUI(board, player);
				ui->PrintEnemy(enemy);
				ui->BattleQuery();
			}
		}
		//3 - Flee
		if (GetKeyState(0x33) & 0x8000)
		{
			/*
			tarkistetaan voiko pelaaja paeta taistelusta,
			jos onnistuu niin taistelu loppuu,
			jos ei niin pelaajan vuoro päättyy ja taistelu jatkuu
			*/
			if (player->CalculateFleeSuccess(enemy))
			{
				cout << "You successfully fled from the battle." << endl;
				battleOver = true;
				playerWon = false;
			}
			else
			{
				cout << "You failed fleeing from the battle." << endl;
			}
			waitChoiceInput = false;
		}
	}

	//lopuksi odotetaan pelaajalta näppäinpainallusta
	system("pause");
}


bool Battle::CheckEnemyAliveStatus(Character* enemy, Player* player)
{
	//tarkistetaan onko vihollinen kuollut
	if (enemy->CheckIfCharacterIsDead())
	{
		cout << "You killed " << enemy->GetName() << endl;

		//päätetään taistelu
		battleOver = true;
		playerWon = true;
		/*
		annetaan pelaajalle expaa ja tarkistetaan riittikö se levelin nostoon
		harvinaisissa tilanteissa vihollinen kuolee ajalliseen damageen tapettuaan pelaajan
		joten tarkistetaan että pelaaja on hengissä ennenkuin vihollisesta annetaaan expaa
		*/
		if (!player->CheckIfCharacterIsDead())
		{
			player->GainExp(enemy->CalculateReceivedExp());
			player->CheckLevelUp();
		}
		system("pause");
		return false;
	}
	else
	{
		return true;
	}
}
bool Battle::CheckPlayerAliveStatus(Player* player)
{
	//tarkistetaan onko pelaaja kuollut
	if (player->CheckIfCharacterIsDead())
	{
		cout << "You died!" << endl;

		//päätetään taistelu
		battleOver = true;
		playerWon = false;

		system("pause");
		return false;
	}
	else
	{
		return true;
	}
}