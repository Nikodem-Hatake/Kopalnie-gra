#include "header.hpp"

bool CzyKliknietoWRude(bool & czyGraczKopie);

void KopanieSamemu()
{
	bool czyGraczKopie = true;
	auto ziarno = time(0);
	std::default_random_engine generatorLosowychLiczb(ziarno);
	std::uniform_int_distribution <short> zakres(1, 1000);
	std::uniform_int_distribution <short> zakresRudy(1, 100);
	std::uniform_real_distribution <float> losowaPozycjaRudy(1, 399);
	std::array <std::pair <std::string, uint64_t>, 5> rudy 
	{{
		{"Wegiel", 5},
		{"Zelazo", 10},
		{"Lapis", 15},
		{"Ametyst", 30},
		{"Diament", 50}
	}};
	GraficznyInterfejsUzytkownika::UtworzPrzyciski(2);
	{
		std::array <std::string, 1> tekstyDlaObiektowTekstu
		{
			"Kop samemu poprzez\nklikanie w rude.\nNacisnij ten \nprzycisk aby wyjsc"
		};
		GraficznyInterfejsUzytkownika::UtworzObiektyTekstow(1, tekstyDlaObiektowTekstu.data());
	}
	GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 160.f);
	GraficznyInterfejsUzytkownika::UtworzRudeZOstatniegoPrzycisku();
	bool czyKliknietoWRude = false;
	short czyWydobyto = 0;
	while(GraficznyInterfejsUzytkownika::okno.isOpen() && czyGraczKopie)
	{
		GraficznyInterfejsUzytkownika::UstawLosowaPozycjeRudy
		(losowaPozycjaRudy(generatorLosowychLiczb), losowaPozycjaRudy(generatorLosowychLiczb));
		czyWydobyto = zakres(generatorLosowychLiczb);
		czyKliknietoWRude = CzyKliknietoWRude(czyGraczKopie);
		//Wydobycie rudy (szansa 7.5%).
		if(czyWydobyto >= 925 && czyKliknietoWRude)
		{
			//Wylosowanie rudy jak¹ siê wydoby³o.
			short jakaRuda = zakresRudy(generatorLosowychLiczb);
			if(jakaRuda <= 30)	//30%.
			{
				jakaRuda = 0;
				GraficznyInterfejsUzytkownika::UstawKolorObiektuTekstu(1, sf::Color(20, 20, 20));
			}
			else if(jakaRuda <= 50)	//20%.
			{
				jakaRuda = 1;
				GraficznyInterfejsUzytkownika::UstawKolorObiektuTekstu(1, sf::Color(204, 204, 204));
			}
			else if(jakaRuda <= 70)	//20%.
			{
				jakaRuda = 2;
				GraficznyInterfejsUzytkownika::UstawKolorObiektuTekstu(1, sf::Color(0, 0, 230));
			}
			else if(jakaRuda <= 85)	//15%.
			{
				jakaRuda = 3;
				GraficznyInterfejsUzytkownika::UstawKolorObiektuTekstu(1, sf::Color(194, 153, 255));
			}
			else	//15%.
			{
				jakaRuda = 4;
				GraficznyInterfejsUzytkownika::UstawKolorObiektuTekstu(1, sf::Color(128, 223, 255));
			}
			Gracz::kasa += rudy[jakaRuda].second;
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu
			(1, "Wydobyto " + rudy[jakaRuda].first + "\nNacisnij ten \nprzycisk aby wyjsc");
		}
	}
	GraficznyInterfejsUzytkownika::PrzywrocOstatniPrzycisk();
	GraficznyInterfejsUzytkownika::UstawKolorObiektuTekstu(1, sf::Color(0, 0, 0));
	GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 40.f);
}

bool CzyKliknietoWRude(bool & czyGraczKopie)
{
	uint8_t wJakiPrzyciskKliknieto = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
	if(wJakiPrzyciskKliknieto == 1)	//Zamkniêcie okna/Funkcji kopania samemu.
	{
		czyGraczKopie = false;
		return false;
	}
	else if(wJakiPrzyciskKliknieto == 2)	//Klikniêcie w rudê.
	{
		return true;
	}
	return false;
}