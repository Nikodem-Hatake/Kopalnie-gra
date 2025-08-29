#include "header.hpp"

std::future <bool> Gracz::autoSaveWTle;
bool Gracz::czyAktywnyAutoSave = true;
bool Gracz::czyWczytanoPlik = false;
std::atomic_bool Gracz::czyTrwaGra = false;
std::atomic_uint64_t Gracz::kasa = 0;
std::vector <Kopalnia> Gracz::kopalnie;
std::future <bool> Gracz::wczytywaniePracownikowOrazKasy = std::async(std::launch::async, Odczyt);
uint64_t Gracz::wybranyNumerKopalni = 0;

void Gracz::AktywujAutoSave()
{
	autoSaveWTle = std::async(std::launch::async, ZapisAutoSave);
}

void Gracz::DodajKopalnieWPrzypadkuBrakuSavea()
{
	if(!kopalnie.empty())
	{
		return;
	}
	kopalnie.emplace_back(1);
	kopalnie.back().pracownicy.emplace_back(10, std::move("Pracownik0"), 1, 1, 0);
}

void Gracz::DodajWatkiNaPoczatkuGry()
{
	for(Kopalnia & kopalnia : kopalnie)
	{
		for(Pracownik & pracownik : kopalnia.pracownicy)
		{
			kopalnia.watkiDlaPracownikow.emplace_front([&]
			{
				pracownik.Kopanie(kopalnia.numerKopalni * 2);
			});
		}
	}
}

void Gracz::KupPracownikaDlaWybranejKopalni()
{
	kopalnie[wybranyNumerKopalni - 1].KupPracownika();
}

void Gracz::KupKopalnie()
{
	uint64_t kosztKupnaKopalni = Gracz::ZwrocKosztKupnaNowejKopalni();
	if(Gracz::kasa < kosztKupnaKopalni)
	{
		return;
	}
	Kopalnia::powiadomienie = false;
	Gracz::kasa -= kosztKupnaKopalni;
	kopalnie.emplace_back(kopalnie.back().numerKopalni + 1);
	Kopalnia::powiadomienie = true;
	Kopalnia::powiadomienieODostepieDoKopalni.notify_one();
}

void Gracz::OczyscWatki()
{
	for(Kopalnia & kopalnia : kopalnie)
	{
		kopalnia.watkiDlaPracownikow.clear();
	}
}

void Gracz::SprawdzCzyAutoSaveDzialal()
{
	if(Gracz::autoSaveWTle.get())
	{
		GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu
		("Zapisano ^^", {120.f, 24.f}, sf::Color(0, 240, 0));
	}
	else
	{
		GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu
		("Nieudalo sie zapisac", {40.f, 24.f}, sf::Color(240, 0, 0));
	}
}

void Gracz::SprawdzCzyWczytanoDane()
{
	GraficznyInterfejsUzytkownika::UstawSamTekstDlaDodatkowegoTekstu("Wczytywanie...");
	if(czyWczytanoPlik)
	{
		return;
	}
	if(!Gracz::wczytywaniePracownikowOrazKasy.get())
	{
		GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu
		("Nieudalo sie wczytac saveu", {25.f, 104.f}, sf::Color(240, 0, 0));
		std::this_thread::sleep_for(std::chrono::milliseconds(2500));
	}
	czyWczytanoPlik = true;
}

std::string Gracz::ZwrocIloscKasyJakoString()
{
	return std::to_string(kasa);
}

uint64_t Gracz::ZwrocKosztKupnaNowegoPracownika()
{
	return (kopalnie[wybranyNumerKopalni - 1].pracownicy.size() + 1) * wybranyNumerKopalni * 1000;
}

uint64_t Gracz::ZwrocKosztKupnaNowejKopalni()
{
	return kopalnie.back().numerKopalni * 25000;
}

uint64_t Gracz::ZwrocOstatniNumerKopalni()
{
	return kopalnie.size();
}

uint64_t Gracz::ZwrocOstatniNumerPracownika()
{
	return kopalnie[wybranyNumerKopalni - 1].pracownicy.size();
}

Pracownik * Gracz::ZwrocWskaznikNaWybranegoPracownika(uint64_t & wybranyPracownik)
{
	std::list <Pracownik>::iterator it;
	//Je¿eli iterator ma byæ dalej ni¿ po³owa listy.
	if(wybranyPracownik > kopalnie[wybranyNumerKopalni - 1].pracownicy.size() / 2)
	{
		it = kopalnie[wybranyNumerKopalni - 1].pracownicy.end();
		it --;
		wybranyPracownik = kopalnie[wybranyNumerKopalni - 1].pracownicy.size() - wybranyPracownik;
		while(wybranyPracownik --)
		{
			it --;
		}
	}
	else	//Je¿eli iterator ma byæ przed po³ow¹ listy.
	{
		it = kopalnie[wybranyNumerKopalni - 1].pracownicy.begin();
		wybranyPracownik --;
		while(wybranyPracownik --)
		{
			it ++;
		}
	}
	return &(* it);
}
