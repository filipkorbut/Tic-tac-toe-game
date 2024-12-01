#include <iostream>
#include <cstdlib>
#include <limits>

using namespace std;

// Function to display the current game board
void displayBoard(char board[3][3]) {
    system("cls"); // Clear the console (Windows-specific)

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            cout << (board[row][col] != ' ' ? board[row][col] : ' ');
            if (col != 2) cout << "|";
        }
        cout << endl;
        if (row != 2) cout << "-----" << endl;
    }
}

// Function to check if the game board is full (no empty spaces left)
bool isBoardFull(char board[3][3]) {
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == ' ') return false;
        }
    }
    return true;
}

// Function to check if a specific player has won
bool checkWinner(char board[3][3], char player) {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) return true; // Row
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player) return true; // Column
    }
    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) return true; // Diagonal \
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) return true; // Diagonal /
    
    return false;
}

// Minimax evaluation function to determine the score of a game state
int minimax(char board[3][3], bool isMaximizing) {
    // Check for base conditions
    if (checkWinner(board, 'X')) return 10; // AI wins
    if (checkWinner(board, 'O')) return -10; // Player wins
    if (isBoardFull(board)) return 0; // Draw (board is full)

    int bestScore = isMaximizing ? -1000 : 1000;

    // Explore all possible moves
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = isMaximizing ? 'X' : 'O'; // Make a move
                int score = minimax(board, !isMaximizing); // Recursively call minimax for the next move
                board[row][col] = ' '; // Undo the move

                // Update the best score
                if (isMaximizing) {
                    bestScore = max(score, bestScore);
                } else {
                    bestScore = min(score, bestScore);
                }
            }
        }
    }
    return bestScore;
}

// Function to determine the best move for the AI
void findBestMove(char board[3][3], int& bestRow, int& bestCol) {
    int bestScore = -1000; // Initialize to a very low value
    bestRow = -1; // Invalid row
    bestCol = -1; // Invalid column

    // Explore all possible moves to find the best one
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = 'X'; // AI makes the move
                int score = minimax(board, false); // Evaluate the move using minimax
                board[row][col] = ' '; // Undo the move

                if (score > bestScore) {
                    bestScore = score;
                    bestRow = row; // Update best row
                    bestCol = col; // Update best column
                }
            }
        }
    }
}

// Function to set the game's difficulty level
int setDifficultyLevel() {
    int difficulty;
    cout << "Select your difficulty:\n";
    cout << "1 - Easy\n2 - Medium\n3 - Hard\n";

    while (true) {
        cout << "Enter difficulty (1-3): ";
        cin >> difficulty;

        // Validate input
        if (cin.fail() || difficulty < 1 || difficulty > 3) {
            cout << "Invalid input! Please enter a number between 1 and 3.\n";
            cin.clear();  // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the incorrect input
        } else {
            break;
        }
    }
    return difficulty;
}

// Function to validate the player's move
bool isValidMove(int row, int col, char board[3][3], bool isAI) {
    // Validate row and column within bounds
    if (row < 0 || row >= 3 || col < 0 || col >= 3) {
        displayBoard(board);
        cout << "Invalid move! Row and column must be between 1 and 3.\n";
        return false;
    }

    // Validate if the cell is already occupied
    if (board[row][col] != ' ') {
        displayBoard(board);
        cout << "Invalid move! Cell is already occupied.\n";
        return false;
    }

    return true;
}

// Function to handle AI's move based on the difficulty level
void aiMove(int difficulty, char board[3][3], int& bestRow, int& bestCol) {
    int randomChance = rand() % 101 + 1; // Random number for randomness in Medium mode
    bool makeBestMove = randomChance >= 60; // 40% chance to make the best move (for Medium difficulty)
    int randomRow, randomCol;

    switch (difficulty) {
        case 1:
            // Easy mode: AI picks a random empty spot
            do {
                randomRow = rand() % 3;
                randomCol = rand() % 3;
            } while (!isValidMove(randomRow, randomCol, board, true));
            bestRow = randomRow;
            bestCol = randomCol;
            break;

        case 2:
            // Medium mode: 30% chance for a random move, 70% for the best move
            if (makeBestMove) {
                findBestMove(board, bestRow, bestCol);
            } else {
                do {
                    randomRow = rand() % 3;
                    randomCol = rand() % 3;
                } while (!isValidMove(randomRow, randomCol, board, true));
                bestRow = randomRow;
                bestCol = randomCol;
            }
            break;

        case 3:
            // Hard mode: AI always makes the best move
            findBestMove(board, bestRow, bestCol);
            break;
    }
}

int main() {
    srand(time(0));
    bool isGameOver = false;
    char gameBoard[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };
    int difficultyLevel;
    int bestRow, bestCol;

    cout << "Tic Tac Toe Game!" << endl;
    difficultyLevel = setDifficultyLevel();
    displayBoard(gameBoard);

    // Main game loop
    while (!isGameOver) {
        cout << "Enter the row and column to place your mark (e.g., '2 3'): ";
        int row, col;

        // Get and validate player's move
        cin >> row >> col;
        row -= 1; // Convert to 0-based index
        col -= 1; // Convert to 0-based index

        while (!isValidMove(row, col, gameBoard, false)) {
            cout << "Enter again (row and column between 1 and 3): ";
            cin >> row >> col;
            row -= 1;
            col -= 1;
        }

        // Player makes the move
        gameBoard[row][col] = 'O';
        displayBoard(gameBoard);

        // Check for game over after the player's move
        isGameOver = isBoardFull(gameBoard) || checkWinner(gameBoard, 'O');
        if (isGameOver) break;

        // AI makes it's move
        aiMove(difficultyLevel, gameBoard, bestRow, bestCol);
        gameBoard[bestRow][bestCol] = 'X';
        displayBoard(gameBoard);

        // Check for game over after the AI's move
        isGameOver = isBoardFull(gameBoard) || checkWinner(gameBoard, 'X');
    }

    // Display the result
    if (checkWinner(gameBoard, 'O')) {
        cout << "You win!\n";
    } else if (checkWinner(gameBoard, 'X')) {
        cout << "AI wins!\n";
    } else {
        cout << "It's a draw!\n";
    }
    
    return 0;
}
