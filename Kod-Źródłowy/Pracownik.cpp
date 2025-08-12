#include "header.hpp"

void Pracownik::InformacjeOPracowniku()
{
	std::cout << std::string(28, '-') << '\n';
	std::cout << this -> imie << ':' << '\n';
	std::cout << "Ilosc wydobywanych zloz na jedno wykopanie: " << this -> iloscWydobywanychZloz << '\n';
	std::cout << "Predkosc kopania: " << this -> predkoscKopaniaWMilisekundach << "ms" << '\n';
	std::cout << "Dostepne ulepszenia: " << '\n';
	std::cout << "Ilosc wydobywanych zloz: ";
	std::cout << "Poziom - ";
	if(this -> ulepszenia[0].first == 5)
	{
		std::cout << "Maksymalny.";
	}
	else
	{
		std::cout << static_cast <short> (this -> ulepszenia[0].first);
		std::cout << ", Koszt ulepszenia - " << this -> ulepszenia[0].second.first << '.';
	}
	std::cout << '\n' << "Predkosc kopania w milisekundach: ";
	std::cout << "Poziom - ";
	if(this -> ulepszenia[1].first == 5)
	{
		std::cout << "Maksymalny.";
	}
	else
	{
		std::cout << static_cast <short> (this -> ulepszenia[1].first);
		std::cout << ". Koszt ulepszenia - " << this->ulepszenia[1].second.first << '.';
	}
	std::cout << '\n' << std::string(28, '-');
}

void Pracownik::Kopanie(std::atomic_bool & czyTrwaGra, std::atomic_uint64_t & kasa,
uint64_t && ileKasyZaZloze)
{
	while(czyTrwaGra)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(this -> predkoscKopaniaWMilisekundach));
		//Zarobienie kasy poprzez zablokowanie dostêpu do kasy dla innych w¹tków na czas dzia³ania jednego (mutex).
		kasa += (ileKasyZaZloze * this -> iloscWydobywanychZloz);
	}
}

void Pracownik::Ulepszenie(char & jakieUlepszenie, std::atomic_uint64_t & kasa)
{
	if(this -> ulepszenia[jakieUlepszenie].first == 5)
	{
		std::cout << "To ulepszenie ma juz maksymalny poziom" << '\n';
		return;	//Niemo¿liwe ulepszenie - maksymalny poziom ulepszenia.
	}
	else if(this -> ulepszenia[jakieUlepszenie].second.first > kasa)
	{
		std::cout << "Niestac cie na to ulepszenie" << '\n';
		return;	//Za drogie ulepszenie.
	}
	kasa -= ulepszenia[jakieUlepszenie].second.first;
	this -> ulepszenia[jakieUlepszenie].first ++;
	this -> ulepszenia[jakieUlepszenie].second.first += this -> ulepszenia[jakieUlepszenie].second.second;
	if(jakieUlepszenie == 0)
	{
		this -> iloscWydobywanychZloz ++;
	}
	else
	{
		this -> predkoscKopaniaWMilisekundach -= 500;
	}
	std::cout << "Zakup udany ^^" << '\n';
}

void Pracownik::ZmianaImienia(std::string & noweImie)
{
	if(noweImie.empty())
	{
		std::cout << "Niewprowadzono imienia, zmiana nieudana" << '\n';
		return;
	}
	else if(noweImie == this -> imie)
	{
		std::cout << "Pracownik posiada juz takie imie" << '\n';
		return;
	}
	this -> imie = noweImie;
	std::cout << "Zmiana udana ^^" << '\n';
}

Pracownik::Pracownik(uint64_t kosztyUlepszen, std::string imieDlaPracownika,
uint16_t poziomIlosciWydobywanychZloz, uint16_t poziomPredkosciKopaniaWMilisekundach)
{
	this -> iloscWydobywanychZloz = poziomIlosciWydobywanychZloz;
	this -> predkoscKopaniaWMilisekundach = 3000 - (500 * poziomPredkosciKopaniaWMilisekundach);
	this -> ulepszenia[0].first = poziomIlosciWydobywanychZloz;
	this -> ulepszenia[0].second.first = kosztyUlepszen * static_cast <uint64_t> (poziomIlosciWydobywanychZloz);
	this -> ulepszenia[0].second.second = kosztyUlepszen;
	this -> ulepszenia[1].first = poziomPredkosciKopaniaWMilisekundach;
	this -> ulepszenia[1].second.first = (kosztyUlepszen / 2) 
	* static_cast <uint64_t> (poziomPredkosciKopaniaWMilisekundach);
	this -> ulepszenia[1].second.second = kosztyUlepszen / 2;
	this -> imie = imieDlaPracownika;
}

Pracownik::~Pracownik()
{

}