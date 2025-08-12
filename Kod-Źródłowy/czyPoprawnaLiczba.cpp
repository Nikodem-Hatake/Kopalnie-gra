#include "header.hpp"

bool czyPoprawnaLiczba(std::string & liczba, uint64_t && maksymalnaWartosc,
uint64_t & liczbaWyjscie)
{
	if(liczba.empty())
	{
		std::cout << "Niewprowadzono danych" << '\n';
		return false;
	}
	if(liczba[0] == '0' && liczba.length() != 1)
	{
		std::cout << "Wprowadzono niepoprawne dane" << '\n';
		return false;
	}
	if(liczba.length() > 2)	//Je¿eli liczba jest wiêksza od 99.
	{
		std::cout << "Wybrano niepoprawna opcje" << '\n';
		return false;
	}
	uint64_t mnozenie = 1;
	//Sprawdzanie czy ka¿dy znak jest cyfr¹.
	for(size_t indeks = 0; indeks < liczba.length(); indeks ++)
	{
		if(liczba[indeks] < 48 || liczba[indeks] > 57)
		{
			std::cout << "Wprowadzono niepoprawne dane" << '\n';
			return false;
		}
		mnozenie *= 10;
	}
	mnozenie /= 10;
	//Zamiana ³añcucha znaków na cyfrê.
	for(const char & cyfra : liczba)
	{
		liczbaWyjscie += (static_cast <uint64_t> (cyfra - 48) * mnozenie);
		mnozenie /= 10;
	}
	if(liczbaWyjscie >= maksymalnaWartosc)
	{
		std::cout << "Wybrano niepoprawna opcje" << '\n';
		return false;
	}
	return true;
}