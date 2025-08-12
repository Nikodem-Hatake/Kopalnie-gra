#include "header.hpp"

void Gra(bool & czyAutoSave, std::list <std::list <Pracownik>> & kopalnie, std::atomic_uint64_t & kasa)
{
	std::cout << "Wczytywanie..." << '\n';
	std::atomic_bool czyTrwaGra = true;
	//U¿ywane do powiadomienia w¹tków o tym, ¿e mo¿na modyfikowaæ i odczytywaæ kopalnie.
	std::atomic_bool powiadomienie = true;
	std::condition_variable powiadomienieODostepieDoKopalni;	//U¿ywane do oczekiwania na dostêp do kopalni.
	std::future <bool> autoSaveWTle;	//Oddzielny w¹tek zapisuj¹cy grê co jakiœ czas w tle.
	std::list <std::jthread> watki;	//W¹tki pracowników
	std::mutex blokadaDostepuDoKopalni;
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
				pracownik.Kopanie(czyTrwaGra, kasa, numerKopalni * 2);
			});
		}
	}
	system("cls");
	if(czyAutoSave)
	{
		autoSaveWTle = std::async(std::launch::async, ZapisAutoSave, &kasa, &kopalnie, 
		&blokadaDostepuDoKopalni, &czyTrwaGra, &powiadomienieODostepieDoKopalni, &powiadomienie);
	}
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
				std::cout << "Wybierz kopalnie poprzez wpisanie numeru i nacisnij enter: ";
				uint64_t wyborKopalni = 0;
				std::string wyborKopalniInput;
				std::cin >> wyborKopalniInput;
				system("cls");
				if(czyPoprawnaLiczba(wyborKopalniInput, kopalnie.size(), wyborKopalni))
				{
					//Przesuniêcie iteratora na wybran¹ kopalnie.
					auto iterator = kopalnie.begin();
					for(uint64_t i = 0; i < wyborKopalni; i ++)
					{
						iterator ++;
					}
					ObslugaKopalni(* iterator, wyborKopalni, czyTrwaGra, kasa, watki,
					blokadaDostepuDoKopalni, powiadomienieODostepieDoKopalni, powiadomienie);
				}
				break;
			}
			case '2':
			{
				//Kupno kopalni.
				if(kopalnie.size() == 1000)
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
						std::cout << "Trwa kupowanie..." << '\n';
						std::unique_lock <std::mutex> blokada(blokadaDostepuDoKopalni);
						if(!powiadomienie)
						{
							powiadomienieODostepieDoKopalni.wait(blokada);
						}
						powiadomienie = false;
						kasa -= kosztKopalni;
						kopalnie.emplace_back();
						powiadomienie = true;
						powiadomienieODostepieDoKopalni.notify_one();
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
				KopanieSamemu(kasa);
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
	std::cout << "Trwa zapisywanie oraz zamykanie gry..." << '\n';
	if(czyAutoSave)	//Zamkniêcie autoSavea.
	{
		//Je¿eli autoSave dzia³a³, wyœwietlenie komunikatu.
		if(bool czyZapisano = autoSaveWTle.get(); czyZapisano)
		{
			std::cout << "Zapisano ^^" << '\n';
		}
		else
		{
			std::cout << "Nieudalo sie zapisac gry, sprawdz czy plik save.txt";
			std::cout << "jest w tej samej sciezce co gra." << '\n';
		}
	}
	else	//Je¿eli autosave jest wy³¹czony, zapisz poprzez u¿ycie tej funkjci.
	{
		Zapis(kasa, kopalnie);
	}
}