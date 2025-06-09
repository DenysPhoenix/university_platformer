#include <iostream>
#include "silnik.h"

using namespace std;

int main()
{
	// powołujemy silnik który całą grę nam uruchamia i tylko aktualizuje i wyświetla i do tamtych funkcji wkładamy funkcje które mają działać
	
    Silnik gra;
	while (gra.czyokno())
	{
		gra.aktualizacja();
		gra.wyswietlenie();
	}
	

    return 0;
}


