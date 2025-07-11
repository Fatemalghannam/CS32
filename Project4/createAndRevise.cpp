#include <fstream>
#include <iostream>
#include <algorithm>
#include "hash.h"
#include <string>
#include <iterator>
#include <cassert>
#include <sstream>

using namespace std;

bool getInt(istream& inf, int& n)
{
    char ch;
    if (!inf.get(ch) || !isascii(ch) || !isdigit(ch))
        return false;
    inf.unget();
    inf >> n;
    return true;
}

bool getCommand(istream& inf, char& cmd, char& delim, int& length, int& offset)
{
    if (!inf.get(cmd))
    {
        cmd = 'x';  // signals end of file
        return true;
    }
    switch (cmd)
    {
        case '+':
            return inf.get(delim).good();
        case '#':
            {
                char ch;
                return getInt(inf, offset) && inf.get(ch) && ch == ',' && getInt(inf, length);
            }
        case '\r':
        case '\n':
            return true;
    }
    return false;
}

char DELIM = '@';

void createRevision(std::istream& fold, std::istream& fnew, std::ostream& frevision) {
    // Read entire contents of fold into oldDay
    std::string oldDay = "";
    char newChar;
    while (fold.get(newChar)) {
        oldDay += newChar;
    }

    // Read entire contents of fnew into newDay
    std::string newDay = "";
    while (fnew.get(newChar)) {
        newDay += newChar;
    }

    // If newDay is empty
    if (newDay.empty())
            return;

    //If oldDay is empty
    if(oldDay.empty()){
        frevision << "+"<<DELIM << newDay << DELIM;
        return;
    }

    // If newDay is equivalent to oldDay, do nothing
    if (newDay == oldDay) {
        return;
    }

    // Determine the size of each sequence
    int N = 8;
    if (newDay.length() >= 50000) {
        N = 32;
    }
    if (newDay.length() >= 100000) {
        N = 64;
    }
    if (newDay.length() < N || oldDay.length() < N) {
        N = std::min(newDay.length(), oldDay.length());
    }

    // Create a hash table and add all N character sequences from oldDay
    hashTable table;
    for (int i = 0; i <= oldDay.length() - N; i++) {
        table.insertSequence(oldDay.substr(i, N), i);
    }

    // Loop through the newDay string to create the revision
    for (int j = 0; j <= newDay.length() - N; j++) {
        std::string checking = newDay.substr(j, N);

        if (table.containsSequence(checking)) {
            int oldOffset;
            table.containsSequence(checking, oldOffset);
            size_t copyLength = table.calculateCopyLengthAdvanced(oldDay, newDay, checking, j, oldOffset);
            int otherOffset = oldOffset;
            int latestVectorOffset = table.findVectorLocation(checking);
            while (table.containsAnotherSequence(checking, latestVectorOffset, otherOffset)) {
                size_t newLength = table.calculateCopyLengthAdvanced(oldDay, newDay, checking, j, otherOffset);
                if (newLength > copyLength) {
                    copyLength = newLength;
                    oldOffset = otherOffset;
                }
            }
            frevision << "#" << oldOffset << "," << copyLength;
            j += copyLength - 1;
            if (j != newDay.length() - 1 && newDay.length() - j - 1 < N) {
                frevision << "+" << DELIM;
                for (int i = j + 1; i < newDay.length(); i++) {
                    frevision << newDay[i];
                }
                frevision << DELIM;
            }
        } else {
            size_t addLength = table.calculateAddLength(newDay, checking, j);
            size_t endOffset = j + addLength;
            frevision << "+" << DELIM;
            for (; j < endOffset; j++) {
                frevision << newDay[j];
            }
    
            j--;
            frevision << DELIM;
            if (j != newDay.length() - 1 && newDay.length() - j - 1 < N) {
                frevision << "+"<< DELIM;
                for (int i = j + 1; i < newDay.length(); i++) {
                    frevision << newDay[i];
                }
                frevision << DELIM;
            }
        }
    }
}


bool revise(istream& fold, istream& frevision, ostream& fnew)
{
    char c;
    string oldString = "";
    while (fold.get(c))
    {
        oldString += c;
    }

    bool leave = false;
    while (frevision && !leave)
    {
        char cmd;
        int length = 0;
        int offset = 0;
        if (!getCommand(frevision, cmd, DELIM, length, offset))
        {
            cerr << "Failed to get command" << endl;
            return false;
        }

        switch (cmd)
        {
            case '+':
            {
                string addString;
                while (frevision.get(c) && c != DELIM)
                {
                    addString += c;
                }
                if (c != DELIM)
                {
                    return false;
                }
                fnew << addString;
                break;
            }
            case '#':
            {
                if (offset + length > static_cast<int>(oldString.size()) || offset < 0 || length < 0)
                {
                    cerr << "Invalid offset or length in copy command." << endl;
                    return false;
                }
                string copyString = oldString.substr(offset, length);
                fnew << copyString;
                break;
            }
            case 'x':
                leave = true;
                break;
            case '\n':
            case '\r':
                break;
            default:
                cerr << "Unknown command: " << cmd << endl;
                return false;
        }
    }
    return true;
}

bool runtest(string oldName, string newName, string revisionName, string newName2)
{
    if (revisionName == oldName  ||  revisionName == newName  ||
        newName2 == oldName  ||  newName2 == revisionName  ||
        newName2 == newName)
    {
        cerr << "Files used for output must have names distinct from other files" << endl;
        return false;
    }
    ifstream oldFile(oldName, ios::binary);
    if (!oldFile)
    {
        cerr << "Cannot open " << oldName << endl;
        return false;
    }
    ifstream newFile(newName, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName << endl;
        return false;
    }
    ofstream revisionFile(revisionName, ios::binary);
    if (!revisionFile)
    {
        cerr << "Cannot create " << revisionName << endl;
        return false;
    }
    createRevision(oldFile, newFile, revisionFile);
    revisionFile.close();

    oldFile.clear();   // clear the end of file condition
    oldFile.seekg(0);  // reset back to beginning of the file
    ifstream revisionFile2(revisionName, ios::binary);
    if (!revisionFile2)
    {
        cerr << "Cannot read the " << revisionName << " that was just created!" << endl;
        return false;
    }
    ofstream newFile2(newName2, ios::binary);
    if (!newFile2)
    {
        cerr << "Cannot create " << newName2 << endl;
        return false;
    }
    assert(revise(oldFile, revisionFile2, newFile2));
    newFile2.close();

    newFile.clear();
    newFile.seekg(0);
    ifstream newFile3(newName2, ios::binary);
    if (!newFile)
    {
        cerr << "Cannot open " << newName2 << endl;
        return false;
    }
    if ( ! equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
    {
        cerr << newName2 << " is not identical to " << newName
                 << "; test FAILED" << endl;
        return false;
    }
    return true;
}

int main()
{
    assert(runtest("greeneggs1.txt", "greeneggs2.txt", "myrevisionfile.txt", "mynewfile2.txt"));
    cerr << "Test PASSED" << endl;
}
