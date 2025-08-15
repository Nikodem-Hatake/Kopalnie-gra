#include "header.hpp"

bool Odczyt(std::atomic_uint64_t * kasa, std::list <std::list <Pracownik>> * kopalnie)
{
	std::ifstream plik("save.txt", std::ios::in | std::ios::binary);
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
		while(ilePracownikow --)	//Dodanie kopalni z ka¿dym pracownikiem jaki w niej jest.
		{
			plik >> poziomIlosciWydobywanychZloz >> poziomPredkosciKopaniaWMilisekundach;
			plik >> kosztyUlepszen >> imieDlaPracownika;
			kopalnie -> back().emplace_back(kosztyUlepszen, imieDlaPracownika,
			poziomIlosciWydobywanychZloz, poziomPredkosciKopaniaWMilisekundach);
		}
	}
	return true;
}

void Zapis(std::atomic_uint64_t & kasa, std::list <std::list <Pracownik>> & kopalnie)
{
	std::ofstream plik("save.txt", std::ios::out);
	if(!plik.good())
	{
		std::cout << "Nieudalo sie zapisac gry, sprawdz czy plik save.txt jest w tej samej sciezce co gra" << '\n';
		std::cout << "nacisnij dowolny przycisk aby kontynuowac ";
		_getch();
		return;
	}
	plik << kasa << ' ' << kopalnie.size() << '\n';	//Iloœæ pieniêdzy oraz kopalni.
	for(const std::list <Pracownik> & kopalnia : kopalnie)
	{
		plik << kopalnia.size() << '\n';	//Iloœæ pracowników w pojedyñczej kopalni.
		for(auto iterator = kopalnia.begin(); iterator != kopalnia.end(); iterator ++)
		{
			//Poziom ilosci wydobywanych z³ó¿ oraz poziom prêdkoœci kopania w milisekundach.
			plik << static_cast <short> (iterator -> ulepszenia[0].first) << ' ';
			plik << static_cast <short> (iterator -> ulepszenia[1].first) << ' ';
			//Koszty ulepszeñ oraz imie danego pracownika.
			plik << iterator -> ulepszenia[0].second.second << ' ' << iterator -> imie << '\n';
		}
	}
	std::cout << "Zapisano ^^" << '\n';
}

bool ZapisAutoSave(std::atomic_uint64_t * kasa, std::list <std::list <Pracownik>> * kopalnie, 
std::mutex * blokadaDostepuDoKopalni, std::atomic_bool * czyTrwaGra,
std::condition_variable * powiadomienieODostepieDoKopalni, std::atomic_bool * powiadomienie)
{
	while(* czyTrwaGra)
	{
		//Petla trwaj¹ca minute i sprawdzaj¹ca co sekundê, czy nadal mo¿e wykonaæ autoSave
		//(sprawdzanie co sekunde, ¿eby niezatrzymywaæ gry na d³ugo w procesie wychodzenia z niej).
		for(uint16_t ileRazyPrzeczekac = 0; * czyTrwaGra && ileRazyPrzeczekac < 60; ileRazyPrzeczekac ++)
		{
			if(* czyTrwaGra)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		std::ofstream plik("save.txt", std::ios::out);

		if(plik.good())
		{
			std::unique_lock <std::mutex> blokada(* blokadaDostepuDoKopalni);
			if(!(* powiadomienie))	//Czekanie na dostêp do kopalni.
			{
				powiadomienieODostepieDoKopalni -> wait(blokada);
			}
			plik << * kasa << ' ' << kopalnie -> size() << '\n';	//Iloœæ pieniêdzy oraz kopalni.
			for(const std::list <Pracownik> & kopalnia : * kopalnie)
			{
				plik << kopalnia.size() << '\n';	//Iloœæ pracowników w pojedyñczej kopalni.
				for(auto iterator = kopalnia.begin(); iterator != kopalnia.end(); iterator ++)
				{
					//Poziom ilosci wydobywanych z³ó¿ oraz poziom prêdkoœci kopania w milisekundach.
					plik << static_cast <short> (iterator -> ulepszenia[0].first) << ' ';
					plik << static_cast <short> (iterator -> ulepszenia[1].first) << ' ';
					//Koszty ulepszeñ oraz imie danego pracownika.
					plik << iterator -> ulepszenia[0].second.second << ' ' << iterator -> imie << '\n';
				}
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}