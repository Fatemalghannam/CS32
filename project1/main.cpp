//
//  main.cpp
//  project1
//
//  Created by Fatimeh Al Ghannam on 4/5/24.
//

#include <iostream>
#include "Tooter.h"
#include "Player.h"
#include "City.h"
#include "Game.h"
#include "globals.h"
///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}
