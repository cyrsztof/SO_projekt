//
// Created by krzysztof-c on 17.06.17.
//

#ifndef SO_PROJEKT_FOOD_H
#define SO_PROJEKT_FOOD_H
#include "stdlib.h"


class food {
public:
    int foodPositionX;
    int foodPositionY;
    food (
            int foodPositionX = 0,
            int foodPositionY = 0
    )
    {

        foodPositionX = rand()%10;
        foodPositionY = rand()%10;

        this->foodPositionX = foodPositionX;
        this->foodPositionY = foodPositionY;
    }

};


#endif //SO_PROJEKT_FOOD_H
