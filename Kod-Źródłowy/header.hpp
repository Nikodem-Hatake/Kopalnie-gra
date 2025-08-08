#pragma once
#ifndef header_hpp
#define header_hpp

#include <atomic>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <list>
#include <mutex>
#include <random>
#include <thread>
#include <vector>
#include <Windows.h>

class Pracownik
{
	private:
	//first - poziom ulepszenia, second - first - koszt ulepszenia, second - second - o ile wzrasta koszt.
	std::pair <uint8_t, std::pair <uint64_t, uint64_t>> ulepszenia[2];
	uint16_t iloscWydobywanychZloz;	//Ilosc wydobywanych z³ó¿ co jedno wykopanie.
	uint16_t predkoscKopaniaWMilisekundach;	//Prêdkoœæ w jakiej odbywa siê jedno wykopanie.
	std::string imie;	//Imie pracownika.

	friend void Zapis(std::atomic_uint64_t & kasa, std::vector <std::vector <Pracownik>> & kopalnie);

	public:
	void Kopanie(std::atomic_bool & czyTrwaGra, std::atomic_uint64_t & kasa,
	uint64_t && ileKasyZaZloze, std::mutex & blokadaDostepuDoKasy);
	void InformacjeOPracowniku();	//Wyœwietla informacje o pracowniku.
	void Ulepszenie(char & jakieUlepszenie, std::atomic_uint64_t & kasa, std::mutex & blokadaDostepuDoKasy);
	void ZmianaImienia(std::string & noweImie);
	Pracownik(uint64_t kosztyUlepszen, std::string imieDlaPracownika,
	uint16_t poziomIlosciWydobywanychZloz, uint16_t poziomPredkosciKopaniaWMilisekundach);
	~Pracownik();
};

//Funkcje w ca³ym programie.
void Gra();
void KopanieSamemu(std::atomic_uint64_t & kasa, std::mutex & blokadaDostepuDoKasy);
void Lobby();
void LobbyGry(std::atomic_uint64_t & kasa);
void LobbyObslugiKopalni(char & numerKopalni, uint64_t && iloscPracownikow, std::atomic_uint64_t & kasa);
void LobbyObslugiPracownika(std::atomic_uint64_t & kasa);
void ObslugaKopalni(std::vector <Pracownik> & kopalnia, char & numerKopalni, std::atomic_bool & czyTrwaGra,
std::atomic_uint64_t & kasa, std::mutex & blokadaDostepuDoKasy, std::list <std::thread> & watki);
void ObslugaPracownika(std::atomic_uint64_t & kasa, std::mutex & blokadaDostepuDoKasy, Pracownik & pracownik);
void Odczyt(std::atomic_uint64_t & kasa, std::vector <std::vector <Pracownik>> & kopalnie);
void Zapis(std::atomic_uint64_t & kasa, std::vector <std::vector <Pracownik>> & kopalnie);

#endif