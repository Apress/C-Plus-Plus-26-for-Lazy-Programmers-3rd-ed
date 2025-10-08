// class Montana, for a game of Montana solitaire
//  -- from _C++26 for Lazy Programmers_

#include <stdexcept> //for std::out_of_range
#include "grid_loc.h"
#include "cell.h"
#include "deck.h"

#ifndef MONTANA_H
#define MONTANA_H

class Montana
{
public:
    static constexpr int ROWS = 4, COLS  = 13;
    static constexpr int NUM_EMPTY_CELLS =  4;// 4 empty cells in grid
    static constexpr int MAX_TURNS       =  4;// 4 turns allowed

    Montana           ()               {}
    Montana           (const Montana&) = delete; //Nobody should ever 
    Montana& operator=(const Montana&) = delete; //copy the whole game!

    void play();

private:
        // displaying
    void display           () const;

        // dealing and redealing
    void deal         (Cards::Deck& deck, Cards::Waste& waste);
    void cleanup      (Cards::Deck& deck, Cards::Waste& waste); // collect cards for redeal
    void resetGrid    ();                         // make it empty

        // playing a turn
    void makeLegalMove(bool& letsQuitOrEndTurn);
    void makeMove     (const GridLoc& oldLoc, 
                       const GridLoc& newLoc);
    bool detectVictory() const;
    void congratulationsOrCondolences(bool isVictory) const;

        // working with empty cells
    
    // store in emptyCells_ the location of each empty cell
    void identifyEmptyCells();
 
    // which of the empty cells has this row and col? A B C or D?
    char whichEmptyCell    (int row, int col) const;
 
    // Is this a valid cell index? It must be 0-3.
    bool inRange(unsigned int emptyCellIndex) const
    {
        return (emptyCellIndex < NUM_EMPTY_CELLS);
    }

    // placing cards
    Cards::Cell&  cellAt(const GridLoc& loc)
    {
        if (inRange(loc)) return grid_[loc.row_][loc.col_];
        else throw std::out_of_range ("Bad grid location");
    }

    const Cards::Cell& cellAt(const GridLoc& loc) const
    {
        if (inRange(loc)) return grid_[loc.row_][loc.col_];
        else throw std::out_of_range("Bad grid location");
    }

    // Is this location within the grid?
    bool inRange(const GridLoc& loc) const
    {
        return(0<=loc.row_ && loc.row_<ROWS  &&  0<=loc.col_ && loc.col_<COLS);
    }

    // Can Card c follow other card?
    bool canFollow(const Cards::Card& c, const Cards::Card& other) const
    {
        return c.suit() == other.suit() && c.rank() == other.rank() + 1;
    }

    // Can card c go at this location?
    bool canGoHere(const Cards::Card& c, const GridLoc& loc) const;

    // Is the cell at row, col ordered at its location? That is,
    //     could we put it here if it weren't already?
    bool cellIsCorrect(int row, int col) const
    {
        return ! grid_[row][col].empty() &&
               canGoHere(grid_[row][col].top(), GridLoc(row, col));
    }
    
    // data members
    Cards::Cell   grid_      [ROWS][COLS];// where the cards are
    GridLoc emptyCells_ [NUM_EMPTY_CELLS];// where the empty cells are
};
#endif //MONTANA_H
