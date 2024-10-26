#pragma once

#include <iostream>
#include <vector>
#include <map>

namespace MineNamespace {

    class BoardBase {
    public:
        int row;  //行数
        int col;  //列数
        int num_mines;  //雷数
    };

    class Board : public BoardBase {
    public:
        std::vector<std::vector<bool>> mines;  // 存储雷的位置
        std::vector<std::vector<bool>> show;  // 记录已经显示的格子
        std::map<std::pair<int, int>, int> surround_mines;  // 存储每个格子周围雷的数量

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