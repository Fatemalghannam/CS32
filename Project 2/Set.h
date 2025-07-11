//
//  Set.hpp
//  Project 2
//
//  Created by Fatimeh Al Ghannam on 4/17/24.
//

#ifndef SET_INCLUDED
#define SET_INCLUDED

#include <string>

using ItemType = std::string;

class Set
{
  public:
    Set();  //constructor
    Set(const Set& source); //copy constructor
    Set& operator=(const Set& rhs);    //assignment operator
    ~Set(); //destructor
    
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
    
    void dump() const;
    
private:
    struct Node
    {
        ItemType m_value;
        Node* m_next;
        Node* m_prev;
    };
    
    Node* m_head;
    int m_size;

};
//NONMEMBER FUNCTIONS
void unite(const Set& s1, const Set& s2, Set& result);
void inOnlyOne(const Set& s1, const Set& s2, Set& result);
#endif /* SET_INCLUDED */
