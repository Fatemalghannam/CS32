//
//  main.cpp
//  Project 2
//
//  Created by Fatimeh Al Ghannam on 4/17/24.
//

/*#include <iostream>
#include <cassert>
#include "Set.h"
using namespace std;


int main()
{ Set ss; assert(ss.insert("A"));
 assert(ss.insert("C"));
 assert(ss.insert("B"));
 Set s2; assert(s2.insert("A"));
 assert(s2.insert("B"));
 assert(s2.insert("C"));
 Set result; assert(result.insert("P"));
 inOnlyOne(ss, s2, result); assert(result.empty());
 unite(ss, s2, result); assert(result.size()==3);
 assert(ss.insert("Z")); assert(ss.insert("P"));
 assert(ss.insert("D")); assert(ss.size() == 6);
 ItemType v; assert(ss.get(5, v)&& v=="A");
 assert(ss.erase("Z")&& !ss.contains("Z"));
 cout << "Assertions successful" << endl; return 0; }*/

#include <iostream>
#include <cassert>
#include "Set.h" // Assuming Set class is defined in Set.h

void testDefaultConstructor() {
    Set set;
    assert(set.empty());
    assert(set.size() == 0);
}

void testInsertAndSize() {
    Set set;
    assert(set.insert("apple"));
    assert(!set.empty());
    assert(set.size() == 1);
    
    assert(set.insert("banana"));
    assert(set.insert("orange"));
    assert(set.size() == 3);
}

void testErase() {
    Set set;
    set.insert("apple");
    set.insert("banana");
    set.insert("orange");
    
    assert(set.erase("banana"));
    assert(set.size() == 2);
    
    assert(!set.erase("grape"));
    assert(set.size() == 2);
}

void testContains() {
    Set set;
    set.insert("apple");
    set.insert("banana");
    set.insert("orange");
    
    assert(set.contains("apple"));
    assert(set.contains("banana"));
    assert(set.contains("orange"));
    
    assert(!set.contains("grape"));
}

void testGet() {
    Set set;
    set.insert("apple");
    set.insert("banana");
    set.insert("orange");
    
    ItemType value;
    assert(set.get(0, value));
    assert(value == "orange");
    
    assert(!set.get(3, value));
    assert(!set.get(-1, value));
    assert(set.get(1, value));
}

void testSwap() {
    Set set1;
    set1.insert("apple");
    Set set2;
    set2.insert("banana");
    
    set1.swap(set2);
    assert(set1.size() == 1 && set1.contains("banana"));
    assert(set2.size() == 1 && set2.contains("apple"));
}

void testCopyConstructor() {
    Set set1;
    set1.insert("apple");
    set1.insert("banana");
    
    Set set2(set1);
    assert(set2.size() == 2 && set2.contains("apple") && set2.contains("banana"));
}

void testAssignmentOperator() {
    Set set1;
    set1.insert("apple");
    set1.insert("banana");
    
    Set set2;
    set2 = set1;
    assert(set2.size() == 2 && set2.contains("apple") && set2.contains("banana"));
}

void testUnite() {
    Set s1;
    s1.insert("apple");
    s1.insert("banana");
    
    Set s2;
    s2.insert("orange");
    s2.insert("grape");
    
    Set result;
    unite(s1, s2, result);
    assert(result.size() == 4 && result.contains("apple") && result.contains("banana") && result.contains("orange") && result.contains("grape"));
}

void testInOnlyOne() {
    Set s1;
    s1.insert("apple");
    s1.insert("banana");
    
    Set s2;
    s2.insert("banana");
    s2.insert("orange");
    
    Set result;
    inOnlyOne(s1, s2, result);
   
    assert(result.size() == 2 && result.contains("apple") && result.contains("orange"));
}

int main() {
    testDefaultConstructor();
    testInsertAndSize();
    testErase();
    testContains();
    testGet();
    testSwap();
    testCopyConstructor();
    testAssignmentOperator();
    testUnite();
    testInOnlyOne();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

