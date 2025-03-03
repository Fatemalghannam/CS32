//
//  Game.hpp
//  project1
//
//  Created by Fatimeh Al Ghannam on 4/5/24.
//

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include <stdio.h>
class City;
class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nTooters);
    ~Game();

        // Mutators
    void play();

  private:
    City* m_city;
};
#endif /* GAME_INCLUDED */
