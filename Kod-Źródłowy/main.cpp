#include "header.hpp"

int main()
{
	bool czyKoniecGry = false;
	char wybor = 0;
	while(!czyKoniecGry)
	{
		Lobby();
		wybor = _getch();
		system("cls");
		switch(wybor)
		{
			case '1':
			{
				Gra();
				break;
			}
			case '2':
			{
				czyKoniecGry = true;
				break;
			}
			default:
			{
				std::cout << "Dokonano zlego wyboru" << '\n';
				break;
			}
		}
	}
}