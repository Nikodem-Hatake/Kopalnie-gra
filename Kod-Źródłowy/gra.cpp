#include "header.hpp"

void UtworzElementyGraficzneGry();
void ZamykanieGry();
void UruchomFunkcjeDlaKliknietegoPrzyciskuGra(uint8_t && wJakiPrzyciskKliknieto);

void Gra()
{
	Gracz::czyTrwaGra = true;
	Gracz::DodajKopalnieWPrzypadkuBrakuSavea();
	Gracz::DodajWatkiNaPoczatkuGry();
	//Uruchamianie funkcji kopania dla ka¿dego pracownika z ka¿dej kopalni jako oddzielny w¹tek.
	if(Gracz::czyAktywnyAutoSave)
	{
		Gracz::AktywujAutoSave();
	}
	UtworzElementyGraficzneGry();
	{
		std::jthread wyswietlanieKasyWTle(GraficznyInterfejsUzytkownika::AktualizujInformacjeOIlosciPieniedzy);
		while(GraficznyInterfejsUzytkownika::okno.isOpen() && Gracz::czyTrwaGra)	//G³ówna pêtla gry.
		{
			UruchomFunkcjeDlaKliknietegoPrzyciskuGra(GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna());
		}
	}
	ZamykanieGry();
}

void UtworzElementyGraficzneGry()
{
	GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu
	("Ilosc kasy: " + Gracz::ZwrocIloscKasyJakoString(), {40.f, 24.f}, sf::Color(0, 240, 0));
	GraficznyInterfejsUzytkownika::UtworzPrzyciski(4);
	{
		std::array <std::string, 4> tekstyDlaObiektowTekstu
		{
			"Wejdz do kopalni",
			"Kup nowa kopalnie\nKoszt: " + std::to_string(Gracz::ZwrocKosztKupnaNowejKopalni()),
			"Kop samemu",
			"Wyjdz"
		};
		GraficznyInterfejsUzytkownika::UtworzObiektyTekstow(4, tekstyDlaObiektowTekstu.data());
	}
	GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(2, 80.f);
}

void UruchomFunkcjeDlaKliknietegoPrzyciskuGra(uint8_t && wJakiPrzyciskKliknieto)
{
	switch(wJakiPrzyciskKliknieto)
	{
		case 1:
		{
			std::string jakaKopalnia = "";
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, "Wprowadz tutaj\nnumer kopalni\nod 1 do "
			+ std::to_string(Gracz::ZwrocOstatniNumerKopalni()) + "\ni nacisnin enter:");
			GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 160.f);
			GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(2, 80.f);
			uint8_t jakieZdarzenie = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
			while(jakieZdarzenie > 13 && GraficznyInterfejsUzytkownika::okno.isOpen())
			{
				//Sprawdzanie czy kod ascii z wciœniêtego klawisza to cyfra lub znak nowej linii.
				if((jakieZdarzenie == 48 && !jakaKopalnia.empty())
				|| (jakieZdarzenie >= 49 && jakieZdarzenie <= 57))
				{
					jakaKopalnia += jakieZdarzenie;
					GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, jakaKopalnia);
				}
				jakieZdarzenie = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
			}
			if(jakieZdarzenie == 0)	//Sprawdzenie czy zamkniêto okno.
			{
				Gracz::czyTrwaGra = false;
			}
			else
			{
				Gracz::wybranyNumerKopalni = ZamienStringaNaLiczbe(jakaKopalnia);
				if(Gracz::wybranyNumerKopalni > 0 && Gracz::wybranyNumerKopalni <= Gracz::ZwrocOstatniNumerKopalni())
				{
					GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 40.f);
					GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(2, 40.f);
					ObslugaKopalni();
					UtworzElementyGraficzneGry();
				}
				else
				{
					GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, "Wejdz do kopalni");
				}
			}
			GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 40.f);
			break;
		}
		case 2:
		{
			Gracz::KupKopalnie();
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu
			(2, "Kup nowa kopalnie\nKoszt: " + std::to_string(Gracz::ZwrocKosztKupnaNowejKopalni()));
			break;
		}
		case 3:
		{
			KopanieSamemu();
			UtworzElementyGraficzneGry();
			break;
		}
		case 4:
		{
			Gracz::czyTrwaGra = false;
			break;
		}
	}
}

void ZamykanieGry()
{
	if(GraficznyInterfejsUzytkownika::okno.isOpen())
	{
		GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu
		("Trwa zapisywanie...", {25.f, 24.f}, sf::Color(0, 0, 0));
	}
	Gracz::OczyscWatki();
	if(Gracz::czyAktywnyAutoSave)	//Zamkniêcie autoSavea.
	{
		Gracz::SprawdzCzyAutoSaveDzialal();
	}
	else	//Je¿eli autosave jest wy³¹czony, zapisz poprzez u¿ycie tej funkjci.
	{
		Zapis();
	}
	GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(2, 40.f);	//Przywrócenie poprzedniej wysokoœci.
}