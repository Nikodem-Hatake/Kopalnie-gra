#include "header.hpp"

bool Odczyt()
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
	Gracz::kasa = tempKasa;
	Gracz::kopalnie.reserve(ileFabryk);

	for(uint16_t i = 1; i <= ileFabryk; i ++)
	{
		Gracz::kopalnie.emplace_back(i);
		plik >> ilePracownikow;
		uint16_t poziomIlosciWydobywanychZloz;
		uint16_t poziomPredkosciKopaniaWMilisekundach;
		uint64_t kosztyUlepszen; 
		uint64_t ileZarobil;
		std::string imieDlaPracownika;
		while(ilePracownikow --)	//Dodanie kopalni z ka¿dym pracownikiem jaki w niej jest.
		{
			plik >> poziomIlosciWydobywanychZloz >> poziomPredkosciKopaniaWMilisekundach;
			plik >> kosztyUlepszen >> imieDlaPracownika >> ileZarobil;
			Gracz::kopalnie.back().pracownicy.emplace_back(kosztyUlepszen, imieDlaPracownika,
			poziomIlosciWydobywanychZloz, poziomPredkosciKopaniaWMilisekundach, ileZarobil);
		}
	}
	return true;
}

void Zapis()
{
	std::ofstream plik("save.txt", std::ios::out);
	if(!plik.good())
	{
		if(GraficznyInterfejsUzytkownika::okno.isOpen())
		{
			GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu
			("Nieudalo sie zapisac", {40.f, 24.f}, sf::Color(240, 0, 0));
		}
		return;
	}
	plik << Gracz::kasa << ' ' << Gracz::kopalnie.size() << '\n';	//Iloœæ pieniêdzy oraz kopalni.
	for(const Kopalnia & kopalnia : Gracz::kopalnie)
	{
		plik << kopalnia.pracownicy.size() << '\n';	//Iloœæ pracowników w pojedyñczej kopalni.
		for(auto iterator = kopalnia.pracownicy.begin(); iterator != kopalnia.pracownicy.end(); iterator ++)
		{
			//Poziom ilosci wydobywanych z³ó¿ oraz poziom prêdkoœci kopania w milisekundach.
			plik << iterator -> ulepszenia[0].first << ' ';
			plik << iterator -> ulepszenia[1].first << ' ';
			//Koszty ulepszeñ oraz imie danego pracownika.
			plik << iterator -> ulepszenia[0].second.second << ' ' << iterator -> imie << ' ';
			plik << iterator -> ileZarobil << '\n';
		}
	}
	if(GraficznyInterfejsUzytkownika::okno.isOpen())
	{
		GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu
		("Zapisano ^^", {120.f, 24.f}, sf::Color(0, 240, 0));
	}
}

bool ZapisAutoSave()
{
	while(Gracz::czyTrwaGra)
	{
		//Petla trwaj¹ca minute i sprawdzaj¹ca co sekundê, czy nadal mo¿e wykonaæ autoSave
		//(sprawdzanie co sekunde, ¿eby niezatrzymywaæ gry na d³ugo w procesie wychodzenia z niej).
		for(uint16_t ileRazyPrzeczekac = 0; Gracz::czyTrwaGra && ileRazyPrzeczekac < 60; ileRazyPrzeczekac ++)
		{
			if(Gracz::czyTrwaGra)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}
		std::ofstream plik("save.txt", std::ios::out);

		if(plik.good())
		{
			plik << Gracz::kasa << ' ' << Gracz::kopalnie.size() << '\n';	//Iloœæ pieniêdzy oraz kopalni.
			for(Kopalnia & kopalnia : Gracz::kopalnie)
			{
				std::unique_lock <std::mutex> blokada(Kopalnia::blokadaDostepuDoKopalni);
				if(!Kopalnia::powiadomienie)	//Czekanie na dostêp do kopalni.
				{
					Kopalnia::powiadomienieODostepieDoKopalni.wait(blokada);
				}
				plik << kopalnia.pracownicy.size() << '\n';	//Iloœæ pracowników w pojedyñczej kopalni.
				for(auto iterator = kopalnia.pracownicy.begin(); iterator != kopalnia.pracownicy.end(); iterator ++)
				{
					if(!Kopalnia::powiadomienie)	//Czekanie na dostêp do kopalni.
					{
						Kopalnia::powiadomienieODostepieDoKopalni.wait(blokada);
					}
					//Poziom ilosci wydobywanych z³ó¿ oraz poziom prêdkoœci kopania w milisekundach.
					plik << iterator -> ulepszenia[0].first << ' ';
					plik << iterator -> ulepszenia[1].first << ' ';
					//Koszty ulepszeñ oraz imie danego pracownika.
					plik << iterator -> ulepszenia[0].second.second << ' ' << iterator -> imie << ' ';
					plik << iterator -> ileZarobil << '\n';
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