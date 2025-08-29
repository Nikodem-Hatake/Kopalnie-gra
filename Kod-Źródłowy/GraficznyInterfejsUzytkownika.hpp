#pragma once
#ifndef GraficznyInterfejsUzytkownika_hpp
#define GraficznyInterfejsUzytkownika_hpp
#include "header.hpp"

class GraficznyInterfejsUzytkownika
{
	private:
	static sf::Font czcionka;
	static sf::RectangleShape prostokatDoWyswietlaniaInformacji;
	static std::list <sf::RectangleShape> przyciski;
	static sf::Text tekstDlaProstokataDoWyswietlaniaInformacji;
	static std::vector <sf::Text> teksty;

	public:
	static void AktualizujInformacjeOIlosciPieniedzy();
	static uint8_t ObslugaZdarzenOkna();
	static void PrzywrocOstatniPrzycisk();
	static sf::RenderWindow okno;
	static void RenderowanieGrafiki();
	static uint8_t SprawdzWJakiPrzyciskKliknieto(sf::Vector2i && pozycjaWJakaKliknalGracz);
	static void UstawKolorObiektuTekstu(uint8_t && numerObiektuTekstu, sf::Color && kolorDlaTekstu);
	static void UstawLosowaPozycjeRudy(float && losowaPozycjaX, float && losowaPozycjaY);
	static void UstawSamTekstDlaDodatkowegoTekstu(std::string && nowyTekst);
	static void UstawTekstObiektuTekstu(uint8_t && numerObiektuTekstu, std::string nowyTekst);
	static void UstawWlasciwosciDlaDodatkowegoTekstu(std::string && tekst,
	sf::Vector2f && pozycjaTekstu, sf::Color && kolorDlaTekstu);
	static void UtworzDodatkoweElementy();
	static void UtworzObiektyTekstow(size_t && ileObiektowTekstu, std::string tekstyDlaObiektowTekstu[]);
	static void UtworzPrzyciski(size_t && ilePrzyciskow);
	static void UtworzRudeZOstatniegoPrzycisku();
	static void ZmienWysokoscPrzycisku(uint8_t && numerPrzycisku, float && nowaWysokosc);
};

#endif