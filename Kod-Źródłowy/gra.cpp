#include "header.hpp"

void Gra()
{
	std::cout << "Wczytywanie..." << '\n';
	std::atomic_bool czyTrwaGra = true;
	std::atomic_uint64_t kasa = 0;
	std::list <std::thread> watki;
	std::mutex blokadaDostepuDoKasy;	//U¿ywane przy dostêpie do kasy, ¿eby by³a ona prawid³owa.
	std::list <std::list <Pracownik>> kopalnie;
	Odczyt(kasa, kopalnie);
	if(kopalnie.empty())	//Je¿eli w savie nic nie by³o b¹dŸ nie móg³ byæ otworzony.
	{
		kopalnie.emplace_back();
		kopalnie.back().emplace_back(static_cast <uint64_t> (10), "Pracownik0",
		static_cast <uint16_t> (1), static_cast <uint16_t> (1));
	}
	uint64_t numerKopalni = 0;
	//Uruchamianie funkcji kopania dla ka¿dego pracownika z ka¿dej kopalni jako oddzielny w¹tek.
	for(std::list <Pracownik> & kopalnia : kopalnie)
	{
		numerKopalni ++;
		for(Pracownik & pracownik : kopalnia)
		{
			//U¿ycie lambdy ¿eby wywo³aæ metodê jako w¹tek.
			watki.emplace_back([&]
			{
				pracownik.Kopanie(czyTrwaGra, kasa, numerKopalni * 2, blokadaDostepuDoKasy);
			});
		}
	}
	system("cls");
	while(czyTrwaGra)	//G³ówna pêtla gry.
	{
		LobbyGry(kasa);
		char wybor = _getch();
		system("cls");
		switch(wybor)
		{
			case '1':
			{
				//Wyœwietlenie dostêpnych kopalni oraz wybor jednej z nich poprzez cyfrê.
				std::cout << "Dostepne kopalnie: " << '\n';
				uint16_t numerKopalni = 0;
				for(auto iterator = kopalnie.begin(); iterator != kopalnie.end(); iterator ++)
				{
					std::cout << "Kopalnia " << numerKopalni << ". Ilosc pracownikow: ";
					std::cout << iterator -> size() << '\n';
					numerKopalni ++;
				}
				std::cout << "Wybierz ktora kopalnie chcesz odwiedzic poprzez wcisniecie cyfry: ";
				char wyborKopalni = _getch();
				system("cls");
				if(wyborKopalni >= 48 && wyborKopalni <= kopalnie.size() + 47)
				{
					wyborKopalni -= 48;
					//Przesuniêcie iteratora na wybran¹ kopalnie.
					auto iterator = kopalnie.begin();
					for(char i = 0; i < wyborKopalni; i ++)
					{
						iterator ++;
					}
					ObslugaKopalni(* iterator, wyborKopalni,
					czyTrwaGra, kasa, blokadaDostepuDoKasy, watki);
				}
				else
				{
					std::cout << "Wybrano zla kopalnie" << '\n';
				}
				break;
			}
			case '2':
			{
				//Kupno kopalni.
				if(kopalnie.size() == 10)
				{
					std::cout << "Niemozesz juz kupic wiecej kopalni" << '\n';
				}
				else
				{
					uint64_t kosztKopalni = (kopalnie.size() + 1) * 25000;
					std::cout << "Koszt nowej kopalni: " << kosztKopalni << '\n';
					std::cout << "Czy chcesz kupic nowa kopalnie?" << '\n';
					std::cout << "1. Tak" << '\n' << "2. Nie" << '\n' << "Dokonaj wyboru: ";
					char wyborKupna = _getch();
					system("cls");
					if(wyborKupna == '1' && kasa >= kosztKopalni)
					{
						std::unique_lock <std::mutex> blokada(blokadaDostepuDoKasy);
						kasa -= kosztKopalni;
						blokada.unlock();
						kopalnie.emplace_back();
						std::cout << "Zakup udany ^^" << '\n';
					}
					else if(wyborKupna == '1' && kasa < kosztKopalni)
					{
						std::cout << "Niestac cie na nowa kopalnie" << '\n';
					}
				}
				break;
			}
			case '3':
			{
				KopanieSamemu(kasa, blokadaDostepuDoKasy);
				system("cls");
				break;
			}
			case '5':
			{
				czyTrwaGra = false;
				break;
			}
			default:
			{
				if(wybor != '4')
				{
					std::cout << "Dokonano zlego wyboru" << '\n';
				}
			}
		}
	}
	std::cout << "Trwa zapisywanie..." << '\n';
	//Zamykanie ka¿dego w¹tku.
	for(auto iterator = watki.begin(); iterator != watki.end(); iterator ++)
	{
		iterator -> join();
	}
	Zapis(kasa, kopalnie);
}