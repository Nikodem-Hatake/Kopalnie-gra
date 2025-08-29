#include "header.hpp"

void UruchomFunkcjeDlaKliknietegoPrzyciskuObslugaKopalni
(uint8_t && wJakiPrzyciskKliknieto, bool & czyWyjscZObslugiPracownika, Pracownik * pracownik);
void UtworzElementyGraficzneObslugiPracownika(Pracownik * pracownik);

void ObslugaPracownika(Pracownik * pracownik)
{
	Kopalnia::powiadomienie = false;
	bool czyWyjscZObslugiPracownika = false;
	UtworzElementyGraficzneObslugiPracownika(pracownik);
	while(GraficznyInterfejsUzytkownika::okno.isOpen() && !czyWyjscZObslugiPracownika)
	{
		UruchomFunkcjeDlaKliknietegoPrzyciskuObslugaKopalni
		(GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna(), czyWyjscZObslugiPracownika, pracownik);
	}
	Kopalnia::powiadomienie = true;
	Kopalnia::powiadomienieODostepieDoKopalni.notify_one();
}

void UruchomFunkcjeDlaKliknietegoPrzyciskuObslugaKopalni
(uint8_t && wJakiPrzyciskKliknieto, bool & czyWyjscZObslugiPracownika, Pracownik * pracownik)
{
	switch(wJakiPrzyciskKliknieto)
	{
		case 1:
		{
			std::string nowaNazwaPracownika = "";
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, "Wprowadz nowe imie\ni nacisnin enter:");
			uint8_t jakieZdarzenie = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
			while(jakieZdarzenie > 13 && GraficznyInterfejsUzytkownika::okno.isOpen())
			{
				//Sprawdzanie czy kod ascii z wciœniêtego klawisza to cyfra lub znak nowej linii.
				if(jakieZdarzenie >= 33 && jakieZdarzenie <= 126 &&
				nowaNazwaPracownika.size() <= 12 && jakieZdarzenie != 92)
				{
					nowaNazwaPracownika += jakieZdarzenie;
					GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, nowaNazwaPracownika);
				}
				jakieZdarzenie = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
			}
			if(jakieZdarzenie == 0)	//Sprawdzenie czy zamkniêto okno.
			{
				czyWyjscZObslugiPracownika = true;
				Gracz::czyTrwaGra = false;
			}
			else
			{
				pracownik -> ZmianaImienia(nowaNazwaPracownika);
				GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, "Zmien imie\npracownika");
			}
			break;
		}
		case 2:
		case 3:
		{
			pracownik -> Ulepszenie(wJakiPrzyciskKliknieto - 2);
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu
			(2, "Ulepsz ilosc\nwydobywanych zloz\n" + pracownik->ZwrocPoziomUlepszeniaOrazCene(0));
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu
			(3, "Ulepsz szybkosc\nkopania\n" + pracownik -> ZwrocPoziomUlepszeniaOrazCene(1));
			break;
		}
		case 4:
		{
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(1, pracownik -> ZwrocInformacje(1));
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(2, pracownik -> ZwrocInformacje(2));
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(3, pracownik -> ZwrocInformacje(3));
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(4, pracownik -> ZwrocInformacje(4));
			GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(5, "Nacisnij dowolny\nprzycisk aby wyjsc");
			GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(5, 80.f);
			uint8_t jakieZdarzenie = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
			while(jakieZdarzenie > 5)
			{
				jakieZdarzenie = GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna();
			}
			if(jakieZdarzenie == 0)	//Sprawdzenie czy zamkniêto okno.
			{
				czyWyjscZObslugiPracownika = true;
				Gracz::czyTrwaGra = false;
			}
			UtworzElementyGraficzneObslugiPracownika(pracownik);
			GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(5, 40.f);
			break;
		}
		case 5:
		{
			czyWyjscZObslugiPracownika = true;
			break;
		}
	}
}

void UtworzElementyGraficzneObslugiPracownika(Pracownik * pracownik)
{
	GraficznyInterfejsUzytkownika::UtworzPrzyciski(5);
	{
		std::array <std::string, 5> tekstyDlaObiektowTekstu
		{
			"Zmien imie\npracownika",
			"Ulepsz ilosc\nwydobywanych zloz\n" + pracownik -> ZwrocPoziomUlepszeniaOrazCene(0),
			"Ulepsz szybkosc\nkopania\n" + pracownik->ZwrocPoziomUlepszeniaOrazCene(1),
			"Zobacz informacje",
			"Wyjdz"
		};
		GraficznyInterfejsUzytkownika::UtworzObiektyTekstow(5, tekstyDlaObiektowTekstu.data());
	}
	GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(1, 80.f);
	GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(2, 160.f);
	GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(3, 160.f);
}