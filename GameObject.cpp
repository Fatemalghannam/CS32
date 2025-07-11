//
//  GameObject.cpp
//  proj3
//
//  Created by Fatimeh Al Ghannam on 5/11/24.
//

#include "GameObject.h"

// Base GameObject constructor
GameObject::GameObject() : m_x(0), m_y(0) {}

GameObject::GameObject(int x, int y) : m_x(x), m_y(y) {}

bool GameObject::isAtPosition(int x, int y) const {
    return m_x == x && m_y == y;
}

// Weapon constructors
Weapon::Weapon() : dexterityBonus(0), damageAmount(0) {}

Weapon::Weapon(const std::string& type) {
    setType(type);
    if (type == "mace") {
        dexterityBonus = 0;
        damageAmount = 2;
        actionString = "swings mace at ";
    } else if (type == "short sword") {
        dexterityBonus = 0;
        damageAmount = 2;
        actionString = "slashes short sword at ";
    } else if (type == "long sword") {
        dexterityBonus = 2;
        damageAmount = 4;
        actionString = "swings long sword at ";
    } else if (type == "magic axe") {
        dexterityBonus = 5;
        damageAmount = 5;
        actionString = "strikes magic axe at ";
    } else if (type == "magic fang of sleep") {
        dexterityBonus = 3;
        damageAmount = 2;
        actionString = "strikes magic fang of sleep at ";
    } else {
        dexterityBonus = 0;
        damageAmount = 0;
    }
}
Weapon::Weapon(const std::string& type, int x, int y, Game* game){
    setGame(game);
    setPos(x, y);
    setType(type);
    if (type == "mace") {
        dexterityBonus = 0;
        damageAmount = 2;
        actionString = "swings mace at ";
    } else if (type == "short sword") {
        dexterityBonus = 0;
        damageAmount = 2;
        actionString = "slashes short sword at ";
    } else if (type == "long sword") {
        dexterityBonus = 2;
        damageAmount = 4;
        actionString = "swings long sword at ";
    } else if (type == "magic axe") {
        dexterityBonus = 5;
        damageAmount = 5;
        actionString = "strikes magic axe at ";
    } else if (type == "magic fang of sleep") {
        dexterityBonus = 3;
        damageAmount = 2;
        actionString = "strikes magic fang of sleep at ";
    } else {
        dexterityBonus = 0;
        damageAmount = 0;
    }
}

// Weapon methods


char Weapon::symbol() const {
    return ')';
}

// Scroll constructor
Scroll::Scroll(std::string type, Game* game){setGame(game); setType(type);}
Scroll:: Scroll(std::string type, int x, int y, Game* game){setType(type); setPos(x, y); setGame(game);}
Scroll::Scroll(std::string type) {setType(type);}

// Scroll methods


char Scroll::symbol() const {
    return '?';
}
