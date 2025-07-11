//
//  hash.hpp
//  pro4
//
//  Created by Fatimeh Al Ghannam on 6/3/24.
//

#ifndef HASH_INCLUDED
#define HASH_INCLUDED

#include <vector>
#include <string>

using namespace std;

// Node class to store sequence and offset
class node {
public:
    node(string seq, int off) : m_seq(seq), m_off(off) {}
    string sequence() const { return m_seq; }
    int offset() const { return m_off; }
private:
    string m_seq;
    int m_off;
};

// HashTable class
class hashTable {
public:
    hashTable();
    void insertSequence(string currentString, int currentOffset);
    bool containsSequence(string currentString, int& currentOffset);
    bool containsAnotherSequence(string currentString, int& latestVectorOffset, int& givenOffset);
    bool containsSequence(string currentString);
    int findVectorLocation(string currentString);
    size_t calculateCopyLength(string newDay, string& currentCopied, int newOffset, int oldOffset);
    size_t calculateAddLength(string newDay, string& currentCopied, int newOffset);
    size_t calculateCopyLengthAdvanced(string oldDay, string newDay, string& currentCopied, int newOffset, int oldOffset);

private:
    vector<vector<node>> m_table;
};


#endif /* HASH_INCLUDED */
