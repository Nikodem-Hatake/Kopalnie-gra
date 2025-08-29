#include "header.hpp"

sf::Font GraficznyInterfejsUzytkownika::czcionka("resources/czcionka.ttf");
sf::RenderWindow GraficznyInterfejsUzytkownika::okno(sf::VideoMode({400, 800}), "Kopalnie", sf::Style::Close);
sf::RectangleShape GraficznyInterfejsUzytkownika::prostokatDoWyswietlaniaInformacji;
std::list <sf::RectangleShape> GraficznyInterfejsUzytkownika::przyciski;
sf::Text GraficznyInterfejsUzytkownika::tekstDlaProstokataDoWyswietlaniaInformacji
(GraficznyInterfejsUzytkownika::czcionka);
std::vector <sf::Text> GraficznyInterfejsUzytkownika::teksty;

void GraficznyInterfejsUzytkownika::AktualizujInformacjeOIlosciPieniedzy()
{
	while(Gracz::czyTrwaGra)
	{
		GraficznyInterfejsUzytkownika::UstawSamTekstDlaDodatkowegoTekstu
		("Ilosc kasy: " + Gracz::ZwrocIloscKasyJakoString());
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

uint8_t GraficznyInterfejsUzytkownika::ObslugaZdarzenOkna()
{
	while(okno.isOpen())
	{
		while(const std::optional event = GraficznyInterfejsUzytkownika::okno.pollEvent())
		{
			if(event -> is <sf::Event::Closed>())
			{
				GraficznyInterfejsUzytkownika::okno.close();
				return 0;
			}
			else if(const auto * klikniecieMyszyki = event -> getIf <sf::Event::MouseButtonPressed>())
			{
				uint8_t wJakiPrzyciskKliknieto = GraficznyInterfejsUzytkownika::SprawdzWJakiPrzyciskKliknieto(
				{klikniecieMyszyki -> position.x, klikniecieMyszyki -> position.y});
				if(wJakiPrzyciskKliknieto != 0)
				{
					return wJakiPrzyciskKliknieto;
				}
			}
			else if(const auto * wcisniecieKlawiszaZKlawiatury = event -> getIf<sf::Event::TextEntered>())
			{
				//Sprawdzanie czy wciœniêto wypisywalny klawisz lub znak nowej linii (enter).
				if((wcisniecieKlawiszaZKlawiatury -> unicode >= 32 && wcisniecieKlawiszaZKlawiatury -> unicode <= 126)
				|| (wcisniecieKlawiszaZKlawiatury -> unicode == 13))
				{
					return static_cast <uint8_t>(wcisniecieKlawiszaZKlawiatury -> unicode);
				}
			}
		}
	}
	return 0;	//Zamkniêto okno/znaleziono b³¹d.
}

void GraficznyInterfejsUzytkownika::PrzywrocOstatniPrzycisk()
{
	sf::Vector2f pozycjaPrzycisku = przyciski.front().getPosition();
	pozycjaPrzycisku.y += 80;
	przyciski.back().setFillColor(sf::Color(153, 102, 255));
	przyciski.back().setPosition(pozycjaPrzycisku);
	przyciski.back().setSize({320.f, 40.f});
}

void GraficznyInterfejsUzytkownika::RenderowanieGrafiki()
{
	okno.setActive(true);
	while(okno.isOpen())
	{
		okno.clear(sf::Color::White);	//Kolor t³a.
		okno.draw(prostokatDoWyswietlaniaInformacji);
		okno.draw(tekstDlaProstokataDoWyswietlaniaInformacji);
		for(const sf::RectangleShape & przycisk : przyciski)
		{
			okno.draw(przycisk);
		}
		for(const sf::Text & tekst : teksty)
		{
			okno.draw(tekst);
		}
		okno.display();
	}
}

uint8_t GraficznyInterfejsUzytkownika::SprawdzWJakiPrzyciskKliknieto(sf::Vector2i && pozycjaWJakaKliknalGracz)
{
	std::list <sf::RectangleShape>::iterator it = przyciski.begin();
	uint8_t ilePrzyciskow = static_cast <uint8_t> (przyciski.size());
	for(uint8_t i = 1; i <= ilePrzyciskow; i ++)
	{
		sf::Vector2f pozycjaPrzycisku = it -> getPosition();
		sf::Vector2f rozmiarPrzycisku = it -> getSize();
		if(pozycjaWJakaKliknalGracz.x >= pozycjaPrzycisku.x
		&& pozycjaWJakaKliknalGracz.x <= pozycjaPrzycisku.x + rozmiarPrzycisku.x
		&& pozycjaWJakaKliknalGracz.y >= pozycjaPrzycisku.y
		&& pozycjaWJakaKliknalGracz.y <= pozycjaPrzycisku.y + rozmiarPrzycisku.y)
		{
			return i;
		}
		it ++;
	}
	return 0;	//Nieklikniêto w ¿aden przycisk.
}

void GraficznyInterfejsUzytkownika::UstawKolorObiektuTekstu(uint8_t && numerObiektuTekstu, sf::Color && kolorDlaTekstu)
{
	teksty[numerObiektuTekstu - 1].setFillColor(kolorDlaTekstu);
}

void GraficznyInterfejsUzytkownika::UstawLosowaPozycjeRudy(float && losowaPozycjaX, float && losowaPozycjaY)
{
	if(losowaPozycjaX >= 370.f)
	{
		losowaPozycjaX -= 30.f;
	}
	sf::Vector2f losowaPozycjaRudy({losowaPozycjaX, losowaPozycjaY + 370.f});
	przyciski.back().setPosition(losowaPozycjaRudy);
}

void GraficznyInterfejsUzytkownika::UstawSamTekstDlaDodatkowegoTekstu(std::string && nowyTekst)
{
	tekstDlaProstokataDoWyswietlaniaInformacji.setString(nowyTekst);
}

void GraficznyInterfejsUzytkownika::UstawTekstObiektuTekstu(uint8_t && numerObiektuTekstu, std::string nowyTekst)
{
	teksty[numerObiektuTekstu - 1].setString(nowyTekst);
}

void GraficznyInterfejsUzytkownika::UstawWlasciwosciDlaDodatkowegoTekstu(std::string && tekst,
sf::Vector2f && pozycjaTekstu, sf::Color && kolorDlaTekstu)
{
	tekstDlaProstokataDoWyswietlaniaInformacji.setFillColor(kolorDlaTekstu);
	tekstDlaProstokataDoWyswietlaniaInformacji.setPosition(pozycjaTekstu);
	tekstDlaProstokataDoWyswietlaniaInformacji.setString(tekst);
}

void GraficznyInterfejsUzytkownika::UtworzDodatkoweElementy()
{
	prostokatDoWyswietlaniaInformacji.setFillColor(sf::Color(128, 128, 128));
	prostokatDoWyswietlaniaInformacji.setPosition({20.f, 20.f});
	prostokatDoWyswietlaniaInformacji.setSize({360.f, 40.f});
	tekstDlaProstokataDoWyswietlaniaInformacji.setCharacterSize(32);
	tekstDlaProstokataDoWyswietlaniaInformacji.setOutlineColor(sf::Color::Black);
}

void GraficznyInterfejsUzytkownika::UtworzObiektyTekstow
(size_t && ileObiektowTekstu, std::string tekstyDlaObiektowTekstu[])
{
	if(teksty.capacity() != 10)
	{
		teksty.reserve(10);
	}
	teksty.clear();
	sf::Vector2f pozycjaObiektuTekstu({25.f, 104.f});
	for(size_t i = 0; i < ileObiektowTekstu; i ++)
	{
		teksty.emplace_back(czcionka);
		teksty.back().setCharacterSize(32);
		teksty.back().setFillColor(sf::Color::Black);
		teksty.back().setOutlineColor(sf::Color::White);
		teksty.back().setPosition(pozycjaObiektuTekstu);
		teksty.back().setString(tekstyDlaObiektowTekstu[i]);
		pozycjaObiektuTekstu.y += 80.f;
	}
}

void GraficznyInterfejsUzytkownika::UtworzPrzyciski(size_t && ilePrzyciskow)
{
	if(ilePrzyciskow <= przyciski.size())
	{
		ilePrzyciskow = (przyciski.size() - ilePrzyciskow);	//Ile przycisków ma byæ usuniêtych.
		for(size_t i = 0; i < ilePrzyciskow; i ++)	//Zmniejszanie iloœci przycisków do tylu, ile zosta³o podane.
		{
			przyciski.pop_back();
		}
		return;
	}
	ilePrzyciskow -= przyciski.size();
	sf::Vector2f rozmiarPrzycisku({360.f, 40.f});
	sf::Vector2f pozycjaPrzycisku({20.f, 100.f + (przyciski.size() * 80.f)});
	while(ilePrzyciskow --)
	{
		przyciski.emplace_back();
		przyciski.back().setFillColor(sf::Color(153, 102, 255));	//Fioletowy.
		przyciski.back().setPosition(pozycjaPrzycisku);
		przyciski.back().setSize(rozmiarPrzycisku);
		pozycjaPrzycisku.y += 80.f;
	}
}

void GraficznyInterfejsUzytkownika::UtworzRudeZOstatniegoPrzycisku()
{
	przyciski.back().setSize({30.f, 30.f});
	przyciski.back().setFillColor(sf::Color(71, 71, 107));
}

void GraficznyInterfejsUzytkownika::ZmienWysokoscPrzycisku(uint8_t && numerPrzycisku, float && nowaWysokosc)
{
	numerPrzycisku --;
	auto iteratorNaPrzycisk = przyciski.begin();
	for(uint8_t i = 0; i < numerPrzycisku; i ++)
	{
		iteratorNaPrzycisk ++;
	}
	sf::Vector2f pozycjaNastepnychPrzyciskow = iteratorNaPrzycisk -> getPosition();
	iteratorNaPrzycisk -> setSize({360.f ,nowaWysokosc});
	iteratorNaPrzycisk ++;
	numerPrzycisku ++;
	pozycjaNastepnychPrzyciskow.y += nowaWysokosc;
	pozycjaNastepnychPrzyciskow.y += 40.f;
	while(iteratorNaPrzycisk != przyciski.end())	//Zmiana pozycji nastêpnych elementów.
	{
		iteratorNaPrzycisk -> setPosition(pozycjaNastepnychPrzyciskow);
		teksty[numerPrzycisku].setPosition({pozycjaNastepnychPrzyciskow.x + 5.f, pozycjaNastepnychPrzyciskow.y + 4.f});
		pozycjaNastepnychPrzyciskow.y += iteratorNaPrzycisk -> getSize().y + 40.f;
		iteratorNaPrzycisk ++;
		numerPrzycisku ++;
	}
}