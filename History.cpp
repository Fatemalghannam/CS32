#include "History.h"
#include "City.h"
#include "globals.h"
#include <iostream>
using namespace std;
History::History(int nRows, int nCols)
{
    m_rows = nRows;
    m_cols = nCols;
    for (int i=0; i<m_rows; i++)
        for (int j=0; j<m_cols; j++)
            m_Grid[i][j]=0; //initialize grid with all zeros (no tooters have been converted)
}
bool History::record(int r, int c)
{
    if (isInBounds(r, c) == false)
        return false;
    
    m_Grid[r-1][c-1]++; //top left corner is 1,1 so we minus 1
    return true;
}
void History::display() const
{   char letters[27]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //Char array with alphabet
    
    clearScreen();
    for(int i=0; i<m_rows; i++)
    {
        for(int j=0; j<m_cols; j++)
        {   int num = m_Grid[i][j];
            if(num==0)  //cell was not preached, print .
                cout << ".";
            else
            {
                if(num<=26) //cell was preached at least once and less than or equal to 26 times (print A to Z normally)
                    cout << letters[num-1];
                else
                    cout << letters[25];   //cell preached more than 26 times, print last char (Z)
            }
        }
        cout << endl;
    }
    cout << endl;
}
bool History::isInBounds(int r, int c)
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}
