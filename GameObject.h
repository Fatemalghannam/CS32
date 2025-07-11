//
//  GameObject.hpp
//  proj3
//
//  Created by Fatimeh Al Ghannam on 5/11/24.
//
 #ifndef GAMEOBJECT_INCLUDED
 #define GAMEOBJECT_INCLUDED

 #include <string>
class Game;
 class GameObject {
 public:
     GameObject();
     GameObject(int x, int y);
     virtual ~GameObject() = default; // Virtual destructor for proper cleanup
     std::string objectName() const{ return m_name;} // Pure virtual function
     virtual char symbol() const = 0; // Pure virtual function
     bool isAtPosition(int x, int y) const; //Checks if object is in pos
     int getX(){return m_x;} int getY(){return m_y;}
     void setGame(Game* game){m_game = game;}
     void setType(std::string type){m_name = type;}
     void setPos(int a, int b){m_x = a; m_y = b;}
 protected:
     std::string m_name; // Protected to allow access in derived classes
     int m_x, m_y;
     Game* m_game;
 };

 class Weapon : public GameObject {
 public:
     Weapon();
     Weapon(const std::string& type);
     Weapon(const std::string& type, int x, int y, Game* game);
     virtual char symbol() const override;
     int getDamage()const{return damageAmount;}
     int getDexterity()const{return dexterityBonus;}
     std::string getActionString() const{ return actionString;}
 private:
     std::string actionString;
     int dexterityBonus;
     int damageAmount;
 };

 class Scroll : public GameObject {
 public:
     Scroll(std::string type, Game* game);
     Scroll(std::string type);
     Scroll(std::string type, int x, int y, Game* game);
     virtual char symbol() const override;

 };

 #endif // GAMEOBJECT_INCLUDED

