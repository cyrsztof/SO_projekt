//
// Created by krzysztof-c on 17.06.17.
//

#ifndef SO_PROJEKT_ANT_H
#define SO_PROJEKT_ANT_H
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
extern int map[10][10];
class ant {

public:
    int age;
    int gender;
    int antPositionX;
    int antPositionY;
    bool food;

    ant (int age = 0, int gender = 0,   int antPositionX = 0,   int antPositionY = 0,   bool food = 0) {

    gender = rand() %2;
    antPositionX = rand() % 10;
    antPositionY = rand() % 10;

    this->age = age;
    this->food = food;
    this->gender = gender;
    this->antPositionX = antPositionX;
    this->antPositionY = antPositionY;
}
    ant (int antPositionX, int antPositionY) {
        this->age = 0;
        this->food = 0;
        this->gender = rand()%2;
        this->antPositionX = antPositionX;
        this->antPositionY = antPositionY;
    }


    void changeAntPositionX(int newAntPositionX) {
    antPositionX = newAntPositionX;
    }

    void changeAntPositionY(int newAntPositionY) {
    antPositionY = newAntPositionY;
    }

    bool hasFood() {
    return food;
    }

    int getGender() {
    return gender;
    }

    void addFood() {
    food = 1;
    }

    void removeFood() {
    food = 0;
    }


};


#endif //SO_PROJEKT_ANT_H
