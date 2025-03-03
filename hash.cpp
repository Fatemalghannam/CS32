#include "hash.h"
#include <functional>

// Constructor initializing the hash table
hashTable::hashTable() {
    m_table.resize(200000);
}

// Function to insert a string with its offset
void hashTable::insertSequence(string currentString, int currentOffset) {
    node toAdd(currentString, currentOffset);
    int hashNumber = hash<string>()(currentString);
    hashNumber = abs(hashNumber);
    int pos = hashNumber % 199999;
    m_table[pos].push_back(toAdd);
}

// Function to check if a string exists and get its offset
bool hashTable::containsSequence(string currentString, int& currentOffset) {
    int hashNum = hash<string>()(currentString);
    hashNum = abs(hashNum);
    int pos = hashNum % 199999;
    if (m_table[pos].size() != 0) {
        for (int i = 0; i < m_table[pos].size(); i++) {
            if (m_table[pos].at(i).sequence() == currentString) {
                currentOffset = m_table[pos].at(i).offset();
                return true;
            }
        }
    }
    return false;
}

// Function to find another occurrence of the string
bool hashTable::containsAnotherSequence(string currentString, int& latestVectorOffset, int& givenOffset) {
    int hashNum = hash<string>()(currentString);
    hashNum = abs(hashNum);
    int pos = hashNum % 199999;
    for (int i = latestVectorOffset + 1; i < m_table[pos].size(); i++) {
        if (m_table[pos].at(i).sequence() == currentString) {
            givenOffset = m_table[pos].at(i).offset();
            latestVectorOffset = i;
            return true;
        }
    }
    return false;
}

// Overloaded function to check existence without getting offset
bool hashTable::containsSequence(string currentString) {
    int any;
    return containsSequence(currentString, any);
}

// Function to find the vector location of a string
int hashTable::findVectorLocation(string currentString) {
    int hashNum = hash<string>()(currentString);
    hashNum = abs(hashNum);
    int pos = hashNum % 199999;
    for (int i = 0; i < m_table[pos].size(); i++) {
        if (m_table[pos].at(i).sequence() == currentString) {
            return i;
        }
    }
    return -1;
}

// Function to calculate the copy length
size_t hashTable::calculateCopyLength(string newDay, string& currentCopied, int newOffset, int oldOffset) {
    size_t currentlength = currentCopied.length();
    size_t N = currentlength;
    int usedOffset = newOffset;
    int usedOldOffset = oldOffset;
    bool stillGoing = true;
    while (stillGoing) {
        string newString = "";
        usedOffset++;
        usedOldOffset++;
        for (int i = 0; i < N && usedOffset < newDay.length() - N; i++) {
            newString += newDay[usedOffset + i];
        }
        int targetOffset;
        if (usedOffset > newDay.length() - N) {
            stillGoing = false;
        } else if (containsSequence(newString, targetOffset) && targetOffset == usedOldOffset) {
            currentlength++;
        } else {
            stillGoing = false;
        }
    }
    return currentlength;
}

// Function to calculate the add length
size_t hashTable::calculateAddLength(string newDay, string& currentCopied, int newOffset) {
    size_t currentlength = 1;
    size_t N = currentCopied.length();
    int usedOffset = newOffset;
    bool stillGoing = true;
    while (stillGoing) {
        string newString = "";
        usedOffset++;
        for (int i = 0; i < N && usedOffset < newDay.length() - N; i++) {
            newString += newDay[usedOffset + i];
        }
        if (usedOffset > newDay.length() - N) {
            stillGoing = false;
        } else if (!containsSequence(newString)) {
            currentlength++;
            if (usedOffset == newDay.length() - N) {
                currentlength += N - 1;
            }
        } else {
            stillGoing = false;
        }
    }
    return currentlength;
}

// Function to calculate the copy length using two strings
size_t hashTable::calculateCopyLengthAdvanced(string oldDay, string newDay, string& currentCopied, int newOffset, int oldOffset) {
    size_t currentLength = currentCopied.length();
    size_t N = currentLength;
    bool stillGoing = true;
    size_t usedNewOffset = newOffset + N;
    size_t usedOldOffset = oldOffset + N;
    while (stillGoing && usedNewOffset < newDay.length() && usedOldOffset < oldDay.length()) {
        if (newDay[usedNewOffset] == oldDay[usedOldOffset]) {
            currentLength++;
        } else {
            stillGoing = false;
        }
        usedOldOffset++;
        usedNewOffset++;
    }
    return currentLength;
}
