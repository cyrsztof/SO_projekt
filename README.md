# SO_projekt
my university multithreading project for Operating System using pthreads and ncurses in c++ for Linux

kryteria zaliczenia projektu:
Celem projektu jest opracowanie prostej wielowątkowej aplikacji dla systemu Linux, działającej i wizualizującej w środowisku konsolowym swoje działanie. Aplikacja może mieć przeznaczenie praktyczne, ale nie musi - program może po prostu symulować z grubsza jakieś zjawiska czy zachowania, być prostą grą, albo narzędziem do czegoś [temat nie będzie bezpośrednio oceniany, ale nie powinny się one powtarzać]. Proszę zwrócić uwagę, iż zależnie od wyboru tematu, użyteczne może być wprowadzenie losowych opóźnień (np. funkcją usleep()) w celu lepszego zaprezentowania działania opracowanego programu. Dozwolone jest korzystanie z dodatkowych bibliotek, jeśli mają one pomóc w zrealizowaniu czegoś (na przykład czytania plików konfiguracyjnych, obsługi połączeń sieciowych, czy realizacji innych złożonych operacji). Celem zadania jest nauczenie się, jak poprawnie stosować wątki w aplikacjach.

Warunki konieczne: 
- wykorzystanie języka C lub C++ 
- wykorzystanie biblioteki pthreads lub std::thread ze standardu C++11 [do obsługi wątków] 
- wykorzystanie biblioteki ncurses [do budowy interfejsu] 
- wykorzystanie przynajmniej dwóch wątków 
- obecność przynajmniej jednej tak zwanej sekcji krytycznej 
- zaprezentowanie (omówienie) działającej aplikacji na zajęciach lub konsultacjach 
- dostarczenie kodu oraz skróconej instrukcji/sprawozdania w formie pliku tekstowego: opis co robi program, jak skompilować i uruchomić

Dodatkowo: (zaczynamy od 2.0 za coś, co spełnia wyżej przedstawione założenia; każdy poniższy punkt liczymy co najwyżej raz, jeśli poprawnie i sensownie go zrealizowano, chyba że wskazano inaczej; do maksymalnej oceny 5.0 za projekt; lista może ulec ewentualnym drobnym korektom - o czym poinformuję na zajęciach) 
- wykorzystanie muteksów (semaforów binarnych) [+0.25] 
- wykorzystanie zmiennych warunkowych [+0.25] 
- tworzenie wątków (zgodnie ze sztuką, z przekazaniem parametrów lub bez) [+0.25] 
- niszczenie wątków (zgodnie ze sztuką, asynchronicznie lub nie) [+0.25] 
- wykorzystanie funkcji finalizujących w niszczonych wątkach [+0.25] 
- wykorzystanie semaforów zliczających (własnych lub systemowych /semaphore.h/) [+0.25] 
- własna implementacja czegoś zachowującego się analogicznie jak semafory zliczające [+0.25] 
- wprowadzenie elementu synchronizacji między wieloma wątkami [+0.25] 
- zaimplementowanie obsługi klawiatury (np. przycisk q - zamyka program; przycisk p - pauza) [+0.25] 
- utworzenie prostego interfejsu (względnie statycznego, np. zwykłe opisy stanów) [+0.25] 
- utworzenie złożonego interfejsu (wprowadzenie dynamiki/namiastek animacji) [+0.25] 
- wprowadzenie przemyślanych elementów kolorystycznych w interfejsie [+0.25] 
- zabezpieczenie przed rysowaniem w aplikacji przez kilka wątków w tym samym czasie [+0.25] 
- zabezpieczenie aplikacji przed możliwością zakleszczenia się (deadlock) [+0.25] 
- zabezpieczenie aplikacji przed możliwością zagłodzenia jakiegoś wątku [+0.25] 
- zabezpieczenie się przed możliwymi wyciekami pamięci i odwołaniami do niezainicjowanych zmiennych (polecam program valgrind)* [+0.25] 
- utworzenie pliku Makefile, aby ułatwić kompilację, testowanie i czyszczenie środowiska [+0.25] 
- utworzenie sprawozdania jako pliku tekstowego w formacie markdown (rozszerzenie .md)** [+0.25] 
- nietuzinkowe rozwiązanie, ciekawe spostrzeżenie [bonus +0.25 za każde wskazanie, maksymalnie dwa brane pod uwagę]
* drobna uwaga co do wycieków z ncurses: http://invisible-island.net/ncurses/ncurses.faq.html… 
** możliwe jest wtedy generowanie PDFa: https://kevin.deldycke.com/…/…/how-to-generate-pdf-markdown/
