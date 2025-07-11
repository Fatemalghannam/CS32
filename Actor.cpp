//
//  Actor.cpp
//  proj3
//
//  Created by Fatimeh Al Ghannam on 5/11/24.
//

#include "Actor.h"
#include "utilities.h"
#include "Game.h"
#include <set>
#include <climits>
#include <algorithm>

Actor::Actor(){
    m_game = nullptr;
    isAlive = true;
}
Actor::~Actor()
{
    m_game->updateCell(x, y, ' ');    //remove the symbol from the grid
    delete weapon;
    m_game = nullptr;
}
Actor* Actor::assignGame(Game* game)
{
    m_game = game;
    bool start = true;
    while(start)
    {
        int a = randInt(1, levelHeight-1);
        int b = randInt(1, levelWidth-1);
        if(m_game->showCell(a,b) == ' ')
        {
            if(m_game->updateCell(a, b, symbol()))
            {
                start = false;
                this->x=a; this->y=b; this->isAlive= true;
            }
                
        }
    }

    return this;
}
void Actor::changePos(int a, int b) {x= a; y =b;}

bool Actor::getAttacked(int damage) {
    this->hitPoints= hitPoints - damage;
    if (this->hitPoints <= 0) {
        this->isAlive = false;
        m_game -> gameOver();       //player was the one that died
        m_game->removeDeadMonsters();       //monster was the one that died
        return false;
    }
    return true;
}

void Actor::pos(int& a, int& b) const{
    a = this->x;
    b = this->y;
    return;
}
void Actor::stats(int& h, int& a, int& s, int& d, int& sl) const{ h = this->hitPoints; a = this->armor; s= this->strength; d= this->dexterity; sl = this->sleepTime; return;}


Player::Player()
{   m_game = nullptr;
    MAX_HP = 20;
    hit(MAX_HP); arm(2); str(2); dex(2);
    inventorySize = 1;
    inventory[0] = new Weapon("short sword");
    setWeapon(dynamic_cast<Weapon*>(inventory[0]));
    type("Player");
}
void Player::move(char arrow) {
    if(getSleepTime()==0){
        int hitP = getHitPoints();
        int st = getStrength();
        int dex = getDeterity();
        if (hitP < MAX_HP && trueWithProbability(0.1)) {
            hit(hitP + 1);
        }
        int damage = randInt(0, st + getWeapon()->getDamage() - 1);
        int attackPoints = dex + getWeapon()->getDexterity();
        int x, y;
        pos(x, y);
        
        Actor* M = nullptr;
        int newX = x, newY = y;
        
        switch (arrow) {
            case ARROW_LEFT:
                newY--;
                break;
            case ARROW_RIGHT:
                newY++;
                break;
            case ARROW_UP:
                newX--;
                break;
            case ARROW_DOWN:
                newX++;
                break;
            case 'c':
                this->str(9);
                this->hit(50);
                break;
            case '>':
                if (m_game->isStaircasePosition(x, y)) {
                    m_game->nextLevel();
                    return;
                }
                break;
            case '&':
                if (m_game->isGoldenIdolPosition(x, y)) {
                    std::cout << "Congratulations, you won!" << std::endl;
                    exit(0);
                }
                break;
            default:
                return;
        }
        
        char g = m_game->showCell(newX, newY);
        if (g == '#' || g == '!') {
            return; // Wall or out of bounds, do nothing
        } else if (g == 'S' || g == 'B' || g == 'G' || g == 'D') {
            M = m_game->monsterHere(newX, newY);
        } else {
            // In case the player was standing on an object and did not pick it up
            GameObject* possibleObj = m_game->objectHere(x, y);
            if (m_game->isGoldenIdolPosition(x, y))
                m_game->updateCell(x, y, '&');
            else if (m_game->isStaircasePosition(x, y))
                m_game->updateCell(x, y, '>');
            else if (possibleObj != nullptr)
                m_game->updateCell(x, y, possibleObj->symbol());
            else
                m_game->updateCell(x, y, ' ');
            
            changePos(newX, newY);
            m_game->updateCell(newX, newY, '@');
        }
        
        if (M != nullptr) {
            int defendPoints = M->getDeterity() + M->getArmor();
            std::cout<< this-> getName() << " " << getWeapon()->getActionString() << M-> getName() << " ";
            if(getWeapon()->objectName() =="magic fang of sleep" && trueWithProbability(0.2))       //to sometimes put to sleep
                M->slep(std::max(randInt(2, 6), M->getSleepTime()));
            if(M->getAttacked(damage)){
                if (randInt(1, attackPoints) >= randInt(1, defendPoints)) {
                    std::cout << "and hits." << std::endl;
                } else {
                    std::cout << "and misses." << std::endl;
                }}
        }
    }}

void Player::pickUp(GameObject* object) {
    if (inventorySize >= 26) {
        std::cout << "Your knapsack is full; you can't pick that up." << std::endl;
        return;
    }
    inventory[inventorySize] = object;
    inventorySize++;
    int x, y; pos(x,y);
    if(object->symbol() == ')')
        std::cout << "You pick up " << object->objectName() << std::endl;
    else
        std::cout << "You pick up a scroll called " << object->objectName() << std::endl;
    m_game->removeObject(x, y);
}
void Player::displayInventory() const {
    clearScreen();
    std::cout << "Inventory:" << std::endl;
    for (int i = 0; i < inventorySize; ++i) {
        char label = 'a' + i;
            std::cout << label << ". " << inventory[i]->objectName() << std::endl;
    }
}
void Player::wieldWeapon()
{
    displayInventory();
    char c = getCharacter();
    int n = c - 'a';
    if(n>=0 && n<inventorySize)
    {
        if(inventory[n]->symbol() == ')'){
            setWeapon(dynamic_cast<Weapon*>(inventory[n]));
            std:: cout << "You are wielding " << getWeapon() ->objectName() << std::endl;}
        else
            std::cout << "You can't wield " << inventory[n] -> objectName()<< std::endl;
    }
}
void Player::readScroll()
{
    displayInventory();
    char c = getCharacter();
    int n = c - 'a';
    if(n>=0 && n<inventorySize)
    {
        if(inventory[n]->symbol() == '?'){
            Scroll* s = dynamic_cast<Scroll*>(inventory[n]);
            std::string type = s->objectName();
            //using all the different types of scrolls
            if(type=="scroll of improve armor")
                {arm(getArmor()+randInt(1, 3)); std:: cout << "Your armor glows blue."<< std::endl;}
            else if(type == "scroll of raise strength")
                {str(getStrength()+randInt(1, 3)); std:: cout << "Your muscles bulge."<< std::endl;}
            else if(type=="scroll of enhance dexterity")
                {dex(getDeterity()+1); std:: cout << "You feel like less of a klutz."<< std::endl;}
            else if (type=="scroll of enhance health")
                {MAX_HP+= randInt(3, 8); std:: cout << "You feel your body wrenched in space and time."<< std::endl;}
            else if(type=="scroll of teleportation")
            {
                bool start = true;
                while(start)
                {   int x, y; pos(x,y);
                    int a = randInt(1, levelHeight-1);
                    int b = randInt(1, levelWidth-1);
                    if(m_game->showCell(a,b) == ' ')
                    {
                        if(m_game->updateCell(a, b, symbol()))
                        {   m_game->updateCell(x, y, ' ');
                            start = false;
                            changePos(a,b);
                        }
                    }
                }
                std:: cout << "You feel your body wrenched in space and time."<< std::endl;
            }
            //destroy read scroll
            delete inventory[n];
            for(int k=n; k<inventorySize; k++)
                inventory[k]= inventory[k+1];
            inventorySize--;
        }
        else
            std::cout << "You can't read a " << inventory[n] -> objectName()<< std::endl;
    }
}
char Player::symbol() const
{
    return '@';
}
char Bogeyman::symbol() const
{
    return 'B';
}
void Bogeyman::move(){
    if(getSleepTime()==0){
        int playerX, playerY;
        m_game->playerPos(playerX, playerY);
        
        int bogeymanX, bogeymanY;
        this->pos(bogeymanX, bogeymanY);
        
        int distance = std::abs(bogeymanX - playerX) + std::abs(bogeymanY - playerY);
        
        if (distance > 5) {
            // Too far to smell the player
            return;
        }
        
        int newX = bogeymanX;
        int newY = bogeymanY;
        
        // Determine move direction
        if (bogeymanX != playerX) {
            newX += (playerX > bogeymanX) ? 1 : -1;
        } else if (bogeymanY != playerY) {
            newY += (playerY > bogeymanY) ? 1 : -1;
        }
        
        // Check if the new position is valid
        char cell = m_game->showCell(newX, newY);
        if ((cell == ' ' || cell == ')' || cell == '?' || cell == '>' || cell == '&') && m_game->monsterHere(newX, newY) == nullptr) {
            // Move to the new position
            //  In case monster standing over object
            GameObject* possibleObj = m_game->objectHere(bogeymanX, bogeymanY);
            if (m_game->isGoldenIdolPosition(bogeymanX, bogeymanY))
                m_game->updateCell(bogeymanX, bogeymanY, '&');
            else if (m_game->isStaircasePosition(bogeymanX, bogeymanY))
                m_game->updateCell(bogeymanX, bogeymanY, '>');
            else if (possibleObj != nullptr)
                m_game->updateCell(bogeymanX, bogeymanY, possibleObj->symbol());
            else
                m_game->updateCell(bogeymanX, bogeymanY, ' ');
            
            m_game->updateCell(newX, newY, 'B');
            this->changePos(newX, newY);
        }
    }}
void Bogeyman::dropObject()
{
    int a,b; 
    this->pos(a,b);
    if(m_game ->objectHere(a, b) == nullptr && trueWithProbability(0.1))
    {GameObject* dropped = new Weapon("magic axe", a, b, m_game);
        m_game-> addGameObject(dropped);
        m_game->updateCell(a, b, ')');}
}
char Snakewoman::symbol() const
{
    return 'S';
}
void Snakewoman::move()
{
    if(getSleepTime()==0){
        int playerX, playerY;
        m_game->playerPos(playerX, playerY);
        
        int snakewomanX, snakewomanY;
        this->pos(snakewomanX, snakewomanY);
        
        int distance = std::abs(snakewomanX - playerX) + std::abs(snakewomanY - playerY);
        
        if (distance > 3) {
            // Too far to smell the player
            return;
        }
        
        int newX = snakewomanX;
        int newY = snakewomanY;
        
        // Determine move direction
        if (snakewomanX != playerX) {
            newX += (playerX > snakewomanX) ? 1 : -1;
        } else if (snakewomanY != playerY) {
            newY += (playerY > snakewomanY) ? 1 : -1;
        }
        
        // Check if the new position is valid
        char cell = m_game->showCell(newX, newY);
        if ((cell == ' ' || cell == ')' || cell == '?' || cell == '>' || cell == '&') && m_game->monsterHere(newX, newY) == nullptr) {
            // Move to the new position
            //In case monster standing over object
            GameObject* possibleObj = m_game->objectHere(snakewomanX, snakewomanY);
            if (m_game->isGoldenIdolPosition(snakewomanX, snakewomanY))
                m_game->updateCell(snakewomanX, snakewomanY, '&');
            else if (m_game->isStaircasePosition(snakewomanX, snakewomanY))
                m_game->updateCell(snakewomanX, snakewomanY, '>');
            else if (possibleObj != nullptr)
                m_game->updateCell(snakewomanX, snakewomanY, possibleObj->symbol());
            else
                m_game->updateCell(snakewomanX, snakewomanY, ' ');
            m_game->updateCell(newX, newY, 'S');
            this->changePos(newX, newY);
        }}
}
void Snakewoman::dropObject()
{
    int a,b; pos(a,b);
    if(m_game ->objectHere(a, b) == nullptr && trueWithProbability(1/3))
    {GameObject* dropped = new Weapon("magic fang of sleep", a, b, m_game);
        m_game-> addGameObject(dropped);
        m_game->updateCell(a, b, ')');}
        
}
char Dragon:: symbol() const
{
    return 'D';
}
void Dragon::move(){}
void Dragon::dropObject()
{
    int a,b; 
    this->pos(a,b);
    if(m_game ->objectHere(a, b) == nullptr)
    {GameObject* dropped = new Scroll("scroll of teleportation", a, b, m_game);
        m_game-> addGameObject(dropped);
        m_game->updateCell(a, b, '?');}
}
char Goblin::symbol() const
{
    return 'G';
}

void Goblin::dropObject()
{
    int a,b; 
    this -> pos(a,b);
    if(m_game ->objectHere(a, b) == nullptr && trueWithProbability(1/3))
    {GameObject* dropped = new Weapon("magic axe", a, b, m_game);
        m_game-> addGameObject(dropped);
        m_game->updateCell(a, b, ')');}
}
bool Goblin::findPath(int curX, int curY, int playerX, int playerY, int stepsLeft) {
    // Base case: Check if the current position is the player's position
    if (curX == playerX && curY == playerY) {
        return true; // Found a path to the player
    }

    // Base case: Check if steps left is 0 or if the current position is out of bounds or a wall
    if (stepsLeft <= 0 || curX < 0 || curX >= levelHeight || curY < 0 || curY >= levelWidth || m_game->showCell(curX, curY) == '#') {
        return false; // Cannot move further
    }

    // Mark the current position as visited by temporarily blocking it
    char originalCell = m_game->showCell(curX, curY);
    m_game->updateCell(curX, curY, '.'); // Mark as visited

    // Define the four possible moves (up, down, left, right)
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    // Explore each possible move recursively
    for (int i = 0; i < 4; ++i) {
        int newX = curX + dx[i];
        int newY = curY + dy[i];
        if (findPath(newX, newY, playerX, playerY, stepsLeft - 1)) {
            // Restore the original cell state before returning
            m_game->updateCell(curX, curY, originalCell);
            return true; // Found a path to the player
        }
    }

    // Restore the original cell state before returning
    m_game->updateCell(curX, curY, originalCell);

    return false; // No path found
}

void Goblin::move() {
    int playerX, playerY;
    m_game->playerPos(playerX, playerY);

    int goblinX, goblinY;
    this->pos(goblinX, goblinY);

    // Calculate the distance between the goblin and the player
    int distance = abs(playerX - goblinX) + abs(playerY - goblinY);

    if (distance > smellDistance) {
        return; // Player is out of smell distance, goblin doesn't move
    }

    // Define the four possible moves (up, down, left, right)
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};

    int optimalMoveIndex = -1;
    int minDistanceToPlayer = INT_MAX;

    // Find the move that gets the goblin closer to the player
    for (int i = 0; i < 4; ++i) {
        int newX = goblinX + dx[i];
        int newY = goblinY + dy[i];
        char cell = m_game->showCell(newX, newY);

        if ((cell == ' ' || cell == ')' || cell == '?' || cell == '>' || cell == '&') && findPath(newX, newY, playerX, playerY, smellDistance - 1)) {
            int newDistance = abs(playerX - newX) + abs(playerY - newY);
            if (newDistance < minDistanceToPlayer) {
                optimalMoveIndex = i;
                minDistanceToPlayer = newDistance;
            }
        }
    }

    // Move the goblin to the new position if an optimal move is found
    if (optimalMoveIndex != -1) {
        int newX = goblinX + dx[optimalMoveIndex];
        int newY = goblinY + dy[optimalMoveIndex];

        // Handle the goblin's previous position
        if (m_game->isGoldenIdolPosition(goblinX, goblinY)) {
            m_game->updateCell(goblinX, goblinY, '&');
        } else if (m_game->isStaircasePosition(goblinX, goblinY)) {
            m_game->updateCell(goblinX, goblinY, '>');
        } else if (GameObject* possibleObj = m_game->objectHere(goblinX, goblinY)) {
            m_game->updateCell(goblinX, goblinY, possibleObj->symbol());
        } else {
            m_game->updateCell(goblinX, goblinY, ' ');
        }

        // Update the goblin's new position
        m_game->updateCell(newX, newY, symbol());
        this->changePos(newX, newY);
    }
}
