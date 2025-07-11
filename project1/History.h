//
//  History.hpp
//  project1
//
//  Created by Fatimeh Al Ghannam on 4/7/24.
//

#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED

#include <stdio.h>
#include "globals.h"
class History
    {
      public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
    private:
        int m_rows;
        int m_cols;
        int m_Grid[MAXROWS][MAXCOLS];
        bool isInBounds(int r, int c);
    };

#endif /* HISTORY_INCLUDED */
