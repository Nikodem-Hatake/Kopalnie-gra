#include "header.hpp"

void ObslugaPracownika(std::atomic_uint64_t & kasa, std::mutex & blokadaDostepuDoKasy, Pracownik & pracownik)
{
	bool czyWyjscZObslugiPracownika = false;
	while(!czyWyjscZObslugiPracownika)
	{
		LobbyObslugiPracownika(kasa);
		char wybor = _getch();
		system("cls");
		switch(wybor)
		{
			case '1':
			case '2':
			{
				wybor -= 49;
				pracownik.Ulepszenie(wybor, kasa, blokadaDostepuDoKasy);
				break;
			}
			case '3':
			{
				std::string noweImie;
				std::cout << "Wprowadz imie: ";
				std::cin >> noweImie;
				pracownik.ZmianaImienia(noweImie);
				break;
			}
			case '4':
			{
				pracownik.InformacjeOPracowniku();
				break;
			}
			case '6':
			{
				czyWyjscZObslugiPracownika = true;
				break;
			}
			default:
			{
				if(wybor != '5')
				{
					std::cout << "Dokonano zlego wyboru" << '\n';
				}
				break;
			}
		}
	}
}

void ObslugaKopalni(std::list <Pracownik> & kopalnia, char & numerKopalni, std::atomic_bool & czyTrwaGra,
std::atomic_uint64_t & kasa, std::mutex & blokadaDostepuDoKasy, std::list <std::thread> & watki)
{
	bool czyWyjscZObslugiKopalni = false;
	while(!czyWyjscZObslugiKopalni)
	{
		LobbyObslugiKopalni(numerKopalni, kopalnia.size(), kasa);
		char wybor = _getch();
		system("cls");
		switch(wybor)
		{
			case '1':
			{
				if(kopalnia.size() == 0)
				{
					std::cout << "Niemasz jeszcze zadnego pracownika w tej kopalni" << '\n';
				}
				else
				{
					std::cout << "Dostepni pracownicy: " << '\n';
					for(size_t indeks = 0; indeks < kopalnia.size(); indeks ++)
					{
						std::cout << "Pracownik o numerze " << indeks << '\n';
					}
					std::cout << "Wybierz pracownika: ";
					char wyborPracownika = _getch();
					system("cls");
					if(wyborPracownika >= 48 && wyborPracownika <= 47 + kopalnia.size())
					{
						wyborPracownika -= 48;
						//Przesuniêcie iteratora na wybranego pracownika.
						auto iterator = kopalnia.begin();
						while(wyborPracownika --)
						{
							iterator ++;
						}
						ObslugaPracownika(kasa, blokadaDostepuDoKasy, * iterator);
					}
					else
					{
						std::cout << "Wybrano zlego pracownika" << '\n';
					}
				}
				break;
			}
			case '2':
			{
				if(kopalnia.size() == 10)
				{
					std::cout << "Niemozesz kupic juz nastepnego pracownika (osiagnieto limit)" << '\n';
				}
				else
				{
					uint64_t kosztNowegoPracownika = (numerKopalni + 1) * (kopalnia.size() + 1) * 1000;
					std::cout << "Ilosc pieniedzy: " << kasa << '\n';
					std::cout << "Koszt nowego pracownika: " << kosztNowegoPracownika << '\n';
					std::cout << "Czy chcesz kupic nowego pracownika?" << '\n';
					std::cout << "1. Tak" << '\n' << "2. Nie" << '\n' << "Dokonaj wyboru: ";
					char wyborKupna = _getch();
					system("cls");
					if(wyborKupna == '1' && kasa >= kosztNowegoPracownika)
					{
						std::unique_lock <std::mutex> blokada(blokadaDostepuDoKasy);
						kasa -= kosztNowegoPracownika;
						//Dodanie nowego pracownika do kopalni oraz uruchomienie w¹tku dla niego.
						std::string imieDlaPracownika = "Pracownik" + static_cast <char> (48 + kopalnia.size());
						kopalnia.emplace_back(static_cast <uint64_t> (10 * (numerKopalni + 1)), imieDlaPracownika,
						static_cast <uint16_t> (1), static_cast <uint16_t> (1));
						watki.emplace_back([&]
						{
							kopalnia.back().Kopanie(czyTrwaGra, kasa, (numerKopalni + 1) * 2, blokadaDostepuDoKasy);
						});
						blokada.unlock();
						std::cout << "Zakup udany ^^" << '\n';

					}
					else if(wyborKupna == '1' && kasa < kosztNowegoPracownika)
					{
						std::cout << "Niestac cie na nowego pracownika" << '\n';
					}
				}
				break;
			}
			case '4':
			{
				czyWyjscZObslugiKopalni = true;
				break;
			}
			default:
			{
				if(wybor != '3')
				{
					std::cout << "Dokonano zlego wyboru" << '\n';
				}
				break;
			}
		}
	}
}