//
//  Set.cpp
//  Project 2
//
//  Created by Fatimeh Al Ghannam on 4/17/24.
//
#include <iostream> //REMOVE LATER
#include "Set.h"
Set::Set()
{
    m_head = nullptr;   //default set is empty
    m_size = 0;
}
Set::Set(const Set& source){
    m_head = nullptr; // Initialize m_head to nullptr to ensure proper initialization
       m_size = 0; // Initialize m_size to 0

       // Copy elements from source to the new set
       ItemType value;
       for (Node* p = source.m_head; p != nullptr; p = p->m_next) {
           value = p->m_value;
           insert(value); // Use the insert function to add the value to the new set
       }

    
}
Set& Set::operator=(const Set& rhs)
{
    if(this != &rhs)
        {
            //create a temp Set to store the values of rhs
            //swap lhs and rhs
            Set temp(rhs);
            swap(temp);
            temp.~Set();
        }//end if
        return *this;
}

Set::~Set()
{
    Node* p = m_head;
    while (p != nullptr) {
        Node* next = p->m_next;
        delete p;
        p = next;
    }
}
bool Set::empty() const
{
    if(m_head==nullptr) //head does not point to item, ie set is empty
        return true;
    return false;
}

int Set::size() const
{
    return m_size;
}
bool Set::insert(const ItemType& value)
{   //create insert so Set is always in strictly DECEREASING order
        Node* p = m_head;
        // Special case: if the list is empty, or if value is greater than the head node (so value must become first item in set)
        if (p == nullptr || p->m_value <= value) {
            Node* newNode = new Node;
            newNode->m_value = value;
            newNode->m_next = p;
            if (p != nullptr) 
            {
                if(p->m_value==value)   //head node has same value we want to insert
                {   delete newNode;
                    return false;}
                p->m_prev = newNode;
            }
            newNode ->m_prev = nullptr;         //first element in list's previous pointer points to NULL
            m_head = newNode;
            m_size++;
            return true;
        }
        else{
            
            // Find the correct position to insert the new node
            while (p->m_next != nullptr && p->m_next->m_value > value) {    //if item is > value then go to next item
                p = p->m_next;
            }
            
            // Check if the value already exists
            if (p->m_next != nullptr && p->m_next->m_value == value) {
                return false;
            }
            
            // Insert the new node
            Node* newNode = new Node;
            newNode->m_value = value;
            newNode->m_next = p->m_next;
            newNode->m_prev = p;
            if (p->m_next != nullptr) {
                p->m_next->m_prev = newNode;
            }
            p->m_next = newNode;
            m_size++;
            return true;}
}

bool Set::erase(const ItemType& value)
{
    if(!contains(value))
        return false;
    Node* p=m_head;
  while(p->m_value!=value)
      p=p->m_next;
    
    if(p!=m_head && p->m_next!=nullptr){        //Not the head nor the tail
        (p->m_prev)->m_next = p->m_next;
        (p->m_next)->m_prev = p->m_prev;
        delete p;}
    else if(p==m_head && p->m_next!=nullptr)    //The head not the tail or middle
    {
        m_head= p->m_next;
        (p->m_next)->m_prev = nullptr;
        delete p;
    }
    else if(m_size>1){                                   //The tail, which does not coincide with the head
        (p->m_prev)->m_next = nullptr;
        delete p;
    }
    else{                                               //One element set, head and tail coincide
        delete p;
        m_head=nullptr;
    }
    m_size--;
    return true;
}

bool Set::contains(const ItemType& value) const
{
    Node* p = m_head;
        while (p != nullptr && p->m_value >= value) {
            if (p->m_value == value) {
                return true;
            }
            p = p->m_next;
        }   //goes through whole linked list, if value found return true, else return false
        return false;
}
bool Set::get(int pos, ItemType& value) const
{       //Due to Set always being strictly DECREASING
    if(pos<0 || pos>= size())
        return false;
    int i=0;
    Node* p = m_head;
    while(i<pos)
    {
        p=p->m_next;
        i++;
    }
    
    value = p->m_value;
    return true;
}

void Set::swap(Set& other)
{
    int t = size();
    m_size = other.m_size;       // switching sizes
    other.m_size = t;

    Node* temp = m_head;
    m_head = other.m_head;  // switching head pointers
    other.m_head = temp;
}
void Set::dump() const{
    for(Node* p = m_head; p!= nullptr; p=p->m_next){
        std::cerr<< p->m_value << std::endl;
    }
}

void unite(const Set& s1, const Set& s2, Set& result){
    //create an empty set then at the very end swap it with result
    Set s;
    ItemType v;
    for(int i=0; i<s1.size(); i++){
        if(s1.get(i, v))    //retrieves all elements in the set without accessing private members suchas Nodes or the head
            s.insert(v);    //will not insert an element which already exists
    }
    for(int i=0; i<s2.size(); i++){
        if(s2.get(i, v))
            s.insert(v);    //duplicates will bw ignored
    }
    
    result.swap(s); //result was possibly nonempty, so this is the safe approach
}
void inOnlyOne(const Set& s1, const Set& s2, Set& result)
{
    Set s;
    ItemType v, u;
    bool b=true;
    for (int i=0; i<s1.size(); i++) {   //adds all unique elements of s1
        for (int j=0; j<s2.size(); j++) {
            if(s1.get(i, v)&& s2.get(j, u)){
                if(v==u)
                {   b=false;    //found in both sets, s1 and s2
                    break;}
            }
        }
        if(b)
            s.insert(v);
        b=true; //reseting b
    }
    
    b=true;
    for (int j=0; j<s2.size(); j++) {   //adds all unique elements of s2
        for (int i=0; i<s1.size(); i++) {
            if(s2.get(j, u)&& s1.get(i, v)){
                if(v==u)
                {   b=false;
                    break;}
            }
        }
        if(b)
            s.insert(u);
        b=true;
    }
       result.swap(s);
}
