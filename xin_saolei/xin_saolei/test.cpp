#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include "tou.h"

using namespace std;

// 方向数组{左上，上，右上，左，右，左下，下，右下}
int direction_x[8] = { -1,-1,-1,0,0,1,1,1 };
int direction_y[8] = { -1,0,1,-1,1,-1,0,1 };
// 方向数组{上，下，左，右}
int direct[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };

namespace MineNamespace {
     
    //构造函数，初始化
    Board::Board(int r, int c, int n) {
        this->BoardBase::row = r;
        this->BoardBase::col = c;
        this->BoardBase::num_mines = n;

        mines.resize(row+2);  
        show.resize(row+2);
        for (int i = 0; i <= row+1; i++) {
            mines[i].resize(col+2, false);
            show[i].resize(col+2, false);
        }
        surround_mines.clear();
    }

    //检查是否越界
    bool check_yuejie(int x, int y, Board& board) {
        if (x >= 1 && x <= board.row && y >= 1 && y <= board.col) return true;
        return false;
    }

    void print_(Board& board) {
        for (int i = 1; i <= board.row; i++) {
            for (int j = 1; j <= board.col; j++) {
                if (board.mines[i][j]) cout << "@ ";
                else cout << "# ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

/********************************************************************
  Function: inputInt()
  Description: 1.输入处理函数，确保输入的是整数
  Calls: None
  Called of: playgame()
  Input: const string& 类型的 prompt，表示用户提示信息,const表述只读不能修改，常量字符串引用
  Output: None
  Return: 返回要输入的一个整数
  *******************************************************************/
  // cin.ignore(numeric_limits<streamsize>::max(), '\n')
  // 忽略输入流中的字符，直到指定的字符（例如换行符）为止。它主要用于丢弃当前输入行中的剩余数据，确保后续输入不被干扰。

    int inputInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail()) {
                cin.clear();   //清除输入流的错误状态标志
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效，请输入一个整数。" << endl;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

/********************************************************************
  Function: placemines()
  Description: 1.在合法且非第一次输入的坐标处埋指定数量个雷
               2.计算非雷坐标周围8个格子有几个雷，存储在surround_mines数组中
  Calls: srand()
         rand()
         check_yuejie()
  Called of: playgame()
*********************************************************************/

        void placemines(Board& board, int x_first, int y_first) {
        srand(time(NULL));
        int placed = 0;
        while (placed < board.num_mines) {
            int x = rand() % board.row + 1; //确保x在[1,row]
            int y = rand() % board.col + 1; //确保y在[1,col]
            if ((x == x_first && y == y_first) || board.mines[x][y]) continue;
            board.mines[x][y] = true;
            placed++;
        }

        print_(board);

        for (int i = 1; i <= board.row; i++) {
            for (int j = 1; j <= board.col; j++) {
                if (board.mines[i][j]) continue;
                int mine_count = 0;
                for (int k = 0; k < 8; k++) {
                    int dx = i + direction_x[k];
                    int dy = j + direction_y[k];
                    if (check_yuejie(dx, dy, board) && board.mines[dx][dy]) {
                        mine_count++;
                    }
                }
                pair<int, int> p(i, j);
                board.surround_mines[p] = mine_count;
            }
        }
    }
        /********************************************************************
          Function: digui()
          Description: 1.打印传入坐标周围四格中合法且未显示过的格子信息
                       2.如果周围4格中有周围8格无雷的格子，利用递归继续显示其周围4格信息
          Calls: digui()
          Called of: deal()
        *********************************************************************/

    void digui(Board& board, int x, int y) {
        for (int i = 0; i < 4; i++) {
            int dx = x + direct[i][0];
            int dy = y + direct[i][1];
            if (check_yuejie(dx, dy, board) && (!board.show[dx][dy])) {
                board.show[dx][dy] = true;
                std::pair<int, int> p(dx, dy);
                if (board.surround_mines.find(p) != board.surround_mines.end() && board.surround_mines[p] == 0) {
                    digui(board, dx, dy);
                }
            }
        }
    }
/********************************************************************
  Function: deal()
  Description: 1.判断要点击的坐标是否越界或已显示过
               2.标记点击坐标
               3.判断点击坐标是否为雷
               4.如何点击坐标周围8格无雷，调用digui以显示周围4格的信息
  Calls: check_yuejie()
         digui()
  Called of: playgame()
  Return: 返回0，1，2
          0 代表点击坐标为雷
          1 代表点击坐标越界或已显示过
          2 代表点击坐标非雷未显示且合法
*********************************************************************/

    int deal(Board& board, int x, int y) {
        if (!check_yuejie(x, y, board)) {
            cout << "输入的位置坐标越界\n";
            return 1;
        }
        if (board.show[x][y]) {
            cout << "已经显示过该坐标了\n";
            return 1;
        }
        board.show[x][y] = true;
        if (board.mines[x][y]) {
            cout << "踩到雷了!游戏失败!" << endl;
            return 0;
        }
        pair<int, int> p(x, y);
        if (board.surround_mines.find(p) != board.surround_mines.end() && board.surround_mines[p] == 0) {
            digui(board, x, y);
        }
        return 2;
    }

    void display(Board& board) {
        for (int i = 1; i <= board.row; i++) {
            for (int j = 1; j <= board.col; j++) {
                if (board.show[i][j]) {
                    pair<int, int> p(i, j);
                    if (board.surround_mines.find(p) != board.surround_mines.end()) {
                        cout << board.surround_mines[p] << " ";
                    }
                    else {
                        cout << "* ";
                    }
                }
                else {
                    cout << "* ";
                }
            }
            cout << "\n";
        }
    }

    bool check_win(Board& board) {
        for (int i = 1; i <= board.row; i++) {
            for (int j = 1; j <= board.col; j++) {
                if (!board.show[i][j] && !board.mines[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
/********************************************************************
  Function: playgame()
  Description: 1.输入第一次要点击坐标后埋雷（保证第一次不会踩到雷）
               2.循环处理每次点击
  Calls: placemines()
         deal()
         print_()
         display()
         check_win()
  Called of: game()
  Input: qipan型指向自己开辟的内存的指针board
  Output: board指向结构体中mines,show,surround_mines数组
  Return: None
*********************************************************************/

    void playgame(Board& board) {
        int x_first, y_first;
        display(board);
        int check_ = 0;
        do {
            x_first = inputInt("请输入 x 坐标：");
            y_first = inputInt("请输入 y 坐标：");
            check_ = check_yuejie(x_first, y_first, board);
            if (check_) break;
            else cout << "输入的位置坐标越界，请重新输入" << endl;
        } while (!check_);

        placemines(board, x_first, y_first);
        int x = x_first;
        int y = y_first;
        while (1) {
            int flag = deal(board, x, y);
            if (!flag) {
                print_(board);
                break;
            }
            if (flag == 2) {
                display(board);
            }
            if (check_win(board)) {
                cout << "胜利了!" << endl;
                print_(board);
                break;
            }
            x = inputInt("请输入 x 坐标：");
            y = inputInt("请输入 y 坐标：");
        }
    }

    void game() {
        int row, col, num_mines;
        row = inputInt("请输入行数：");
        if (row <= 0 || row >= 1000) {
            cout << "输入错误，请重新输入\n";
            return;
        }
        col = inputInt("请输入列数：");
        if (col <= 0 || col >= 1000) {
            cout << "输入错误，请重新输入\n";
            return;
        }
        num_mines = inputInt("请输入雷的数量：");
        if (num_mines <= 0 || num_mines >= row * col) {
            cout << "输入错误，请重新输入\n";
            return;
        }

        Board board(row, col, num_mines);
        playgame(board);
    }

    void test() {
        int choice = 0;
        do {
            cout << "*************************************\n";
            cout << "********  1. play   0.exit  *********\n";
            cout << "*************************************\n";

            cout << "请选择" << endl;
            choice = inputInt("请输入选择：");
            switch (choice) {
            case 1:
                game();
                break;
            case 0:
                cout << "退出游戏\n" << endl;
                exit(0);
                break;
            default:
                cout << "无效输入，请重新输入，只能输入 0 和 1 两个数字中的一个\n";
                break;
            }
        } while (choice);
    }
}
int main() {
    MineNamespace::test();
    return 0;
}