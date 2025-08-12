#include "header.hpp"

bool Odczyt(std::atomic_uint64_t * kasa, std::list <std::list <Pracownik>> * kopalnie)
{
	std::fstream plik;
	plik.open("save.txt", std::ios::in | std::ios::binary);
	if(!plik.good())
	{
		return false;
	}
	uint16_t ileFabryk = 0;
	uint16_t ilePracownikow = 0;
	uint64_t tempKasa = 0;
	plik >> tempKasa >> ileFabryk;
	if(ileFabryk > 100)
	{
		ileFabryk = 100;
	}
	* kasa = tempKasa;
	while(ileFabryk --)
	{
		kopalnie -> emplace_back();
		plik >> ilePracownikow;
		if(ilePracownikow > 100)
		{
			ilePracownikow = 100;
		}
		uint16_t poziomIlosciWydobywanychZloz;
		uint16_t poziomPredkosciKopaniaWMilisekundach;
		uint64_t kosztyUlepszen; 
		std::string imieDlaPracownika;
		while(ilePracownikow --)	//Dodanie kopalni z ka�dym pracownikiem jaki w niej jest.
		{
			plik >> poziomIlosciWydobywanychZloz >> poziomPredkosciKopaniaWMilisekundach;
			plik >> kosztyUlepszen >> imieDlaPracownika;
			kopalnie -> back().emplace_back(kosztyUlepszen, imieDlaPracownika,
			poziomIlosciWydobywanychZloz, poziomPredkosciKopaniaWMilisekundach);
		}
	}
	plik.close();
	return true;
}

void Zapis(std::atomic_uint64_t & kasa, std::list <std::list <Pracownik>> & kopalnie)
{
	std::fstream plik;
	plik.open("save.txt", std::ios::out);
	if(!plik.good())
	{
		std::cout << "Nieudalo sie zapisac gry, sprawdz czy plik save.txt jest w tej samej sciezce co gra" << '\n';
		std::cout << "nacisnij dowolny przycisk aby kontynuowac ";
		_getch();
		return;
	}
	plik << kasa << ' ' << kopalnie.size() << '\n';	//Ilo�� pieni�dzy oraz kopalni.
	for(const std::list <Pracownik> & kopalnia : kopalnie)
	{
		plik << kopalnia.size() << '\n';	//Ilo�� pracownik�w w pojedy�czej kopalni.
		for(auto iterator = kopalnia.begin(); iterator != kopalnia.end(); iterator ++)
		{
			//Poziom ilosci wydobywanych z�� oraz poziom pr�dko�ci kopania w milisekundach.
			plik << static_cast <short> (iterator -> ulepszenia[0].first) << ' ';
			plik << static_cast <short> (iterator -> ulepszenia[1].first) << ' ';
			//Koszty ulepsze� oraz imie danego pracownika.
			plik << iterator -> ulepszenia[0].second.second << ' ' << iterator -> imie << '\n';
		}
	}
	plik.close();
	std::cout << "Zapisano ^^" << '\n';
}

bool ZapisAutoSave(std::atomic_uint64_t * kasa, std::list <std::list <Pracownik>> * kopalnie, 
std::mutex * blokadaDostepuDoKopalni, std::atomic_bool * czyTrwaGra,
std::condition_variable * powiadomienieODostepieDoKopalni, std::atomic_bool * powiadomienie)
{
	std::fstream plik;
	plik.open("save.txt", std::ios::out);
	if(!plik.good())
	{
		std::cout << "Funkcja auto save niedziala ze wzgledu na brak dostepu do pliku save.txt" << '\n';
		return false;
	}
	plik.close();
	while(* czyTrwaGra)
	{
		//Petla trwaj�ca minute i sprawdzaj�ca co sekund�, czy nadal mo�e wykona� autoSave
		//(sprawdzanie co sekunde, �eby niezatrzymywa� gry na d�ugo w procesie wychodzenia z niej).
		for(uint16_t ileRazyPrzeczekac = 0; * czyTrwaGra && ileRazyPrzeczekac < 60; ileRazyPrzeczekac ++)
		{
			if(* czyTrwaGra)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		plik.open("save.txt", std::ios::out);
		if(plik.good())
		{
			std::unique_lock <std::mutex> blokada(* blokadaDostepuDoKopalni);
			if(!(* powiadomienie))	//Czekanie na dost�p do kopalni.
			{
				powiadomienieODostepieDoKopalni -> wait(blokada);
			}
			* powiadomienie = false;
			plik << * kasa << ' ' << kopalnie -> size() << '\n';	//Ilo�� pieni�dzy oraz kopalni.
			for(const std::list <Pracownik> & kopalnia : * kopalnie)
			{
				plik << kopalnia.size() << '\n';	//Ilo�� pracownik�w w pojedy�czej kopalni.
				for(auto iterator = kopalnia.begin(); iterator != kopalnia.end(); iterator ++)
				{
					//Poziom ilosci wydobywanych z�� oraz poziom pr�dko�ci kopania w milisekundach.
					plik << static_cast <short> (iterator -> ulepszenia[0].first) << ' ';
					plik << static_cast <short> (iterator -> ulepszenia[1].first) << ' ';
					//Koszty ulepsze� oraz imie danego pracownika.
					plik << iterator -> ulepszenia[0].second.second << ' ' << iterator -> imie << '\n';
				}
			}
			* powiadomienie = true;
			powiadomienieODostepieDoKopalni -> notify_one();	//Powiadomienie innego w�tku o dost�pie do kopalni.
			plik.close();
		}
		else
		{
			return false;
		}
	}
	return true;
}