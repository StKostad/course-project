#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<cmath>
#include <fstream>
#include<string>
using namespace std;
int N;
int M;
double player1 = 0;
double player2 = 0;
vector<int> positionplayera(2);
vector<int> positionplayerb(2);
bool isEmpty() {
    ifstream file("savedgame.txt", ios::binary);
    if (!file) {
        return false;
    }
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    file.close();
    return fileSize == 0;
}
void printboard(vector<vector<int>> gameBoard, int N, int M, vector<vector<string>>Usedspaces, vector<int>positionplayera, vector<int>positionplayerb, double player1, double player2);
void gameplay(int N, int M, vector<int>positionplayera, vector<int>positionplayerb, vector<vector<string>>& Usedspaces, vector<vector<int>>& gameBoard, double player1, double player2);
//Saves the game
void savegame(int N, int M, vector<vector<int>>gb, vector<vector<string>> us, vector<int>pa, vector<int>pb, double sc1, double sc2) {
    ofstream emptyFile("savedgame.txt", ios::trunc);
    emptyFile.close();
    ofstream outFile("savedgame.txt");
    if (outFile.is_open()) {
        outFile << N << endl;
        outFile << M << endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                outFile << gb[i][j] << " ";
            }
        }
        outFile << endl;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                outFile << us[i][j] << ",";
            }
        }
        outFile << endl;
        outFile << pa[0] << " " << pa[1] << endl;
        outFile << pb[0] << " " << pb[1] << endl;
        outFile << sc1 << endl;
        outFile << sc2 << endl;
        outFile.close();
        cout << "Game saved" << endl;
    }
    else {
        std::cerr << "Failed to open the file for writing.\n";
    }
}
//Saves and quits the game
void quitgame(int N, int M, vector<vector<int>>gb, vector<vector<string>> us, vector<int>pa, vector<int>pb, double sc1, double sc2) {
    savegame(N, M, gb, us, pa, pb, sc1, sc2);
    exit(0);
}
//Loads a saved game
void loadgame() {
    system("cls");
    string uni;
    ifstream inFile("savedgame.txt");
    if (inFile.is_open()) {
        getline(inFile, uni);
        N = stoi(uni);
        getline(inFile, uni);
        M = stoi(uni);
        getline(inFile, uni);
        int tile = 0;
        vector<vector<int>> gameb(N, vector<int>(M));
        int tempN = 0;
        int tempM = 0;
        int counter = 1;
        for (char c : uni) {
            if (c == ' ') {
                gameb[tempN][tempM] = tile;
                counter = 1;
                tile = 0;
                if (tempM + 1 == M) {
                    tempN++;
                    tempM = 0;
                }
                else {
                    tempM++;
                }
            }
            else {
                tile = tile * counter;
                tile += c - '0';
                counter *= 10;
            }
        }
        uni = "";
        getline(inFile, uni);
        string cre;
        vector<vector<string>> us(N, vector<string>(M));
        tempN = 0;
        tempM = 0;
        for (char c : uni) {
            if (c == ',') {
                us[tempN][tempM] = cre;
                cre = "";
                if (tempM + 1 == M) {
                    tempN++;
                    tempM = 0;
                }
                else {
                    tempM++;
                }
            }
            else {
                cre += c;
            }
        }
        vector<int> pa(2);
        vector<int> pb(2);
        inFile >> pa[0] >> pa[1];
        inFile >> pb[0] >> pb[1];
        double sc1;
        double sc2;
        inFile >> sc1;
        inFile >> sc2;
        inFile.close();
        printboard(gameb, N, M, us, pa, pb, sc1, sc2);
        gameplay(N, M, pa, pb, us, gameb, sc1, sc2);
    }
    else {
        std::cerr << "Failed to open the file for reading.\n";
    }
}
//Checks whether or not a string can be converted to a number
bool isNumber(const string& str) {
    try {
        stoi(str);
        return true;
    }
    catch (const invalid_argument& e) {
        return false;
    }
    catch (const out_of_range& e) {
        return false;
    }
}
//Prints the board
void printboard(vector<vector<int>> gameBoard, int N, int M, vector<vector<string>>Usedspaces, vector<int>positionplayera, vector<int>positionplayerb, double player1, double player2) {
    cout << "+";
    for (int i = 0; i < M; i++) {
        cout << "----+";
    }
    cout << endl;
    for (int i = 0; i < N; i++) {
        cout << "|";
        for (int j = 0; j < M; j++) {
            if (Usedspaces[i][j][1] == 'b') {
                cout << "\033[44m";
            }
            else if (Usedspaces[i][j][1] == 'g') {
                cout << "\033[42m";
            }
            if ((positionplayera[0] == i && positionplayera[1] == j) || (positionplayerb[0] == i && positionplayerb[1] == j)) {
                cout << "\033[33m" << " " << Usedspaces[i][j][0] << gameBoard[i][j] << " " << "\033[0m";
            }
            else {
                cout << " " << Usedspaces[i][j][0] << gameBoard[i][j] << " " << "\033[0m";
            }
            cout << "|";
        }

        cout << endl;
        cout << "+";
        for (int i = 0; i < M; i++) {
            cout << "----+";
        }
        cout << endl;
    }
    cout << "\t" << "Blue:" << round((player1 + 0.0000000001) * 100.0) / 100.0 << "\t" << "Green:" << round((player2 + 0.000001) * 100.0) / 100.0 << endl;
}
//Generates all cells;
int generateCell(int Multcap, vector<vector<string>>& Usedspaces, int v, int t) {
    int random = rand() % 6;
    int value = (rand() % 10);
    switch (random) {

    case 0:
    case 1:
        return value;
    case 2:
    case 3:
        if (!value) {
            return value;
        }
        Usedspaces[v][t][0] = '-';
        return value;
    case 4:
        if (Multcap < 64) {
            value = value % 3;
        }
        Usedspaces[v][t][0] = '*';
        return value;
    case 5:
        if (Multcap < 64) {
            value = value % 2 + 1;
        }
        while (!value) {
            value = (rand() % 10);
        }
        Usedspaces[v][t][0] = '/';
        return value;
    }
    return 0;
}
//Ensures that there is at least one *2, /2, -x,+x square
void ensureRequiredOperations(vector<vector<int>>& gameBoard, int N, int M, vector<vector<string>>& Usedspaces) {
    int addRow = rand() % N, addCol = rand() % M;
    int subRow = rand() % N, subCol = rand() % M;
    int mulRow = rand() % N, mulCol = rand() % M;
    int divRow = rand() % N, divCol = rand() % M;
    while ((subRow == 0 && subCol == 0) || (subRow == N - 1 && subCol == M - 1)) {
        subRow = rand() % N;
        subCol = rand() % M;
    }
    while ((addRow == subRow && addCol == subCol) || (addRow == 0 && addCol == 0) || (addRow == N - 1 && addCol == M - 1)) {
        addRow = rand() % N;
        addCol = rand() % M;
    }
    while ((mulRow == addRow && mulCol == addCol) || (mulRow == subRow && mulCol == subCol) || (mulRow == 0 && mulCol == 0) || (mulRow == N - 1 && mulCol == M - 1)) {
        mulRow = rand() % N;
        mulCol = rand() % M;
    }
    while ((divRow == mulRow && divCol == mulCol) || (divRow == addRow && divCol == addCol) || (divRow == subRow && divCol == subCol) || (divRow == 0 && divCol == 0) || (divRow == N - 1 && divCol == M - 1)) {
        divRow = rand() % N;
        divCol = rand() % M;
    }
    gameBoard[0][0] = 0;
    Usedspaces[0][0][0] = ' ';
    gameBoard[N - 1][M - 1] = 0;
    Usedspaces[N - 1][M - 1][0] = ' ';
    int psnum = rand() % 10;
    int nnum = rand() % 10;
    if (!nnum) {
        gameBoard[subRow][subCol] = nnum;
    }
    else {
        gameBoard[subRow][subCol] = nnum;
        Usedspaces[subRow][subCol][0] = '-';
    }
    gameBoard[addRow][addCol] = psnum;
    Usedspaces[addRow][addCol][0] = ' ';
    gameBoard[mulRow][mulCol] = 2;
    Usedspaces[mulRow][mulCol][0] = '*';
    gameBoard[divRow][divCol] = 2;
    Usedspaces[divRow][divCol][0] = '/';
}
//Generates the gameboard
void generateGameBoard(int N, int M, vector<vector<int>>& gameBoard, vector<vector<string>>& Usedspaces) {
    int Multcap = N * M;
    srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            gameBoard[i][j] = generateCell(Multcap, Usedspaces, i, j);
        }
    }
    ensureRequiredOperations(gameBoard, N, M, Usedspaces);

}
//Does score calculating
void Calc(double& score, char sign, int num) {
    if (sign == ' ') {
        score += num;
    }
    else if (sign == '-') {
        score -= num;
    }
    else if (sign == '*') {
        score *= num;
    }
    else if (sign == '/') {
        score /= num;
    }
    else {
        cout << "Something went wrong!" << endl;
    }
}
//Checks whether a player has ran out of moves
bool IsEnd(vector<int>positionplayera, vector<vector<string>>Usedspaces, int N, int M) {
    int dx[] = { -1, -1, -1,  0,  0,  1,  1,  1 };
    int dy[] = { -1,  0,  1, -1,  1, -1,  0,  1 };
    for (int i = 0; i < 8; i++) {
        if (positionplayera[0] + dx[i] > N - 1 || positionplayera[0] + dx[i] < 0 || positionplayera[1] + dy[i]<0 || positionplayera[1] + dy[i]>M - 1) {
            continue;
        }
        if (Usedspaces[positionplayera[0] + dx[i]][positionplayera[1] + dy[i]][1] != 'b' && Usedspaces[positionplayera[0] + dx[i]][positionplayera[1] + dy[i]][1] != 'g') {
            return false;
        }
    }
    return true;
}
//Decides who the winner is
string winner(double score1, double score2) {
    if (score1 > score2) {
        return "THE WINNER IS BLUE!!!!";
    }
    else if (score2 > score1) {
        return "THE WINNER IS GREEN!!!!";
    }
    else {
        return "Looks like we have a draw :c";
    }
}
//Ensures that the coordinates are numbers or special commands and not gibberish
void IllegalCoordinates(string& a, string& b, vector<vector<int>>gb, vector<vector<string>>us, bool should = true) {
    vector<string> validator;
    while (true) {
        if (should) {
            cout << "Illegal coordinates please enter legal ones!" << endl;
        }
        string newcoor;
        getline(cin, newcoor);
        string command;
        for (char c : newcoor) {
            if (c == ' ') {
                if (!command.empty()) {
                    validator.push_back(command);
                    command.clear();
                }
            }
            else {
                command += c;
            }
        }
        if (!command.empty()) {
            validator.push_back(command);
        }
        if (validator.size() > 2 || validator.size() == 0) {
            continue;
        }
        if (validator.size() == 1) {
            if (validator[0] == "s" || validator[0] == "S") {
                savegame(N, M, gb, us, positionplayera, positionplayerb, player1, player2);
                break;
            }
            else if (validator[0] == "q" || validator[0] == "Q") {
                quitgame(N, M, gb, us, positionplayera, positionplayerb, player1, player2);
                break;
            }
            else {
                continue;
            }
        }
        else if (isNumber(validator[0]) && isNumber(validator[1])) {
            a = validator[0];
            b = validator[1];
            break;
        }
        else {
            continue;
        }
    }
    if (validator[0] == "s" || validator[0] == "S") {
        IllegalCoordinates(a, b, gb, us, false);
    }

}
//This one checks whether or not the square has already been traversed
bool IsForbidden(vector<vector<string>>Forbidden, string a, string b) {
    return Forbidden[stoi(a)][stoi(b)][1] == 'b' || Forbidden[stoi(a)][stoi(b)][1] == 'g';
}
//Coordinate validator
void IsLegal(string& a, string& b, int N, int M, vector<int>position, vector<vector<string>>Forbidden, vector<vector<int>>gb) {
    while (true) {
        while (!(stoi(a) >= 0 && stoi(b) >= 0) + !(stoi(a) <= N - 1 && stoi(b) <= M - 1) + !(position[0] - stoi(a) <= 1 && position[0] - stoi(a) >= -1) + !(position[1] - stoi(b) <= 1 && position[1] - stoi(b) >= -1) > 0) {
            IllegalCoordinates(a, b, gb, Forbidden);
        }
        if (IsForbidden(Forbidden, a, b)) {
            IllegalCoordinates(a, b, gb, Forbidden);
            continue;
        }
        break;
    }
}
//A simple movement function
void mov(string a, string b, int N, int M, vector<int>& positionplayera, vector<vector<string>>Forbidden, vector<vector<int>>gb) {
    IsLegal(a, b, N, M, positionplayera, Forbidden, gb);
    positionplayera[0] = stoi(a);
    positionplayera[1] = stoi(b);
}
//This is the turn structure
void turns(int N, int M, vector<int>& positionplayera, vector<vector<string>>& Usedspaces, vector<vector<int>>& gameBoard, double& player1, char colour) {
    vector<string> validator;
    while (true) {
        string a;
        getline(cin, a);
        string command;
        for (char c : a) {
            if (c == ' ') {
                if (!command.empty()) {
                    validator.push_back(command);
                    command.clear();
                }
            }
            else {
                command += c;
            }
        }
        if (!command.empty()) {
            validator.push_back(command);
        }
        if (validator.size() > 2 || validator.size() == 0) {
            cout << "Illegal coordinates" << endl;
            validator.clear();
            continue;
        }
        if (validator.size() == 1) {
            if (validator[0] == "s" || validator[0] == "S") {
                savegame(N, M, gameBoard, Usedspaces, positionplayera, positionplayerb, player1, player2);
                validator.clear();
            }
            else if (validator[0] == "q" || validator[0] == "Q") {
                quitgame(N, M, gameBoard, Usedspaces, positionplayera, positionplayerb, player1, player2);
            }
            else {
                cout << "Illegal coordinates" << endl;
                validator.clear();
                continue;
            }
        }
        else if (!(isNumber(validator[0]) && isNumber(validator[1]))) {
            cout << "Illegal coordinates" << endl;
            validator.clear();
            continue;
        }
        else {
            break;
        }
    }
    mov(validator[0], validator[1], N, M, positionplayera, Usedspaces, gameBoard);
    system("cls");
    Usedspaces[positionplayera[0]][positionplayera[1]][1] = colour;
    Calc(player1, Usedspaces[positionplayera[0]][positionplayera[1]][0], gameBoard[positionplayera[0]][positionplayera[1]]);

}
//This is where gameturns are decided
void gameplay(int N, int M, vector<int>positionplayera, vector<int>positionplayerb, vector<vector<string>>& Usedspaces, vector<vector<int>>& gameBoard, double player1, double player2) {
    for (int i = 0; i < N * M / 2 + 1; i++) {
        if (IsEnd(positionplayera, Usedspaces, N, M)) break;
        turns(N, M, positionplayera, Usedspaces, gameBoard, player1, 'b');
        printboard(gameBoard, N, M, Usedspaces, positionplayera, positionplayerb, player1, player2);
        if (IsEnd(positionplayerb, Usedspaces, N, M)) break;
        turns(N, M, positionplayerb, Usedspaces, gameBoard, player2, 'g');
        printboard(gameBoard, N, M, Usedspaces, positionplayera, positionplayerb, player1, player2);
    }
    cout << "\t" << winner(player1, player2) << endl;
}
//New game fuction
void newgame() {
    system("cls");
    cout << "Input gridsize to start the game:" << endl;
    while (true) {
        string grid;
        vector<string> gridsize;
        string command;
        getline(cin, grid);
        for (char c : grid) {
            if (c == ' ') {
                if (!command.empty()) {
                    gridsize.push_back(command);
                    command.clear();
                }
            }
            else {
                command += c;
            }
        }
        if (!command.empty()) {
            gridsize.push_back(command);
        }
        if (gridsize.size() != 2) {
            cout << "Invalid gridsize" << endl;
            continue;
        }
        else if (isNumber(gridsize[0]) && isNumber(gridsize[1])) {
            if (stoi(gridsize[0]) < 4 || stoi(gridsize[1]) < 4) {
                cout << "Invalid gridsize" << endl;
                continue;
            }
            else {
                N = stoi(gridsize[0]);
                M = stoi(gridsize[1]);
                break;
            }
        }
        else {
            cout << "Invalid gridsize" << endl;
            continue;
        }
    }
    vector<vector<int>> gameBoard(N, vector<int>(M));
    vector<vector<string>> Usedspaces(N, vector<string>(M, "  "));
    Usedspaces[0][0][1] = 'b';
    Usedspaces[N - 1][M - 1][1] = 'g';
    system("cls");
    generateGameBoard(N, M, gameBoard, Usedspaces);
    positionplayerb[0] = N - 1;
    positionplayerb[1] = M - 1;
    printboard(gameBoard, N, M, Usedspaces, positionplayera, positionplayerb, player1, player2);
    gameplay(N, M, positionplayera, positionplayerb, Usedspaces, gameBoard, player1, player2);
}

int main() {
    cout << "New game <n>" << endl;
    if (!isEmpty()) {
        cout << "Continue <c>" << endl;
    }
    cout << "Press <q> after the start of the game if you want to save and quit the game. Press <s> if you want to save and continue playing." << endl;
    string choice;
    getline(cin, choice);
    while (choice != "n" && choice != "N" && (!isEmpty || (choice != "c" && choice != "C"))) {
        cout << "Invalid command try again" << endl;
        getline(cin, choice);
    }
    if (choice == "n" || choice == "N") {
        newgame();
    }
    else if(choice=="c"|| choice=="C"){
        loadgame();
    }
    return 0;
}
