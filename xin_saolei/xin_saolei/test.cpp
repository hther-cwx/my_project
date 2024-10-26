#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include "tou.h"

using namespace std;

// ��������{���ϣ��ϣ����ϣ����ң����£��£�����}
int direction_x[8] = { -1,-1,-1,0,0,1,1,1 };
int direction_y[8] = { -1,0,1,-1,1,-1,0,1 };
// ��������{�ϣ��£�����}
int direct[4][2] = { {-1,0},{1,0},{0,-1},{0,1} };

namespace MineNamespace {
     
    //���캯������ʼ��
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

    //����Ƿ�Խ��
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
  Description: 1.���봦������ȷ�������������
  Calls: None
  Called of: playgame()
  Input: const string& ���͵� prompt����ʾ�û���ʾ��Ϣ,const����ֻ�������޸ģ������ַ�������
  Output: None
  Return: ����Ҫ�����һ������
  *******************************************************************/
  // cin.ignore(numeric_limits<streamsize>::max(), '\n')
  // �����������е��ַ���ֱ��ָ�����ַ������绻�з���Ϊֹ������Ҫ���ڶ�����ǰ�������е�ʣ�����ݣ�ȷ���������벻�����š�

    int inputInt(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            cin >> value;
            if (cin.fail()) {
                cin.clear();   //����������Ĵ���״̬��־
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "������Ч��������һ��������" << endl;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return value;
            }
        }
    }

/********************************************************************
  Function: placemines()
  Description: 1.�ںϷ��ҷǵ�һ����������괦��ָ����������
               2.�������������Χ8�������м����ף��洢��surround_mines������
  Calls: srand()
         rand()
         check_yuejie()
  Called of: playgame()
*********************************************************************/

        void placemines(Board& board, int x_first, int y_first) {
        srand(time(NULL));
        int placed = 0;
        while (placed < board.num_mines) {
            int x = rand() % board.row + 1; //ȷ��x��[1,row]
            int y = rand() % board.col + 1; //ȷ��y��[1,col]
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
          Description: 1.��ӡ����������Χ�ĸ��кϷ���δ��ʾ���ĸ�����Ϣ
                       2.�����Χ4��������Χ8�����׵ĸ��ӣ����õݹ������ʾ����Χ4����Ϣ
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
  Description: 1.�ж�Ҫ����������Ƿ�Խ�������ʾ��
               2.��ǵ������
               3.�жϵ�������Ƿ�Ϊ��
               4.��ε��������Χ8�����ף�����digui����ʾ��Χ4�����Ϣ
  Calls: check_yuejie()
         digui()
  Called of: playgame()
  Return: ����0��1��2
          0 ����������Ϊ��
          1 ����������Խ�������ʾ��
          2 �������������δ��ʾ�ҺϷ�
*********************************************************************/

    int deal(Board& board, int x, int y) {
        if (!check_yuejie(x, y, board)) {
            cout << "�����λ������Խ��\n";
            return 1;
        }
        if (board.show[x][y]) {
            cout << "�Ѿ���ʾ����������\n";
            return 1;
        }
        board.show[x][y] = true;
        if (board.mines[x][y]) {
            cout << "�ȵ�����!��Ϸʧ��!" << endl;
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
  Description: 1.�����һ��Ҫ�����������ף���֤��һ�β���ȵ��ף�
               2.ѭ������ÿ�ε��
  Calls: placemines()
         deal()
         print_()
         display()
         check_win()
  Called of: game()
  Input: qipan��ָ���Լ����ٵ��ڴ��ָ��board
  Output: boardָ��ṹ����mines,show,surround_mines����
  Return: None
*********************************************************************/

    void playgame(Board& board) {
        int x_first, y_first;
        display(board);
        int check_ = 0;
        do {
            x_first = inputInt("������ x ���꣺");
            y_first = inputInt("������ y ���꣺");
            check_ = check_yuejie(x_first, y_first, board);
            if (check_) break;
            else cout << "�����λ������Խ�磬����������" << endl;
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
                cout << "ʤ����!" << endl;
                print_(board);
                break;
            }
            x = inputInt("������ x ���꣺");
            y = inputInt("������ y ���꣺");
        }
    }

    void game() {
        int row, col, num_mines;
        row = inputInt("������������");
        if (row <= 0 || row >= 1000) {
            cout << "�����������������\n";
            return;
        }
        col = inputInt("������������");
        if (col <= 0 || col >= 1000) {
            cout << "�����������������\n";
            return;
        }
        num_mines = inputInt("�������׵�������");
        if (num_mines <= 0 || num_mines >= row * col) {
            cout << "�����������������\n";
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

            cout << "��ѡ��" << endl;
            choice = inputInt("������ѡ��");
            switch (choice) {
            case 1:
                game();
                break;
            case 0:
                cout << "�˳���Ϸ\n" << endl;
                exit(0);
                break;
            default:
                cout << "��Ч���룬���������룬ֻ������ 0 �� 1 ���������е�һ��\n";
                break;
            }
        } while (choice);
    }
}
int main() {
    MineNamespace::test();
    return 0;
}