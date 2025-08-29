#pragma once
#ifndef Kopalnia_hpp
#define Kopalnia_hpp

#include "header.hpp"

class Kopalnia
{
	private:
	static std::mutex blokadaDostepuDoKopalni;
	static std::atomic_bool powiadomienie;
	static std::condition_variable powiadomienieODostepieDoKopalni;
	std::list <Pracownik> pracownicy;
	uint64_t numerKopalni;
	static std::forward_list <std::jthread> watkiDlaPracownikow;
	friend class Gracz;
	friend void ObslugaPracownika(Pracownik * pracownik);
	friend bool Odczyt();
	friend void Zapis();
	friend bool ZapisAutoSave();

	public:
	void DodajWatekDlaPracownika();
	void KupPracownika();
	Kopalnia(uint64_t && numerKopalni);
	Kopalnia(uint64_t & numerKopalni);
	Kopalnia() = default;
	~Kopalnia() = default;
};

#endif