//
//  Game.hpp
//  proj3
//
//  Created by Fatimeh Al Ghannam on 5/11/24.
//

// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

// You may add data members and other member functions to this class.

#include <iostream>
#include <vector>
#include "Actor.h"
#include "GameObject.h"
#include "utilities.h"

const int levelWidth = 70; // Width of the level
const int levelHeight = 18; // Height of the level

class Game
{
public:
    Game(int goblinSmellDistance);
    ~Game();
    void play();// Function to start and run the game
    
    void nextLevel();
    void placeStaircase();
    void placeGoldenIdol();
    void displayLevel(); // Function to display the current level
    void gameOver(); // Function to check if the game is over
    Actor* monsterHere(int a, int b) const;
    char showCell(int a, int b) const;
    bool updateCell(int a, int b, char i);
    void removeDeadMonsters();
    void monstersMove();
    void createGameObjects();
    void addGameObject(GameObject* object);
    GameObject* objectHere(int a, int b) const;
    void removeObject(int a, int b);
    void playerPos(int& a, int& b){player->pos(a,b);}
    bool isLastLevel() const { return currentLevel == 4; }
    bool isStaircasePosition(int x, int y) const { return x == staircaseX && y == staircaseY; }
    bool isGoldenIdolPosition(int x, int y) const { return x == goldenIdolX && y == goldenIdolY; }
private:
        Player* player; // Player character
        Actor* monsters[27]; // Array of monster characters, at most 26 monsters per level, dead monsters are null
        int numMonsters = 0;
        GameObject* gameObjs[10];       //Bigger to account for dropped objects
        int numGameObjects = randInt(1, 4);
        int staircaseX, staircaseY;
        int goldenIdolX, goldenIdolY;
        int m_goblinSmellDistance;
        void createMonsters(); // Create monsters for the current level

    int currentLevel;
    char levelMap[levelHeight][levelWidth]; // Store the layout of the level

    //Level stuff
    void generateLevel();
    struct Room {
           int x; // Top-left corner x-coordinate
           int y; // Top-left corner y-coordinate
           int width; // Width of the room
           int height; // Height of the room
       };

       std::vector<Room> rooms; // Vector to store rooms
       int numRooms = randInt(2, 6); // Number of rooms generated
};

#endif // GAME_INCLUDED
