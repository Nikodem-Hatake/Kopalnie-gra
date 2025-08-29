#include "header.hpp"

uint64_t ZamienStringaNaLiczbe(std::string & liczba)
{
	if(liczba.empty())
	{
		return 0;
	}
	if(liczba[0] == '0' && liczba.length() != 1)
	{
		return 0;
	}
	uint64_t mnozenie = 1;
	//Sprawdzanie czy ka¿dy znak jest cyfr¹.
	for(const char & cyfra : liczba)
	{
		if(cyfra < 48 || cyfra > 57)
		{
			return 0;
		}
		mnozenie *= 10;
	}
	mnozenie /= 10;
	uint64_t liczbaWyjscie = 0;
	//Zamiana ³añcucha znaków na cyfrê.
	for(const char & cyfra : liczba)
	{
		liczbaWyjscie += (static_cast <uint64_t> (cyfra - 48) * mnozenie);
		mnozenie /= 10;
	}
	return liczbaWyjscie;
}