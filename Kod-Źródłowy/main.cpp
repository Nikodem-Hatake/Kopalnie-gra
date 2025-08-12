#include "header.hpp"

int main()
{
	bool czyAutoSave = true;
	bool czyKoniecGry = false;
	bool czyWczytanoZPliku = false;
	char wybor = 0;
	std::atomic_uint64_t kasa = 0;
	std::list <std::list <Pracownik>> kopalnie;
	//Wczytywanie danych do gry w tle.
	std::future <bool> wczytywanieGry = std::async(std::launch::async, Odczyt, &kasa, &kopalnie);
	while(!czyKoniecGry)
	{
		Lobby(czyAutoSave);
		wybor = _getch();
		system("cls");
		switch(wybor)
		{
			case '1':
			{
				std::cout << "Wczytywanie..." << '\n';
				if(!czyWczytanoZPliku)
				{
					if(bool czyUdaloSieWczytacZPliku = wczytywanieGry.get(); !czyUdaloSieWczytacZPliku)
					{
						std::cout << "Nieudalo sie wczytac danych z pliku,";
						std::cout << "nacisnij dowolny przycisk aby kontynuowac" << '\n';
						_getch();
					}
					czyWczytanoZPliku = true;	//Ustawienie na true, ¿eby nieodczytywaæ danych z pliku drugi raz.
				}
				Gra(czyAutoSave, kopalnie, kasa);
				break;
			}
			case '2':
			{
				if(czyAutoSave)
				{
					czyAutoSave = false;
				}
				else
				{
					czyAutoSave = true;
				}
				break;
			}
			case '3':
			{
				czyKoniecGry = true;
				break;
			}
			default:
			{
				std::cout << "Dokonano zlego wyboru" << '\n';
				break;
			}
		}
	}
}