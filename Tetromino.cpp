//
// Created by Alex Pricop on 11/2/2023.
//
#include <vector>
#include "Global.hpp"
#include "Tetromino.hpp"
#include "GetTetromino.hpp"

Tetromino::Tetromino(unsigned char i_shape, const std::vector<std::vector<unsigned char>>& i_matrix) :
        rotation(0),
        shape(i_shape),
        minos(get_tetromino(i_shape, COLUMNS / 2, 1)){}
bool Tetromino::move_down(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    for (Position& mino : minos)
    {
        if (ROWS == 1 + mino.y)
        {
            return 0;
        }
        if (0 < i_matrix[mino.x][1 + mino.y])
        {
            return 0;
        }
    }
    for (Position& mino : minos)
    {
        mino.y++;
    }
    return 1;
}

bool Tetromino::reset(unsigned char i_shape, const std::vector<std::vector<unsigned char>>& i_matrix)
{
    rotation = 0;
    shape = i_shape;

    minos = get_tetromino(shape, COLUMNS / 2, 1);

    for (Position& mino : minos)
    {
        if (0 < i_matrix[mino.x][mino.y])
        {
            return 0;
        }
    }
    return 1;
}

unsigned char Tetromino::get_shape()
{
    return shape;
}
void Tetromino::update_matrix(std::vector<std::vector<unsigned char>>& i_matrix)
{
    for (Position& mino : minos)
    {
        if (0 > mino.y)
        {
            continue;
        }

        i_matrix[mino.x][mino.y] = 1 + shape;
    }
}
std::vector<Position> Tetromino::get_minos()
{
    return minos;
}