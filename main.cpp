
#include "food.h"
#include "ant.h"
#include <functional>



int width = 0;
int height = 0;
int speed = 1;
int dead = 0;
int mapCornerWidth = 0;
int mapCornerHeight = 0;
int map[10][10];          //0 - nothing, 1 - ant female, 2 - ant male, 3 - male + female, 4 - more ants, -1 - food...
char c;
int counter = 0;
int bless = 1;
int kill = -1;
std::vector <ant> antsVector;
std::vector <food> foodsVector;

void boxAround( int y, int x, int h, int w );
void printMap(int mapCornerWidth, int mapCornerHeight);
void printAnts (int mapCornerHeight, int mapCornerWidth);

void initial();
void startMenu();
void countAnts();
void createFood();
void moveAnts(std::vector <ant> &antsVector);

void* blessOrKill(void *arg);
void* mainGame(void *);

bool threadAddingStart = false;
bool threadRemovingStart = false;
pthread_t threadMain;
pthread_t threadAdding;
pthread_t threadRemoving;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //mutex



int main (void) {

    srand(time(NULL));          //for rand() function
    setlocale(LC_ALL, "");      //utf-8 for polish characters

    startMenu();
    initial();

    pthread_create(&threadMain, NULL, mainGame, NULL); //main thread


    pthread_join(threadMain, NULL);
    pthread_join(threadAdding, NULL);
    pthread_join(threadRemoving, NULL);

    return 0;
}
void* blessOrKill(void *arg) {
    int choice = *(int *) arg;

   for (int i = 0; i<100; i++) {
       pthread_mutex_lock(&mutex);
       counter ++;
       pthread_mutex_unlock(&mutex);

       if (choice == -1&&antsVector.size()>0) {

           antsVector.erase(antsVector.begin());
       } else if (choice == 1){

           bool check = true;
           while (check) {
               ant ant;
               if (map[ant.antPositionX][ant.antPositionY] == 0) {
                   check = false;
                   map[ant.antPositionX][ant.antPositionY] = ant.gender + 1;
                   antsVector.push_back(ant);
               }
           }
       }
       sleep(1);
   }

    pthread_exit(NULL);

}
void* mainGame(void *) {

    erase();
    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);     //colors for printing in ncurses
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);

    getmaxyx(stdscr, height, width);    //for scaling interface
    mapCornerWidth = width / 2 - 25;
    mapCornerHeight = height / 2 - 15;

    refresh();


    timeout(0);
    while (true) {

        usleep(1000000 / speed);
        moveAnts(antsVector);
        erase();
        printMap(mapCornerWidth, mapCornerHeight);
        countAnts();
        printAnts(mapCornerHeight, mapCornerWidth);
        refresh();

        move (0,0);
        bool check2 = true;

        c = getch();
        if (c == 'q') {
            endwin();
            pthread_cancel(threadMain);
            pthread_cancel(threadAdding);
            pthread_cancel(threadRemoving);
            break;
        } else if (c == 'w') {
            createFood();
        } else if (c == '\033') { // if the first value is esc
            getch(); // skip the [
            switch (getch()) { // the real value
                case 'A':
                    //miracle
                    if(threadAddingStart) {
                        threadAddingStart = false;
                        pthread_cancel(threadAdding);
                    } else {
                        threadAddingStart = true;
                        pthread_create(&threadAdding, NULL, blessOrKill, &bless );
                    }
                    break;

                case 'B':
                    //annihilation
                    if (threadRemovingStart) {
                        threadRemovingStart = false;
                        pthread_cancel((threadRemoving));
                    } else {
                        threadRemovingStart = true;
                        pthread_create(&threadRemoving, NULL, blessOrKill, &kill);
                    }
                    break;

                case 'C':
                    // code for arrow left
                    speed++;
                    if (speed > 9) {
                        speed = 10;
                    }
                    break;
                case 'D':
                    // code for arrow right
                    speed--;
                    if (speed < 2) {
                        speed = 1;
                    }
                    break;
            }
            refresh();

        } else if (c == 'e') {
            for (int i = 0; i < antsVector.size(); i++) {
                dead++;
            }
            antsVector.clear();
        } else if (c == 'r' && antsVector.size() > 0) {
            antsVector.erase(antsVector.begin());
}
            init_pair(1, COLOR_GREEN, COLOR_BLACK);
            for (int i =0; i < antsVector.size();i++) {
                int j =0;
                if (i > height-1) {
                    j = 1;
                }
                attron(COLOR_PAIR(1));
                mvprintw(i%height, 1+j*20, "Wiek mrówki %u: %u", i,antsVector[i].age);
                attroff(COLOR_PAIR(1));
                refresh();
            }
        refresh();

        attron(COLOR_PAIR(2));
            mvprintw(1, width-15,"Jedzenie: %u", foodsVector.size() );
            mvprintw(2, width-15,"Mrówki:   %u", antsVector.size() );
            mvprintw(3, width-15,"Zmarło:   %u", dead);
            mvprintw(5, width-18,"Święty licznik: %u", counter);
            refresh();
            attroff(COLOR_PAIR(2));

        }
        timeout(10000);
        endwin();

}
void startMenu() {

    initscr();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);     //colors for printing in ncurses
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);

    getmaxyx(stdscr, height, width);    //for scaling interface
    mapCornerWidth = width/2-25;
    mapCornerHeight = height/2-15;
    int movingFont = 0;
    int i = 0;
    if (has_colors()) { //checking if console can use colors
        timeout(0);
        char text[] = "Obudziłeś się w cudownym świecie...";
        char text2[] = "Pełnym mrówek...";
        char text3[] = "Ty tu jesteś Bogiem...";
        char text4[] = "Panda trzy...";
        while (i<15) {
            erase();
            attron(COLOR_PAIR(2));
            if (movingFont < 4){
                mvprintw((height / 2) + movingFont % 4, (width / 2 - (sizeof(text) / 2)) - movingFont % 4, text);
            } if (movingFont > 3 && movingFont < 8) {
                mvprintw((height / 2)+movingFont%4, (width / 2 - (sizeof(text2) / 2))-movingFont%4, text2);
            } if (movingFont > 7 && movingFont < 12) {
                mvprintw((height / 2)+movingFont%4, (width / 2 - (sizeof(text3) / 2))-movingFont%4, text3);
            } if (movingFont > 11) {
                mvprintw((height / 2)+movingFont%4, (width / 2 - (sizeof(text4) / 2))-movingFont%4, text4);
            }

            attroff(COLOR_PAIR(2));
            refresh();
            sleep(1);
            movingFont++;
            i++;
        }
        timeout(40000);
        erase();
        attron(COLOR_PAIR(3));
        char text5[] = "Naciśnij dowolny przycisk, by rozpocząć";
        mvprintw((height / 2), (width / 2 - (sizeof(text5) / 2)), text5);
        attroff(COLOR_PAIR(3));
        getch();
        refresh();
        endwin();

    } else printf("\nTwoja konsola nie obsługuje kolorów\n");


}
void boxAround( int y, int x, int h, int w ) {

    move( y, x );
    addch (ACS_ULCORNER);   // upper left corner
    int j;
    for (j = 0;  j < w;  ++j)
        addch (ACS_HLINE);
    addch (ACS_URCORNER);   // upper right

    for( j = 0; j < h; ++j ) {
        move(  y+1+j, x );
        addch (ACS_VLINE);
        move( y+1+j, x+w+1 );
        addch (ACS_VLINE);
    }

    move( y+h+1,x );
    addch (ACS_LLCORNER);   // lower left corner

    for (j = 0;  j < w;  ++j)
        addch (ACS_HLINE);
    addch (ACS_LRCORNER);   // lower right
}
void createFood() {
    bool check = true;
    while (check) {
        food food;
        if (map[food.foodPositionX][food.foodPositionY] == 0) {
            map [food.foodPositionX][food.foodPositionY] = -1;
            foodsVector.push_back(food);
            check = false;
        }
    }
}
void printMap(int mapCornerWidth, int mapCornerHeight) {
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    for (int i = 0;i<10;i++) {
        for (int j = 0; j<10;j++) {
            boxAround(mapCornerHeight+i*3,mapCornerWidth+j*5,1,3);
        }
    }
    attroff(COLOR_PAIR(1));
}
void printAnts (int mapCornerHeight, int mapCornerWidth) {

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);

    for (int i = 0; i <10; i++) {
        for (int j = 0; j < 10; j++) {


            switch ((map[i][j])){
                case (0):
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"");       //blank fields - nothing there
                    break;
                case (1):
                    attron(COLOR_PAIR(5));
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"f");      //female ant
                    attroff(COLOR_PAIR(5));
                    break;
                case (2):
                    attron(COLOR_PAIR(2));
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"m");      //male ant
                    attroff(COLOR_PAIR(2));
                    break;
                case (3):
                    attron(COLOR_PAIR(4));
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"x");      //male ant and female ant
                    attroff(COLOR_PAIR(4));
                    break;
                case (4):
                    attron(COLOR_PAIR(4));
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"&");      //more ants (or 2 of the same gender)
                    attroff(COLOR_PAIR(4));
                    break;
                case (-1):
                    attron(COLOR_PAIR(4));
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"@");      //food
                    attroff(COLOR_PAIR(4));
                    break;
                default:
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"x");
                    break;

            }

        }
    }
}
void initial() {

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            map[i][j] = 0;
        }
    }

    for (int i = 0; i < 20; i++) {         //creating some start food
        createFood();
    }
    for (int i = 0; i < 10; i++) {          //creating some start ants
        bool check = true;
        while (check) {
            ant ant;
            if (map[ant.antPositionX][ant.antPositionY] == 0) {
                check = false;
                map [ant.antPositionX][ant.antPositionY] = ant.gender+1;
                antsVector.push_back(ant);
            }
        }
    }


}
void moveAnts(std::vector <ant> &antsVector){
    for (int k = 0; k < (antsVector).size();k++) {
        antsVector[k].age ++;
        int deadOrAlive = rand() % 30000;
            if (deadOrAlive < antsVector[k].age) {
                dead++;
                map[antsVector[k].antPositionX][antsVector[k].antPositionY] = 0;
                antsVector.erase(antsVector.begin() + k);
            }
        int i = rand()%4;
        switch (i) {
            case(0): //moving up
                if (antsVector[k].antPositionY > 0) {
                    antsVector[k].antPositionY -= 1;
                }
                break;
            case(1): //moving right
                if (antsVector[k].antPositionX < 9) {
                    antsVector[k].antPositionX +=1;
                }
                break;
            case(2): //moving down
                if (antsVector[k].antPositionY <9) {
                    antsVector[k].antPositionY += 1;
                }
                break;
            case(3): ;//moving left
                if (antsVector[k].antPositionX>0) {
                    antsVector[k].antPositionX -= 1;
                }
                break;
        }
    }
}
void countAnts () {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            map[i][j] = 0;
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            for (int m = 0; m <foodsVector.size();m++) {
                if (foodsVector[m].foodPositionX == i && foodsVector[m].foodPositionY == j) {
                    map [i][j] = -1;
                }
            }
            for (int k = 0; k < antsVector.size(); k++) {
                if (antsVector[k].antPositionX == i && antsVector[k].antPositionY == j) {
                    switch(map[i][j]) {
                        case(-1):
                            map[i][j] = antsVector[k].gender + 1;
                            if (antsVector[k].gender == 1 && !antsVector[k].hasFood()) {
                                antsVector[k].addFood();
                                for (int n = 0; n < foodsVector.size(); n++) {
                                    if (foodsVector[n].foodPositionX == i && foodsVector[n].foodPositionY == j) {
                                        foodsVector.erase(foodsVector.begin() + n);
                                    }
                                }
                            }
                            break;
                        case(1):

                            if (antsVector[k].gender == 1 && antsVector[k].hasFood()){
                                antsVector[k].removeFood();
                                ant ant;
                                antsVector.push_back(ant);
                                map[i][j] = 3;
                            } else if (antsVector[k].gender == 1 && !antsVector[k].hasFood()) {
                                map[i][j] = 3;
                            } else
                                map[i][j] = 4;
                            break;
                        case(2):
                            if (antsVector[k].gender == 1){
                                map[i][j] = 4;
                            } else {
                                map[i][j] = 3;
                                for (int l = 0; l < antsVector.size();l++) {
                                    if (antsVector[l].antPositionX == i && antsVector[l].antPositionY == j && antsVector[l].gender == 1 && antsVector[l].hasFood()) {
                                        bool check = true;
                                        if (antsVector.size()+foodsVector.size() >= 100){
                                            check = false;
                                        }
                                        while (check) {
                                            ant ant;
                                            for (int n = 0; n < antsVector.size();n++) {
                                                if (antsVector[n].antPositionX == ant.antPositionX && antsVector[n].antPositionY == ant.antPositionY) {
                                                    continue;
                                                } else {
                                                    antsVector[l].removeFood();
                                                    antsVector.push_back(ant);
                                                    check = false;
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }


                            break;
                        case(3):
                            map[i][j] = 4;
                            break;
                        case(4):
                            map[i][j] = 4;
                            break;
                        default:
                            map[i][j] = antsVector[k].gender+1;
                            break;

                    }

                }
            }


        }
    }

}