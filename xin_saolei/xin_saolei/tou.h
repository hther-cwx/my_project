#pragma once

#include <iostream>
#include <vector>
#include <map>

namespace MineNamespace {

    class BoardBase {
    public:
        int row;  //����
        int col;  //����
        int num_mines;  //����
    };

    class Board : public BoardBase {
    public:
        std::vector<std::vector<bool>> mines;  // �洢�׵�λ��
        std::vector<std::vector<bool>> show;  // ��¼�Ѿ���ʾ�ĸ���
        std::map<std::pair<int, int>, int> surround_mines;  // �洢ÿ��������Χ�׵�����

        Board(int r, int c, int n);
    };

    bool check_yuejie(int x, int y, Board& board);
    void print_(Board& board);
    int inputInt(const std::string& prompt);
    void placemines(Board& board, int x_first, int y_first);
    void digui(Board& board, int x, int y);
    int deal(Board& board, int x, int y);
    void display(Board& board);
    bool check_win(Board& board);
    void playgame(Board& board);
    void game();
    void test();
}