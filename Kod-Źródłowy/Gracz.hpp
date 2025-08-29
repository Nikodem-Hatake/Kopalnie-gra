#pragma once
#ifndef Gracz_hpp
#define Gracz_hpp
#include "header.hpp"

class Gracz
{
	private:
	static std::future <bool> autoSaveWTle;
	static bool czyWczytanoPlik;
	static std::atomic_uint64_t kasa;
	static std::vector <Kopalnia> kopalnie;
	friend class GraficznyInterfejsUzytkownika;
	friend class Kopalnia;
	friend void KopanieSamemu();
	friend bool Odczyt();
	friend class Pracownik;
	static std::future <bool> wczytywaniePracownikowOrazKasy;
	friend void Zapis();
	friend bool ZapisAutoSave();

	public:
	static void AktywujAutoSave();
	static bool czyAktywnyAutoSave;
	static std::atomic_bool czyTrwaGra;
	static void DodajKopalnieWPrzypadkuBrakuSavea();
	static void DodajWatkiNaPoczatkuGry();
	static void KupPracownikaDlaWybranejKopalni();
	static void KupKopalnie();
	static void OczyscWatki();
	static void SprawdzCzyAutoSaveDzialal();
	static void SprawdzCzyWczytanoDane();
	static uint64_t wybranyNumerKopalni;
	static std::string ZwrocIloscKasyJakoString();
	static uint64_t ZwrocKosztKupnaNowegoPracownika();
	static uint64_t ZwrocKosztKupnaNowejKopalni();
	static uint64_t ZwrocOstatniNumerKopalni();
	static uint64_t ZwrocOstatniNumerPracownika();
	static Pracownik * ZwrocWskaznikNaWybranegoPracownika(uint64_t & wybranyPracownik);
};

#endif