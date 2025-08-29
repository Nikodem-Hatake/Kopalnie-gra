#pragma once
#ifndef header_hpp
#define header_hpp

#include <array>
#include <condition_variable>
#include <forward_list>
#include <fstream>
#include <future>
#include <list>
#include <mutex>
#include <random>
#include <SFML/Graphics.hpp>
#include <thread>
#include <vector>

#include "Pracownik.hpp"
#include "Kopalnia.hpp"
#include "Gracz.hpp"
#include "GraficznyInterfejsUzytkownika.hpp"

void Gra();
void KopanieSamemu();
uint64_t ZamienStringaNaLiczbe(std::string & liczba);

void ObslugaKopalni();
void ObslugaPracownika(Pracownik * pracownik);

bool Odczyt();
void Zapis();
bool ZapisAutoSave();

#endif