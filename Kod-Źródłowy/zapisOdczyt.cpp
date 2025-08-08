#include "header.hpp"

void Odczyt(std::atomic_uint64_t & kasa, std::vector <std::vector <Pracownik>> & kopalnie)
{
	std::fstream plik;
	plik.open("save.txt", std::ios::in | std::ios::binary);
	if(!plik.good())
	{
		std::cout << "Nieudalo sie wczytac savea, nacisnij dowolny przycisk aby kontynuowac";
		_getch();
		return;
	}
	uint16_t ileFabryk = 0;
	uint16_t ilePracownikow = 0;
	uint64_t tempKasa = 0;
	plik >> tempKasa >> ileFabryk;
	kasa = tempKasa;
	while(ileFabryk --)
	{
		kopalnie.emplace_back();
		plik >> ilePracownikow;
		uint16_t poziomIlosciWydobywanychZloz;
		uint16_t poziomPredkosciKopaniaWMilisekundach;
		uint64_t kosztyUlepszen; 
		std::string imieDlaPracownika;
		while(ilePracownikow --)	//Dodanie kopalni z ka¿dym pracownikiem jaki w niej jest.
		{
			plik >> poziomIlosciWydobywanychZloz >> poziomPredkosciKopaniaWMilisekundach;
			plik >> kosztyUlepszen >> imieDlaPracownika;
			kopalnie.back().emplace_back(kosztyUlepszen, imieDlaPracownika,
			poziomIlosciWydobywanychZloz, poziomPredkosciKopaniaWMilisekundach);
		}
	}
	plik.close();
}

void Zapis(std::atomic_uint64_t & kasa, std::vector <std::vector <Pracownik>> & kopalnie)
{
	std::fstream plik;
	plik.open("save.txt", std::ios::out);
	if(!plik.good())
	{
		std::cout << "Nieudalo sie zapisac gry, nacisnij dowolny przycisk aby kontynuowac";
		_getch();
		return;
	}
	plik << kasa << ' ' << kopalnie.size() << '\n';	//Iloœæ pieniêdzy oraz kopalni.
	for(const std::vector <Pracownik> & kopalnia : kopalnie)
	{
		plik << kopalnia.size() << '\n';	//Iloœæ pracowników w pojedyñczej kopalni.
		for(size_t indeks = 0; indeks < kopalnia.size(); indeks ++)
		{
			//Poziom ilosci wydobywanych z³ó¿ oraz poziom prêdkoœci kopania w milisekundach.
			plik << static_cast <short> (kopalnia[indeks].ulepszenia[0].first) << ' ';
			plik << static_cast <short> (kopalnia[indeks].ulepszenia[1].first) << ' ';
			//Koszty ulepszeñ oraz imie danego pracownika.
			plik << kopalnia[indeks].ulepszenia[0].second.second << ' ' << kopalnia[indeks].imie << '\n';
		}
	}
	plik.close();
	std::cout << "Zapisano ^^" << '\n';
}