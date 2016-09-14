#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Hetman {
public:
    int y;
    int x;
    long long val;
    Hetman(int y, int x, long long int val) : y(y), x(x), val(val) {}

    bool isEqualTo(Hetman hetman) {
        return Hetman::x == hetman.x && Hetman::y == hetman.y;
    }
};

class Move {
public:
    int from_y;
    int from_x;
    int to_y;
    int to_x;
    
    Move(int from_y, int from_x, int to_y, int to_x) : from_y(from_y), from_x(from_x), to_y(to_y), to_x(to_x) {}

    string toString() {
        return to_string(from_y) + " " + to_string(from_x) + " " + to_string(to_y) + " " + to_string(to_x);
    }
};

int boardSize;
int hetmanNumberTarget;
int numOfHetmans;
int directions[16] = {-1, -1, -1, 0, 0, 1, 1, 1, // 8 moves possible, each with 2 coords
                      -1, 0, 1, -1, 1, -1, 0, 1};

long long **board;
stack<Move> moves;
void init(vector<Hetman> &listOfHetmans);
void addHetmanToList(Hetman hetman, vector<Hetman> *hetmanList);
Hetman findFirstTargetInLine(int from_y, int from_x, int direction_y, int direction_x);
bool solve(int hetmansNumber, vector<Hetman> listOfHetmans);
void printAnswer();

int main() {
    vector<Hetman> inputListOfHetmans;
    init(inputListOfHetmans);
    if (solve(numOfHetmans, inputListOfHetmans)) printAnswer();
    return 0;
}

bool operator<(Hetman const &a, Hetman const &b) {
    return a.val < b.val;
}

void init(vector<Hetman> &listOfHetmans) {
    scanf("%d", &boardSize);
    scanf("%d", &hetmanNumberTarget);
    numOfHetmans = 0;

    board = new long long *[boardSize];
    for (int i = 0; i < boardSize; i++) {
        board[i] = new long long[boardSize];
        for (int j = 0; j < boardSize; j++) {
            scanf("%lld", &board[i][j]);
            if (board[i][j] > 0) {
                numOfHetmans++;
                addHetmanToList(Hetman(i, j, board[i][j]), &listOfHetmans);
            }
        }
    }

}

void addHetmanToList(Hetman hetman, vector<Hetman> *hetmanList) {
    vector<Hetman>::iterator it = hetmanList->begin();
    while(it != hetmanList->end() && (*it).val <= hetman.val) it++;
    hetmanList->insert(it, hetman);
}

Hetman findFirstTargetInLine(int from_y, int from_x, int direction_y, int direction_x) {
    while(from_x >= 0 && from_x < boardSize && from_y >= 0 && from_y < boardSize) {
        if (board[from_y][from_x] > 0) {
            return Hetman(from_y, from_x, board[from_y][from_x]);
        }
        from_x += direction_x;
        from_y += direction_y;
    }
    return Hetman(-1, -1, 0);
}

bool solve(int hetmansNumber, vector<Hetman> hetmanList) {
    if (hetmansNumber <= hetmanNumberTarget) return true;
    while (!hetmanList.empty()) {
        Hetman first = hetmanList.front();
        hetmanList.erase(hetmanList.begin());
        for (int i = 0; i < 8; i++) {
            Hetman target = findFirstTargetInLine(first.y + directions[i], first.x + directions[8 + i], directions[i], directions[8 + i]);
            if (target.val == first.val) {
                vector<Hetman> newHetmanList;
                for(vector<Hetman>::iterator it = hetmanList.begin(); it != hetmanList.end(); it++) {
                    if((*it).isEqualTo(target)) continue;
                    newHetmanList.push_back(*it);
                }
                board[first.y][first.x] = 0;
                board[target.y][target.x] *= 2;
                addHetmanToList(Hetman(target.y, target.x, target.val*2), &newHetmanList);
                if (solve(hetmansNumber - 1, newHetmanList)) {
                    moves.push(Move(first.y, first.x, target.y, target.x));
                    return true;
                }

                board[first.y][first.x] = first.val;
                board[target.y][target.x] = target.val;
            }
        }
    }
    return false;
}

void printAnswer() {
    while (!moves.empty()) {
        Move top = moves.top();
        printf("%s\n", top.toString().c_str());
        moves.pop();
    }
}