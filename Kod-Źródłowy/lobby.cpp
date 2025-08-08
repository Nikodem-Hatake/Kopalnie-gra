#include "header.hpp"

void Lobby()
{
	std::cout << std::string(10, '-') << "KOPALNIE" << std::string(10, '-') << '\n';
	std::cout << "1. Rozpocznij gre" << '\n';
	std::cout << "2. Wyjdz" << '\n';
	std::cout << "Dokonaj wyboru: ";
}

void LobbyGry(std::atomic_uint64_t & kasa)
{
	std::cout << std::string(28, '-') << '\n';
	std::cout << "Ilosc pieniedzy: " << kasa << '\n';
	std::cout << std::string(28, '-') << '\n';
	std::cout << "1. Wejdz do wybranej kopalni" << '\n';
	std::cout << "2. Kup kopalnie" << '\n';
	std::cout << "3. Kop samemu" << '\n';
	std::cout << "4. Odswiez ilosc pieniedzy" << '\n';
	std::cout << "5. Wyjdz" << '\n';
	std::cout << "Dokonaj wyboru: ";
}

void LobbyObslugiKopalni(char & numerKopalni, uint64_t && iloscPracownikow, std::atomic_uint64_t & kasa)
{
	std::cout << std::string(28, '-') << '\n';
	std::cout << "Kopalnia " << static_cast <short> (numerKopalni);
	std::cout << ". Ilosc pracownikow: " << static_cast <short> (iloscPracownikow) << '.' << '\n';
	std::cout << "Ilosc pieniedzy: " << kasa << '\n';
	std::cout << "1. Zarzadzaj danym pracownikiem" << '\n';
	std::cout << "2. Kup nowego pracownika dla tej kopalni" << '\n';
	std::cout << "3. Odswiez ilosc pieniedzy" << '\n';
	std::cout << "4. Wyjdz" << '\n';
	std::cout << "Dokonaj wyboru: ";
}

void LobbyObslugiPracownika(std::atomic_uint64_t & kasa)
{
	std::cout << std::string(28, '-') << '\n';
	std::cout << "Ilosc pieniedzy: " << kasa << '\n';
	std::cout << "Wybierz co chcesz z nim zrobic: " << '\n';
	std::cout << "1. Ulepsz ilosc wydowybanych zloz" << '\n';
	std::cout << "2. Ulepsz predkosc kopania w milisekundach" << '\n';
	std::cout << "3. Zmien imie" << '\n';
	std::cout << "4. Zobacz informacje o pracowniku" << '\n';
	std::cout << "5. Odswiez ilosc pieniedzy" << '\n';
	std::cout << "6. Wyjdz" << '\n';
	std::cout << "Dokonaj wyboru: ";
}