//
//  Game.cpp
//  proj3
//
//  Created by Fatimeh Al Ghannam on 5/11/24.
//

// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
using namespace std;

Game::Game(int goblinSmellDistance)
{   currentLevel = -1;      //generate level always adds 1 so it will become 0
    m_goblinSmellDistance = goblinSmellDistance;
    player = new Player;
    generateLevel();
    placeStaircase();
    player->assignGame(this);
    displayLevel();
    //numGameObjects = randInt(1, 4);
}
Game::~Game()
{
    for(int i=0; i<numMonsters; i++)
        delete monsters[i];
    for(int j=0; j<numGameObjects; j++)
        delete gameObjs[j];
}
void Game:: generateLevel()
{
    rooms.clear();                  //clear rooms from previous level genration
        // Fill the entire level with walls
                for (int i = 0; i < levelHeight; ++i) {
                    for (int j = 0; j < levelWidth; ++j) {
                        levelMap[i][j] = '#';
                    }
                }

                // Generate rooms
                for (int i = 0; i < numRooms; ++i) {
                    // Randomly generate room dimensions
                    int width = randInt(5, levelWidth / 3); // Adjust the range as per your requirement
                    int height = randInt(5, levelHeight / 3); // Adjust the range as per your requirement

                    // Randomly generate room position (within the bounds of the level)
                    int x = randInt(1, levelWidth - width - 1);
                    int y = randInt(1, levelHeight - height - 1);

                    // Update level map with room
                    for (int k = y; k < y + height; ++k) {
                        for (int l = x; l < x + width; ++l) {
                            levelMap[k][l] = ' ';
                        }
                    }

                    // Store room information in the rooms array
                    rooms.push_back(Room{x, y, width, height});
                }

                // Generate corridors
                for (int i = 0; i < numRooms - 1; ++i) {
                    // Get center coordinates of the current room
                    int startX = rooms[i].x + rooms[i].width / 2;
                    int startY = rooms[i].y + rooms[i].height / 2;

                    // Get center coordinates of the next room
                    int endX = rooms[i + 1].x + rooms[i + 1].width / 2;
                    int endY = rooms[i + 1].y + rooms[i + 1].height / 2;

                    // Draw horizontal corridor
                    for (int x = std::min(startX, endX); x <= std::max(startX, endX); ++x) {
                        levelMap[startY][x] = ' ';
                    }

                    // Draw vertical corridor
                    for (int y = std::min(startY, endY); y <= std::max(startY, endY); ++y) {
                        levelMap[y][endX] = ' ';
                    }

                    // Add a connecting corridor between the horizontal and vertical parts
                    levelMap[startY][endX] = ' ';
                }

                currentLevel++;
                createMonsters();
                createGameObjects();
    
    }
void Game::displayLevel()
{
        // First display the level layout
        for (int i = 0; i < levelHeight; ++i) {
            for (int j = 0; j < levelWidth; ++j) {
                std::cout << levelMap[i][j];
            }
            std::cout << std::endl;
        }

        // Display player stats
        int h, a, s, d, sl;
        player->stats(h,a,s,d,sl);
        std::cout << "Level: " << currentLevel << ", Hit points: " << h
        << ", Armor: " << a << ", Strength: " << s << ", Dexterity: " << d << std::endl;
}
void Game::placeStaircase() {
    int x, y;
    bool placed = false;
    while (!placed) {
        x = randInt(1, levelHeight - 1);
        y = randInt(1, levelWidth - 1);
        if (showCell(x, y) == ' ') {
            updateCell(x, y, '>');
            staircaseX = x;
            staircaseY = y;
            placed = true;
        }
    }
}

void Game::placeGoldenIdol() {
    int x, y;
    bool placed = false;
    while (!placed) {
        x = randInt(1, levelHeight - 1);
        y = randInt(1, levelWidth - 1);
        if (showCell(x, y) == ' ') {
            updateCell(x, y, '&');
            goldenIdolX = x;
            goldenIdolY = y;
            placed = true;
        }
    }
}
void Game::nextLevel() {
    if (currentLevel < 4) {
        generateLevel();
        player->assignGame(this);
    }
    
    if (isLastLevel())
        placeGoldenIdol();
    else
        placeStaircase();
}
char Game::showCell(int a, int b) const
{
    if(a< 0 || a>=levelHeight || b<0 || b>=levelWidth)
        return '!';
    char c = levelMap[a][b];
    return c;
}
bool Game::updateCell(int a, int b, char i)
{
    if (a < 0 || a >= levelHeight || b < 0 || b >= levelWidth || levelMap[a][b] == '#')
            return false;
        levelMap[a][b] = i;
        return true;
}
void Game::createMonsters()
{  {  int i = 0;
    while(i < randInt(2, 5*(currentLevel+1)+1))
     {
         switch(currentLevel){   //to ensure we follow rules of which monsters can appear in which levels
             case 4:
             case 3:
                 if(trueWithProbability(0.3)){           //so the monsters come in random frequency
                     Actor* monster = new Dragon;
                     monsters[i]= monster;
                     monster->assignGame(this);          //assigns the actor's position and game pointer
                     i++;
                     break;
                    }
             case 2:
                 if(trueWithProbability(0.25)){
                     Actor* monster1 = new Bogeyman;
                     monsters[i] = monster1;
                     monster1->assignGame(this);
                     i++;
                     break;
                 }
             case 1:
             case 0:
                 if(trueWithProbability(0.5)){
                     Actor* monster2 = new Snakewoman;
                     monsters[i] = monster2;
                     monster2->assignGame(this);
                     i++;
                     break;
                     }
                 if(trueWithProbability(0.5)){
                     Actor* monster3 = new Goblin(m_goblinSmellDistance);
                     monsters[i] = monster3;
                     monster3->assignGame(this);
                     i++;
                     break;
                     }
             default:
                 break;
         }
     }
     monsters[i]= nullptr;
     numMonsters = i;
 }
}
void Game::removeDeadMonsters() {
    for (int i = 0; i < numMonsters; ++i) {
        if ((monsters[i]->isDead())) {
            std::cout<< "dealing a final blow." << std::endl;
            monsters[i]->dropObject();
            delete monsters[i]; // Free the memory
            // Shift the rest of the array down
            for (int j = i; j < numMonsters - 1; ++j) {
                monsters[j] = monsters[j + 1];
            }
            monsters[numMonsters - 1] = nullptr; // Clear the last slot
            --numMonsters; // Decrement the count
            --i; // Adjust the index to re-check the current position
        }
    }
}

Actor* Game::monsterHere(int a, int b) const
{
    Actor* res = nullptr;
    for(int i=0; i< this->numMonsters; i++)
    {int x, y;
        (this->monsters[i])->pos(x, y);
        if(x==a && y==b)
          res = monsters[i];
    }
    return res;
}

void Game::monstersMove()
{   int playerX, playerY, x, y;
    player-> pos(playerX, playerY);
    for (int i=0; i<numMonsters; i++){
        int attackerPoints = monsters[i]->getDeterity() + monsters[i]->getWeapon()->getDexterity();
        int defenderPoints = player->getDeterity() + player ->getArmor();
        int damage = randInt(0, monsters[i]->getStrength() + monsters[i]->getWeapon()->getDamage() - 1);
        monsters[i] -> pos(x, y);
        if(monsters[i] ->getSleepTime() >0)             //if asleep, do nothing and decrease sleep time
            monsters[i] ->slep(monsters[i] ->getSleepTime() - 1);
        else if ((x== playerX && (y-1==playerY || y+1==playerY)) || (y== playerY && (x-1==playerX || x+1==playerX)))
        {       //if adjacent to player, attempt attack
            if(monsters[i] ->getWeapon()->objectName() =="magic fang of sleep" && trueWithProbability(0.2))
                player->slep(std::max(randInt(2, 6), player->getSleepTime()));  //when using magic fang of sleep, put player to sleep
            std::cout<< monsters[i]-> getName() << " " << monsters[i]->getWeapon()->getActionString() << player-> getName() << " and ";
            if(randInt(1, attackerPoints) >= randInt(1, defenderPoints))
            {player-> getAttacked(damage);
                std:: cout << "hits."<< std::endl;}
            else
                std:: cout << "misses."<< std::endl;
        }
        else       //else move toward player (move() will not move if Player too far to smell)
            monsters[i]->move();
        
    }
}
void Game::createGameObjects()
{

    for (int i = 0; i < numGameObjects; ++i) {
        int x, y;
        do {
            x = randInt(1, levelHeight - 2);
            y = randInt(1, levelWidth - 2);
        } while (levelMap[x][y] != ' '); // Find an empty space to place the game object

            GameObject* object;
            int objectType = randInt(0, 6);
            switch (objectType) {
                case 0:
                    object = new Weapon("mace", x, y, this);
                    break;
                case 1:
                    object = new Weapon("short sword", x, y, this);
                    break;
                case 2:
                    object = new Weapon("long sword", x, y, this);
                    break;
                case 3:
                    object = new Scroll("scroll of enhance health", x, y, this);
                    break;
                case 4:
                    object = new Scroll("scroll of improve armor", x, y, this);
                    break;
                case 5:
                    object = new Scroll("scroll of raise strength", x, y, this);
                    break;
                case 6:
                    object = new Scroll("scroll of enhance dexterity", x, y, this);
                    break;
                default:
                    object = nullptr;
            }
    
            gameObjs[i] = object;
            levelMap[x][y] = object->symbol(); // Update the level map with the object's symbol
        }
}
void Game::addGameObject(GameObject* object)    //needed for killing monster drops
{
    gameObjs[numGameObjects] = object;
    numGameObjects++;
}
GameObject* Game::objectHere(int a, int b) const {      //needed to redisplay object after Actor movement
    for (int i = 0; i < numGameObjects; ++i) {
        if (gameObjs[i] && gameObjs[i]->getX() == a && gameObjs[i]->getY() == b) {
            return gameObjs[i];
        }
    }
    return nullptr;
}

void Game::removeObject(int a, int b) {
    for (int i = 0; i < numGameObjects; ++i) {
        if (gameObjs[i] && gameObjs[i]->getX() == a && gameObjs[i]->getY() == b) {
            gameObjs[i] = nullptr;
            break;
        }
    }
    updateCell(a, b, '@'); // Update cell to player
}
void Game::gameOver()
{
    if(player->isDead())
    {
        std::cout << "dealing the final blow." << std::endl;
        exit(0);
    }
}
void Game::play()
{
    cout << "Press q to exit game." << endl;
        
        char c = ' ';
        while ((c = getCharacter()) && c != 'q') {
            if(player->getSleepTime()==0){      //player allowed to move
                if (c == 'h' || c == 'l' || c == 'k' || c == 'j' || c == '>' || c == '&' || c == 'c') {
                    player->move(c);
                } else if (c == 'g') {
                    int a, b;
                    player->pos(a, b);
                    if (objectHere(a, b) != nullptr)
                        player->pickUp(objectHere(a, b));
                } else if (c == 'i') {
                    player->displayInventory();
                }
                else if (c=='w')
                    player->wieldWeapon();
                else if (c== 'r')
                    player->readScroll();
            }
            else
                player->slep(player->getSleepTime()-1);
            monstersMove();
            clearScreen();
            displayLevel();
        }
        
}
