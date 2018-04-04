# Mini-Checkers
NYU Tandon School of Engineering
CS-GY 6613 - Artificial Intelligence I

*More information in PDF

AI project with the intent of creating an AI that uses an Alpha-Beta Pruning algorithm to play against a human opponent in a modified version of checkers.  In a nutshell, this modified checkers game is only on a 6x6 board (as opposed to an 8x8 board), pieces can only move forward, and pieces cannot jump more than once per turn.  

# General Timeline

Not Started

*In Progress*

~~Completed~~

*Not necessarily in order
1. Develop General Checkers Class
  - ~~Create cBoard class~~
  - ~~Constructor~~
  - Copy Constructor
  - ~~Destructor~~
  - ~~Moving Pieces~~
    - ~~Single move, and jump moves~~
    - ~~Check to see if valid move~~
    - Possibly return a boolean to indicate if it is a valid move? -> would be useful for AI
  - *Check if game ended/who won*
    - Potentially add utility value function here, based on number of pieces left
  - ~~Count Pieces for both players~~
2. Add GUI
  - Research SFML
    https://www.sfml-dev.org/
  - Point and Click functionality, should check that clicking on valid piece
  - NOTE: Spend no more than one day on this, just make sure it works
3. Program and Test Alpha-Beta Pruning Algorithm
  - Use algorithm on simpler case
  - Add in utility value checking, and depth-checking
  - Limit depth of search, and code evaluation function
4. Test complete program
  - Make sure it works with maximum difficulty
  - Add in variable difficulty (possibly changing either utility values, or maximum depth)