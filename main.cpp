#include <iostream>
#include <unistd.h>
#include "locale.h"
#include <ncursesw/ncurses.h>
#include "time.h"
#include <stdlib.h>
#include <string.h>
#include "string"
#include "vector"
#include <sstream>

namespace patch         //small patch for std::string, since it wasn't working with my compiler
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


int map[9][9];          //0 - nothing, 1 - ant, 2 - food, ...
void boxAround( int y, int x, int h, int w );
void printMap(int mapCornerWidth, int mapCornerHeight);
class ant {
    int age;
    bool food;
    int gender;
    int antPositionX;
    int antPositionY;

public:
    ant (int age = 0,    // ant is aging
         bool food = 0,  // ant collects food
         int gender = 0, // 0 female, 1 male
         int antPositionX = 0,
         int antPositionY = 0
        )
    {
        gender = rand() %2;
        antPositionX = rand()%10;
        antPositionY = rand()%10;

        this->age = age;
        this->food = food;
        this->gender = gender;
        this->antPositionX = antPositionX;
        this->antPositionY = antPositionY;
    }

    int getAge() {
        return age;
    }

    bool hasFood() {
        return food;
    }

    int getGender() {
        return gender;
    }

    int getPositionAntX() {
        return antPositionX;
    }
    int getPositionAntY() {
        return antPositionY;
    }

    void addFood() {
        food = 1;
    }
    void removeFood() {
        food = 0;
    }

};
std::vector <ant> antsVector;
void startMenu();
int width = 0;
int height = 0;


int main (void) {
    srand(time(NULL));
    setlocale(LC_ALL, "");      //utf-8 for polish characters

    for (int i =0; i<4; i++) {
        ant mrowka;
        antsVector.push_back(mrowka);
    }
    startMenu();


    return 0;
}

void startMenu() {

    initscr();
    getmaxyx(stdscr, height, width);    //for scaling interface
    int mapCornerWidth = width/2-25;
    int mapCornerHeight = height/2-15;

    if (has_colors()) { //sprawdzenie, czy konsola obsługuje kolory
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        attron(COLOR_PAIR(1));

        printMap(mapCornerWidth,mapCornerHeight);


        for (int k =0;k<4;k++) {
            if (antsVector[k].getGender() == 1) {
                mvprintw(mapCornerHeight+1+antsVector[k].getPositionAntY()*3, mapCornerWidth+5*antsVector[k].getPositionAntX()+2,"f");
            } else if (antsVector[k].getGender() == 0) {
                mvprintw(mapCornerHeight+1+antsVector[k].getPositionAntY()*3, mapCornerWidth+5*antsVector[k].getPositionAntX()+2,"m");
            }

        }
        //ant mrowka;
        //mrowka.addFood();
        //std::string tekst = patch::to_string(mrowka.getPositionAntX());
        //mvprintw(mapCornerHeight +1,mapCornerWidth+2 , tekst.c_str());


        getch();
        endwin();
        attroff(COLOR_PAIR(1));

    } else printf("\nTwoja konsola nie obsługuje kolorów\n");

    getch();
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


    for (int i = 0;i<10;i++) {
        for (int j = 0; j<10;j++) {
            boxAround(mapCornerHeight+i*3,mapCornerWidth+j*5,1,3);
        }
    }
}