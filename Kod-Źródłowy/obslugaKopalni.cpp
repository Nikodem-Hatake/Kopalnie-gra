#include "header.hpp"

void UruchomFunkcjeDlaKliknietegoPrzyciskuObslugaKopalni
(uint8_t && wJakiPrzyciskKliknieto, bool & czyWyjscZObslugiKopalni);
void UtworzElementyGraficzneObslugiKopalni();

void ObslugaKopalni()
{
	bool czyWyjscZObslugiKopalni = false;
	UtworzElementyGraficzneObslugiKopalni();
	while(GraficznyInterfejsUzytkownika::okno.isOpen() && !czyWyjscZObslugiKopalni)
	{
		UruchomFunkcjeDlaKliknietegoPrzyciskuObslugaKopalni
		(GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna(), czyWyjscZObslugiKopalni);
	}
}

void UruchomFunkcjeDlaKliknietegoPrzyciskuObslugaKopalni
(uint8_t && wJakiPrzyciskKliknieto, bool & czyWyjscZObslugiKopalni)
{
	switch(wJakiPrzyciskKliknieto)
	{
		case 1:
		{
			std::string jakiPracownik = "";
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, "Wprowadz tutaj\nnumer pracownika\nod 1 do "
			+ std::to_string(Gracz::ZwrocOstatniNumerPracownika()) + "\ni nacisnij enter:");
			GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 160.f);
			uint8_t jakieZdarzenie = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
			while(jakieZdarzenie > 13 && GraficznyInterfejsUzytkownika::okno.isOpen())
			{
				//Sprawdzanie czy kod ascii z wciœniêtego klawisza to cyfra lub znak nowej linii.
				if((jakieZdarzenie == 48 && !jakiPracownik.empty())
				|| (jakieZdarzenie >= 49 && jakieZdarzenie <= 57))
				{
					jakiPracownik += jakieZdarzenie;
					GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, jakiPracownik);
				}
				jakieZdarzenie = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
			}
			if(jakieZdarzenie == 0)	//Sprawdzenie czy zamkniêto okno.
			{
				czyWyjscZObslugiKopalni = true;
				Gracz::czyTrwaGra = false;
			}
			else
			{
				uint64_t wyborPracownika = ZamienStringaNaLiczbe(jakiPracownik);
				if(wyborPracownika > 0 && wyborPracownika <= Gracz::ZwrocOstatniNumerPracownika())
				{
					GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 40.f);
					ObslugaPracownika(Gracz::ZwrocWskaznikNaWybranegoPracownika(wyborPracownika));
					UtworzElementyGraficzneObslugiKopalni();
					GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 40.f);
					GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(3, 40.f);
				}
				else
				{
					GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, "Obsluz pracownika");
				}
			}
			GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(2, 80.f);
			break;
		}
		case 2:
		{
			Gracz::KupPracownikaDlaWybranejKopalni();
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu
			(2, "Kup pracownika\nKoszt: " + std::to_string(Gracz::ZwrocKosztKupnaNowegoPracownika()));
			break;
		}
		case 3:
		{
			czyWyjscZObslugiKopalni = true;
			GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 40.f);
			break;
		}
	}
}

void UtworzElementyGraficzneObslugiKopalni()
{
	GraficznyInterfejsUzytkownika::UtworzPrzyciski(3);
	{
		std::array <std::string, 3> tekstyDlaObiektowTekstu
		{
			"Obsluz pracownika",
			"Kup pracownika\nKoszt: " + std::to_string(Gracz::ZwrocKosztKupnaNowegoPracownika()),
			"Wyjdz"
		};
		GraficznyInterfejsUzytkownika::UtworzObiektyTekstow(3, tekstyDlaObiektowTekstu.data());
	}
	GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(2, 80.f);
}