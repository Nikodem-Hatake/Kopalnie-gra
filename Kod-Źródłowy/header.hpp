#pragma once
#ifndef header_hpp
#define header_hpp

#include <conio.h>
#include <condition_variable>
#include <fstream>
#include <future>
#include <iostream>
#include <list>
#include <mutex>
#include <random>
#include <thread>
#include <Windows.h>

class Pracownik
{
	private:
	//first - poziom ulepszenia, second.first - koszt ulepszenia, second.second - o ile wzrasta koszt.
	std::pair <uint8_t, std::pair <uint64_t, uint64_t>> ulepszenia[2];
	uint16_t iloscWydobywanychZloz;	//Ilosc wydobywanych z³ó¿ co jedno wykopanie.
	uint16_t predkoscKopaniaWMilisekundach;	//Prêdkoœæ w jakiej odbywa siê jedno wykopanie.
	std::string imie;	//Imie pracownika.

	friend void Zapis(std::atomic_uint64_t & kasa, std::list <std::list <Pracownik>> & kopalnie);
	friend bool ZapisAutoSave(std::atomic_uint64_t * kasa, std::list <std::list <Pracownik>> * kopalnie,
	std::mutex * blokadaDostepuDoKopalni, std::atomic_bool * czyTrwaGra,
	std::condition_variable * powiadomienieODostepieDoKopalni, std::atomic_bool * powiadomienie);

	public:
	void Kopanie(std::atomic_bool & czyTrwaGra, std::atomic_uint64_t & kasa,
	uint64_t && ileKasyZaZloze);
	void InformacjeOPracowniku();	//Wyœwietla informacje o pracowniku.
	void Ulepszenie(char & jakieUlepszenie, std::atomic_uint64_t & kasa);
	void ZmianaImienia(std::string & noweImie);
	Pracownik(uint64_t kosztyUlepszen, std::string imieDlaPracownika,
	uint16_t poziomIlosciWydobywanychZloz, uint16_t poziomPredkosciKopaniaWMilisekundach);
	~Pracownik();
};

//Funkcje w ca³ym programie.
bool czyPoprawnaLiczba(std::string & liczba, uint64_t && maksymalnaWartosc,
uint64_t & liczbaWyjscie);
void Gra(bool & czyAutoSave, std::list <std::list <Pracownik>> & kopalnie, std::atomic_uint64_t & kasa);
void KopanieSamemu(std::atomic_uint64_t & kasa);
void Lobby(bool & czyWlaczonyAutoSave);
void LobbyGry(std::atomic_uint64_t & kasa);
void LobbyObslugiKopalni(uint64_t & numerKopalni, uint64_t && iloscPracownikow, std::atomic_uint64_t & kasa);
void LobbyObslugiPracownika(std::atomic_uint64_t & kasa);
void ObslugaKopalni(std::list <Pracownik> & kopalnia, uint64_t & numerKopalni, std::atomic_bool & czyTrwaGra,
std::atomic_uint64_t & kasa, std::list <std::jthread> & watki, std::mutex & blokadaDostepuDoKopalni,
std::condition_variable & powiadomienieODostepieDoKopalni, std::atomic_bool & powiadomienie);
void ObslugaPracownika(std::atomic_uint64_t & kasa, Pracownik & pracownik, std::mutex & blokadaDostepuDoKopalni,
std::condition_variable & powiadomienieODostepieDoKopalni, std::atomic_bool & powiadomienie);
bool Odczyt(std::atomic_uint64_t * kasa, std::list <std::list <Pracownik>> * kopalnie);
void Zapis(std::atomic_uint64_t & kasa, std::list <std::list <Pracownik>> & kopalnie);
bool ZapisAutoSave(std::atomic_uint64_t * kasa, std::list <std::list <Pracownik>> * kopalnie,
std::mutex * blokadaDostepuDoKopalni, std::atomic_bool * czyTrwaGra,
std::condition_variable * powiadomienieODostepieDoKopalni, std::atomic_bool * powiadomienie);

#endif