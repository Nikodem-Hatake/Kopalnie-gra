#include "header.hpp"

void UruchomFunkcjeDlaKliknietegoPrzyciskuMain(uint8_t && wJakiPrzyciskKliknieto);

int main()
{
	GraficznyInterfejsUzytkownika::UtworzPrzyciski(3);
	{	//Dodatkowe zagnie�d�enie, �eby array string�w zosta� p�niej usuni�ty z pami�ci.
		std::array <std::string, 3> tekstyDlaObiektowTekstu
		{
			"Rozpocznij gre",
			"Wylacz auto save",
			"Wyjdz"
		};
		GraficznyInterfejsUzytkownika::UtworzObiektyTekstow(3, tekstyDlaObiektowTekstu.data());
	}
	GraficznyInterfejsUzytkownika::UtworzDodatkoweElementy();
	GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu("KOPALNIE", {120.f, 24.f}, sf::Color(0, 0, 0));
	//Wy��czenie renderowania w g��wnym w�tku (zamiast tego odbywa si� w w�tku poni�ej)
	GraficznyInterfejsUzytkownika::okno.setActive(false);
	std::jthread renderowanieGrafikiWTle(GraficznyInterfejsUzytkownika::RenderowanieGrafiki);

	while(GraficznyInterfejsUzytkownika::okno.isOpen())	//G��wna p�tla gry.
	{
		UruchomFunkcjeDlaKliknietegoPrzyciskuMain(GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna());
	}
}

void UruchomFunkcjeDlaKliknietegoPrzyciskuMain(uint8_t && wJakiPrzyciskKliknieto)
{
	switch(wJakiPrzyciskKliknieto)
	{
		case 1:
		{
			Gracz::SprawdzCzyWczytanoDane();
			Gra();
			//Renderowanie z powrotem poprzednich element�w.
			GraficznyInterfejsUzytkownika::UtworzPrzyciski(3);
			{
				std::array <std::string, 3> tekstyDlaObiektowTekstu
				{
					"Rozpocznij gre",
					"Wylacz auto save",
					"Wyjdz"
				};
				if(!Gracz::czyAktywnyAutoSave)
				{
					tekstyDlaObiektowTekstu[1] = "Wlacz auto save";
				}
				GraficznyInterfejsUzytkownika::UtworzObiektyTekstow(3, tekstyDlaObiektowTekstu.data());
			}
			break;
		}
		case 2:
		{
			if(Gracz::czyAktywnyAutoSave)
			{
				Gracz::czyAktywnyAutoSave = false;
				GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(2, "Wlacz auto save");
			}
			else
			{
				Gracz::czyAktywnyAutoSave = true;
				GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(2, "Wylacz auto save");
			}
			break;
		}
		case 0:
		case 3:
		{
			GraficznyInterfejsUzytkownika::okno.close();
			break;
		}
	}
}