
#include "food.h"
#include "ant.h"



int width = 0;
int height = 0;

int mapCornerWidth = 0;
int mapCornerHeight = 0;

int map[10][10];          //0 - nothing, 1 - ant female, 2 - ant male, 3 - male + female, 4 - more ants, 10 - food...

std::vector <ant> antsVector;
std::vector <food> foodsVector;


void boxAround( int y, int x, int h, int w );
void printMap(int mapCornerWidth, int mapCornerHeight);
void printAnts (int mapCornerHeight, int mapCornerWidth);
void moveAnts(ant& ant);
void initial();
void startMenu();
void countAnts();

pthread_t threadPrinting;
pthread_t threadMoving;




int main (void) {

    srand(time(NULL));          //for rand() function
    setlocale(LC_ALL, "");      //utf-8 for polish characters

    initial();
    startMenu();



   // usleep(200000);


    return 0;
}

void startMenu() {

    initscr();
    getmaxyx(stdscr, height, width);    //for scaling interface
    int mapCornerWidth = width/2-25;
    int mapCornerHeight = height/2-15;

    if (has_colors()) { //sprawdzenie, czy konsola obsługuje kolory
        start_color();

        printMap(mapCornerWidth, mapCornerHeight); //printing the map lines
        printAnts(mapCornerHeight, mapCornerWidth);
        refresh();

        timeout(0);
        while (true) {
            sleep(1);

            for (int i = 0; i < antsVector.size(); i++) {
                int deadOrAlive = rand()%10000;
                if (deadOrAlive>antsVector[i].age) {
                    moveAnts(antsVector[i]);
                } else {
                    map[antsVector[i].antPositionX][antsVector[i].antPositionY] = 0;
                    antsVector.erase(antsVector.begin() + i);
                }

            }
            erase();
            printMap(mapCornerWidth, mapCornerHeight);
            countAnts();
            printAnts(mapCornerHeight, mapCornerWidth);

            refresh();
            char c;
            c=getch();
            if (c == 'q') {

                endwin();
                break;
            }
            init_pair(1, COLOR_GREEN, COLOR_BLACK);

            for (int i =0; i < antsVector.size();i++) {
                attron(COLOR_PAIR(1));
                mvprintw(10+i, 10, "Wiek mrówki %u: %u", i,antsVector[i].age);
                attroff(COLOR_PAIR(1));
                refresh();
            }
        }
        timeout(40000);
    } else printf("\nTwoja konsola nie obsługuje kolorów\n");


    endwin();

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

    for (int i = 0; i <10; i++) {
        for (int j = 0; j < 10; j++) {


            switch ((map[i][j])){
                case (0):
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"");       //blank fields - nothing there
                    break;
                case (1):
                    attron(COLOR_PAIR(3));
                    mvprintw(mapCornerHeight+1+i*3, mapCornerWidth+5*j+2,"f");      //female ant
                    attroff(COLOR_PAIR(3));
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
    for (int i = 0; i < 8; i++) {          //creating some start ants
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
void moveAnts(ant& ant) {

    ant.age ++;

    int i = rand()%4;
    switch (i) {
        case(0): //moving up
            if (ant.antPositionY > 0) {
                //map[ant.antPositionX][ant.antPositionY] = 0;

                ant.antPositionY -= 1;
                //map[ant.antPositionX][ant.antPositionY] = ant.gender+1;

            }
            break;
        case(1): //moving right
            if (ant.antPositionX < 9) {
                //map[ant.antPositionX][ant.antPositionY] = 0;

                ant.antPositionX +=1;
               // map[ant.antPositionX][ant.antPositionY] = ant.gender+1;
            }
            break;
        case(2): //moving down
            if (ant.antPositionY <9) {
              //  map[ant.antPositionX][ant.antPositionY] = 0;

                ant.antPositionY += 1;
                //map[ant.antPositionX][ant.antPositionY] = ant.gender+1;

            }
            break;
        case(3): ;//moving left
            if (ant.antPositionX>0) {
               // map[ant.antPositionX][ant.antPositionY] = 0;

                ant.antPositionX -= 1;
               // map[ant.antPositionX][ant.antPositionY] = ant.gender+1;
            }
            break;
    }
    mvprintw(1,1,"RUSZAM SIĘ");
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
                            if (antsVector[k].gender == 1 && antsVector[k].hasFood() == false) {
                                antsVector[k].food = true;
                                for (int n = 0; n < foodsVector.size(); n++) {
                                    if (foodsVector[n].foodPositionX == i && foodsVector[n].foodPositionY == j) {
                                        foodsVector.erase(foodsVector.begin() + n);
                                    }
                                }
                            }
                            break;
                        case(1):

                            if (antsVector[k].gender == 1 && antsVector[k].hasFood() == true){
                                antsVector[k].hasFood() == false;
                                ant ant;
                                antsVector.push_back(ant);
                                map[i][j] = 3;
                            } else if (antsVector[k].gender == 1) {
                                map[i][j] = 3;
                            } else
                                map[i][j] = 4;
                            break;
                        case(2):
                            break;
                        case(3):
                            break;
                        case(4):
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