#include "header.hpp"

void KopanieSamemu(std::atomic_uint64_t & kasa)
{
	HANDLE kolor = GetStdHandle(STD_OUTPUT_HANDLE);
	auto ziarno = time(0);
	std::default_random_engine generatorLosowychLiczb(ziarno);
	std::uniform_int_distribution <short> zakres(1, 1000);
	std::uniform_int_distribution <short> zakresRudy(1, 100);
	std::pair <std::string, uint64_t> rudy[5] 
	{
		{"Wegiel", 5},
		{"Zelazo", 10},
		{"Lapis", 15},
		{"Ametyst", 30},
		{"Diament", 50}
	};
	char znaczek = 0;
	short czyWydobyto = 0;
	std::cout << std::string(28, '-') << '\n';
	std::cout << "Kop poprzez trzymanie ciagle dowolnego przycisku poza \'x\'" << '\n';
	std::cout << "Co ktores wykopanie masz szanse na wydobycie rudy" << '\n';
	std::cout << "Nacisniecie klawisza \'x\' spowoduje wyjscie z kopania samemu" << '\n';
	while(znaczek != 'x' && znaczek != 'X')
	{
		znaczek = _getch();
		czyWydobyto = zakres(generatorLosowychLiczb);
		//Wydobycie rudy (szansa 2%).
		if(czyWydobyto >= 980 && znaczek != 'x' && znaczek != 'X')
		{
			std::cout << "Wydobyto ";
			//Wylosowanie rudy jak¹ siê wydoby³o.
			short jakaRuda = zakresRudy(generatorLosowychLiczb);
			if(jakaRuda <= 30)	//30%.
			{
				SetConsoleTextAttribute(kolor, 8);
				jakaRuda = 0;
			}
			else if(jakaRuda <= 50)	//20%.
			{
				SetConsoleTextAttribute(kolor, 8);
				jakaRuda = 1;
			}
			else if(jakaRuda <= 70)	//20%.
			{
				SetConsoleTextAttribute(kolor, 1);
				jakaRuda = 2;
			}
			else if(jakaRuda <= 85)	//15%.
			{
				SetConsoleTextAttribute(kolor, 5);
				jakaRuda = 3;
			}
			else	//15%.
			{
				SetConsoleTextAttribute(kolor, 3);
				jakaRuda = 4;
			}
			std::cout << rudy[jakaRuda].first;
			SetConsoleTextAttribute(kolor, 7);
			std::cout << " zarabiasz " << rudy[jakaRuda].second << " kasy" << '\n';
			kasa += rudy[jakaRuda].second;
		}
	}
}