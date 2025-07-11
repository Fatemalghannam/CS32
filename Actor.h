//
//  Actor.hpp
//  proj3
//
//  Created by Fatimeh Al Ghannam on 5/11/24.
//

#ifndef ACTOR_INCLUDED
#define ACTOR_INCLUDED
#include "utilities.h"
#include "GameObject.h"
class Game;

class Actor
{
public:
    Actor();
    virtual ~Actor();
    Actor* assignGame(Game* game);
    virtual char symbol() const = 0;      //Pure abstract class, a general actor cannot exist(or have a symbol)
    virtual void move(){}
    
    void pos(int& a, int& b) const;      //get current position
    void changePos(int a, int b);        //change positions
    Game* m_game;                       //Points to game actor is in
    bool getAttacked(int damage);       //decreases HP by damage
    virtual void dropObject(){}          //determines the dropped object when killed
    //Getters
    int getHitPoints() const{ return hitPoints;}
    int getStrength() const{ return strength;}
    int getDeterity()const{ return dexterity;}
    int getArmor()const{ return armor;}
    int getSleepTime()const{ return sleepTime;}
    std::string getName() const{return name;}
    Weapon* getWeapon()const{return weapon;}
    bool isDead() const{ return (!isAlive); }
    void stats(int& h, int& a, int& s, int& d, int& sl) const;
    //Setters
    void hit(int a){hitPoints = a;}
    void arm(int a){armor= a;}
    void str(int a){strength= a;}
    void dex(int a){dexterity = a;}
    void slep(int a){sleepTime = a;}
    void type(std::string type){name = type;}
    void setWeapon(Weapon* newWeapon){ weapon = newWeapon;}
private:
    int x, y; // Current position
    int hitPoints;
    int armor;
    int strength;
    int dexterity;
    int sleepTime = 0;
    bool isAlive = true;
    std:: string name;
    Weapon* weapon;
    
};

class Player : public Actor {
public:
    Player();
    virtual ~Player(){}
    virtual char symbol() const;
    virtual void move(){}
    void move(char arrow);
    void pickUp(GameObject* object);
    void displayInventory() const;
    void wieldWeapon();
    void readScroll();
    virtual void dropObject(){}
private:
    GameObject* inventory[26];
    int inventorySize;
    int MAX_HP;
};


class Bogeyman : public Actor {

public:
    Bogeyman(){
        hit(randInt(5, 10)); arm(2); str(randInt(2, 3)); dex(randInt(2, 3)); type("Bogeyman"); setWeapon(new Weapon("short sword"));
    }
    virtual char symbol()const;
    virtual ~Bogeyman(){}
    virtual void move();
    virtual void dropObject();
private:

};

class Snakewoman : public Actor {
public:
    Snakewoman(){
        hit(randInt(3, 6)); arm(3); str(2); dex(3); type("Snakewoman");  setWeapon(new Weapon("magic fang of sleep"));
    }
    virtual ~Snakewoman(){}
    virtual char symbol() const;
    virtual void dropObject();
    virtual void move();
private:
   
};

class Dragon : public Actor {
public:
    Dragon(){
        hit(randInt(20, 25)); arm(4); str(4); dex(4); type("Dragon"); setWeapon(new Weapon("long sword"));
    }
    virtual char symbol() const;
    virtual void dropObject();
    virtual ~Dragon(){}
    virtual void move();
private:
    
};

class Goblin : public Actor {
public:
    Goblin(int smell){
        hit(randInt(15, 20)); arm(1); str(3); dex(1); type("Goblin"); setWeapon(new Weapon("short sword"));
        smellDistance =smell;
    }
    virtual ~Goblin(){}
    virtual char symbol()const;
    virtual void dropObject();
    virtual void move();
    //Move helpers
    bool findPath(int curX, int curY, int playerX, int playerY, int stepsLeft);
private:
    int smellDistance;
};

#endif /* ACTOR_INCLUDED */
