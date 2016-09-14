#include <iostream>
#include <vector>
#include <stack>
#include <string>

int boardSize;
int hetmanNumberTarget;
int numOfHetmans = 0;
int directions[16] = {-1, -1, -1, 0, 0, 1, 1, 1, -1, 0, 1, -1, 1, -1, 0, 1};
long long **board;

class Hetman {
private:
    int y;
    int x;
    long long val;
public:
    Hetman(int y, int x, long long int val) : y(y), x(x), val(val) {}

    int getX() const {
        return x;
    }

    void setX(int x) {
        Hetman::x = x;
    }

    int getY() const {
        return y;
    }

    void setY(int y) {
        Hetman::y = y;
    }

    long long int getVal() const {
        return val;
    }

    void setVal(long long int val) {
        Hetman::val = val;
    }

    bool isEqualTo(Hetman hetman) {
        return Hetman::x == hetman.x && Hetman::y == hetman.y;
    }
};

class Move {
private:
    int from_y;
    int from_x;
    int to_y;
    int to_x;
public:
    Move(int from_y, int from_x, int to_y, int to_x) : from_y(from_y), from_x(from_x), to_y(to_y), to_x(to_x) {}

    int getFrom_y() const {
        return from_y;
    }

    void setFrom_y(int from_y) {
        Move::from_y = from_y;
    }

    int getFrom_x() const {
        return from_x;
    }

    void setFrom_x(int from_x) {
        Move::from_x = from_x;
    }

    int getTo_y() const {
        return to_y;
    }

    void setTo_y(int to_y) {
        Move::to_y = to_y;
    }

    int getTo_x() const {
        return to_x;
    }

    void setTo_x(int to_x) {
        Move::to_x = to_x;
    }

    std::string toString() {
        return std::to_string(from_y) + " " + std::to_string(from_x) + " " + std::to_string(to_y) + " " + std::to_string(to_x);
    }
};

std::stack<Move> moves;
void init(std::vector<Hetman> &listOfHetmans);
void addHetmanToList(Hetman hetman, std::vector<Hetman> hetmanList);
Hetman findTarget(int from_y, int from_x, int direction_y, int direction_x);
bool solve(int numOfHetmans, std::vector<Hetman> listOfHetmans);

int main() {
    std::vector<Hetman> listOfHetmans;
    init(listOfHetmans);
    if (solve(numOfHetmans, listOfHetmans)) {
        while(!moves.empty()) {
            Move top = moves.top();
            std::cout << top.toString() << std::endl;
            moves.pop();
        }
    }
    return 0;
}

bool operator<(Hetman const &a, Hetman const &b) {
    return a.getVal() < b.getVal();
}

void init(std::vector<Hetman> &listOfHetmans) {
    std::cin >> boardSize;
    std::cin >> hetmanNumberTarget;

    board = new long long *[boardSize];
    for (int i = 0; i < boardSize; i++) {
        board[i] = new long long[boardSize];
        for (int j = 0; j < boardSize; j++) {
            std::cin >> board[i][j];
            if (board[i][j] > 0) {
                numOfHetmans++;
                listOfHetmans.push_back(Hetman(i, j, board[i][j]));
            }
        }
    }

    std::sort(listOfHetmans.begin(), listOfHetmans.end());
}

void addHetmanToList(Hetman hetman, std::vector<Hetman> *hetmanList) {
    std::vector<Hetman>::iterator it;
    for (it = hetmanList->begin(); it != hetmanList->end(); it++) {
        if ((*it).getVal() > hetman.getVal()) {
            hetmanList->insert(it, hetman);
            return;
        }
    }
    hetmanList->insert(it, hetman);
}

Hetman findTarget(int from_y, int from_x, int direction_y, int direction_x) {
    from_x += direction_x;
    from_y += direction_y;

    while(from_x >= 0 && from_x < boardSize && from_y >= 0 && from_y < boardSize) {
        if (board[from_y][from_x] > 0) {
            return Hetman(from_y, from_x, board[from_y][from_x]);
        }
        from_x += direction_x;
        from_y += direction_y;
    }
    return Hetman(-1, -1, 0);
}

bool solve(int numOfHetmans, std::vector<Hetman> hetmanList) {
    if (numOfHetmans <= hetmanNumberTarget) return true;
    while (!hetmanList.empty()) {
        Hetman first = hetmanList.front();
        hetmanList.erase(hetmanList.begin());
        for (int i = 0; i < 8; i++) {
            Hetman target = findTarget(first.getY(), first.getY(), directions[i], directions[8 + i]);
            if (target.getVal() == first.getVal()) {
                std::vector<Hetman> newHetmanList;
                for(std::vector<Hetman>::iterator it = hetmanList.begin(); it != hetmanList.end(); it++) {
                    if((*it).isEqualTo(target)) continue;
                    newHetmanList.push_back(*it);
                }
                board[first.getY()][first.getX()] = 0;
                board[target.getY()][target.getX()] *= 2;
                addHetmanToList(Hetman(target.getY(), target.getX(), target.getVal()*2), &newHetmanList);
                if (solve(numOfHetmans - 1, newHetmanList)) {
                    moves.push(Move(first.getY(), first.getX(), target.getY(), target.getX()));
                    return true;
                }

                board[first.getY()][first.getX()] = first.getVal();
                board[target.getY()][target.getX()] = target.getVal();
            }
        }
    }
    return false;
}