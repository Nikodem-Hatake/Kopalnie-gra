#include "header.hpp"

std::mutex Kopalnia::blokadaDostepuDoKopalni;
std::atomic_bool Kopalnia::powiadomienie = true;
std::condition_variable Kopalnia::powiadomienieODostepieDoKopalni;
std::forward_list <std::jthread> Kopalnia::watkiDlaPracownikow;

void Kopalnia::DodajWatekDlaPracownika()
{
	watkiDlaPracownikow.emplace_front([&]
	{
		this -> pracownicy.back().Kopanie(this -> numerKopalni * 2);
	});
}

void Kopalnia::KupPracownika()
{
	uint64_t kosztKupnaPracownika = Gracz::ZwrocKosztKupnaNowegoPracownika();
	if(Gracz::kasa < kosztKupnaPracownika)
	{
		return;
	}
	powiadomienie = false;
	std::unique_lock blokada(this -> blokadaDostepuDoKopalni);
	Gracz::kasa -= kosztKupnaPracownika;
	std::string imieDlaPracownika = "Pracownik";
	imieDlaPracownika += static_cast <char> (48 + pracownicy.size());
	pracownicy.emplace_back(10 * this -> numerKopalni, std::move(imieDlaPracownika), 1, 1, 0);
	this -> DodajWatekDlaPracownika();
	powiadomienie = true;
	powiadomienieODostepieDoKopalni.notify_one();
}

Kopalnia::Kopalnia(uint64_t && numerKopalni)
{
	this -> numerKopalni = numerKopalni;
}

Kopalnia::Kopalnia(uint64_t & numerKopalni)
{
	this -> numerKopalni = numerKopalni;
}