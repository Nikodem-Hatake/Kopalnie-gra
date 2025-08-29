#pragma once
#ifndef Pracownik_hpp
#define Pracownik_hpp
#include "header.hpp"

class Pracownik
{
	private:
	//first - poziom ulepszenia, second.first - koszt ulepszenia, second.second - o ile wzrasta koszt.
	std::array <std::pair <uint16_t, std::pair <uint64_t, uint64_t>>, 2> ulepszenia;
	uint16_t iloscWydobywanychZloz;
	uint16_t predkoscKopaniaWMilisekundach;
	uint64_t ileZarobil;
	std::string imie;

	friend void Zapis();
	friend bool ZapisAutoSave();

	public:
	void Kopanie(uint64_t && ileKasyZaZloze);
	void Ulepszenie(uint8_t && jakieUlepszenie);
	void ZmianaImienia(std::string & noweImie);
	std::string ZwrocInformacje(uint8_t && jakieInformacje);
	std::string ZwrocPoziomUlepszeniaOrazCene(uint8_t && jakieUlepszenie);
	//Konstruktor dla rvalues (u¿ywany przy kupowaniu nowego pracownika lub przy uruchomieniu gry bez danych.
	Pracownik(uint64_t && kosztyUlepszen, std::string imieDlaPracownika,
	uint16_t && poziomIlosciWydobywanychZloz, uint16_t && poziomPredkosciKopaniaWMilisekundach, uint64_t && ileZarobil);
	//Konstruktor dla lvalues (u¿ywany przy odczycie danych z pliku).
	Pracownik(uint64_t & kosztyUlepszen, std::string & imieDlaPracownika,
	uint16_t & poziomIlosciWydobywanychZloz, uint16_t & poziomPredkosciKopaniaWMilisekundach, uint64_t & ileZarobil);
	~Pracownik() = default;
};

#endif