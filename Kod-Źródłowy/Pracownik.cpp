#include "header.hpp"

void Pracownik::Kopanie(uint64_t && ileKasyZaZloze)
{
	while(Gracz::czyTrwaGra)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(this -> predkoscKopaniaWMilisekundach));
		uint64_t ileKasyZaWydobycie = (ileKasyZaZloze * this->iloscWydobywanychZloz);
		Gracz::kasa += ileKasyZaWydobycie;
		this -> ileZarobil += ileKasyZaWydobycie;
	}
}

void Pracownik::Ulepszenie(uint8_t && jakieUlepszenie)
{
	if(this -> ulepszenia[jakieUlepszenie].first == 5)
	{
		return;
	}
	else if(this -> ulepszenia[jakieUlepszenie].second.first > Gracz::kasa)
	{
		return;
	}
	Gracz::kasa -= ulepszenia[jakieUlepszenie].second.first;
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
}

void Pracownik::ZmianaImienia(std::string & noweImie)
{
	if(noweImie.empty())
	{
		return;
	}
	else if(noweImie == this -> imie)
	{
		return;
	}
	this -> imie = noweImie;
}

std::string Pracownik::ZwrocInformacje(uint8_t && jakieInformacje)
{
	switch(jakieInformacje)
	{
		case 1:
		{
			return "Imie: " + this -> imie;
			break;
		}
		case 2:
		{
			std::string zwracaneInformacje = "Poziom ilosc\nwydobywanych\nzloz: ";
			if(this -> ulepszenia[0].first == 5)
			{
				zwracaneInformacje += "Maksymalny";
			}
			else
			{
				zwracaneInformacje += std::to_string(this -> ulepszenia[0].first);
			}
			return zwracaneInformacje;
			break;
		}
		case 3:
		{
			std::string zwracaneInformacje = "Poziom predkosci\nkopania: ";
			if(this -> ulepszenia[1].first == 5)
			{
				zwracaneInformacje += "Maksymalny";
			}
			else
			{
				zwracaneInformacje += std::to_string(this -> ulepszenia[1].first);
			}
			return zwracaneInformacje;
			break;
		}
		case 4:
		{
			return "Ile zarobil: " + std::to_string(this -> ileZarobil);
			break;
		}
	}
	return "";	//W przypadku przes³ania z³ego argumentu.
}

std::string Pracownik::ZwrocPoziomUlepszeniaOrazCene(uint8_t && jakieUlepszenie)
{
	if(this -> ulepszenia[jakieUlepszenie].first == 5)
	{
		return "Maksymalny poziom";
	}
	return "Poziom: " + std::to_string(ulepszenia[jakieUlepszenie].first) + '\n'
	+ "Cena: " + std::to_string(ulepszenia[jakieUlepszenie].second.first);
}

Pracownik::Pracownik(uint64_t & kosztyUlepszen, std::string & imieDlaPracownika,
uint16_t & poziomIlosciWydobywanychZloz, uint16_t & poziomPredkosciKopaniaWMilisekundach, uint64_t & ileZarobil)
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
	this -> ileZarobil = ileZarobil;
}

Pracownik::Pracownik(uint64_t && kosztyUlepszen, std::string imieDlaPracownika,
uint16_t && poziomIlosciWydobywanychZloz, uint16_t && poziomPredkosciKopaniaWMilisekundach, uint64_t && ileZarobil)
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
	this -> ileZarobil = ileZarobil;
}