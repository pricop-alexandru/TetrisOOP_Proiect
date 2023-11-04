//
// Created by Alex Pricop on 11/2/2023.
//
#include <vector>
#include "Global.hpp"
#include "Tetromino.hpp"
#include "GetTetromino.hpp"
#include "GetWallKickData.hpp"

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

void Tetromino::hard_drop(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    minos = get_ghost_minos(i_matrix);
}

void Tetromino::move_left(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    for (Position& mino : minos)
    {
        if (0 > mino.x - 1)
        {
            return;
        }
        if (0 > mino.y)
        {
            continue;
        }
        else if (0 < i_matrix[mino.x - 1][mino.y])
        {
            return;
        }
    }
    for (Position& mino : minos)
    {
        mino.x--;
    }
}

void Tetromino::move_right(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    for (Position& mino : minos)
    {
        if (COLUMNS == 1 + mino.x)
        {
            return;
        }
        if (0 > mino.y)
        {
            continue;
        }
        else if (0 < i_matrix[1 + mino.x][mino.y])
        {
            return;
        }
    }

    for (Position& mino : minos)
    {
        mino.x++;
    }
}

void Tetromino::rotate(bool i_clockwise, const std::vector<std::vector<unsigned char>>& i_matrix)
{
    //asta a fost partea dureroasa
    if (3 != shape) //oare pt ca cea rotunda nu se roteste?
    {
        unsigned char next_rotation;
        std::vector<Position> current_minos = minos;
        //rotatia urmatoare
        if (0 == i_clockwise)
        {
            next_rotation = (3 + rotation) % 4;
        }
        else
        {
            next_rotation = (1 + rotation) % 4;
        }
        if (0 == shape)
        {
            float center_x = 0.5f * (minos[1].x + minos[2].x);
            float center_y = 0.5f * (minos[1].y + minos[2].y);

            switch (rotation)
            {
                case 0:
                {
                    center_y += 0.5f;

                    break;
                }
                case 1:
                {
                    center_x -= 0.5f;

                    break;
                }
                case 2:
                {
                    center_y -= 0.5f;

                    break;
                }
                case 3:
                {
                    center_x += 0.5f;
                }
            }//folosim centrul relativ al piesei i pentru toate
            for (Position& mino : minos)
            {
                float x = mino.x - center_x;
                float y = mino.y - center_y;

                if (0 == i_clockwise)
                {
                    mino.x = static_cast<char>(center_x + y);
                    mino.y = static_cast<char>(center_y - x);
                }
                else
                {
                    mino.x = static_cast<char>(center_x - y);
                    mino.y = static_cast<char>(center_y + x);
                }
            }
        }
        else
        {//acelasi lucru doar ca pentru T de exemplu, nu rotim blocul din centru
            for (unsigned char a = 1; a < minos.size(); a++)
            {
                char x = minos[a].x - minos[0].x;
                char y = minos[a].y - minos[0].y;

                if (0 == i_clockwise)
                {
                    minos[a].x = y + minos[0].x;
                    minos[a].y = minos[0].y - x;
                }
                else
                {
                    minos[a].x = minos[0].x - y;
                    minos[a].y = x + minos[0].y;
                }
            }
        }
        for (Position& wall_kick : get_wall_kick_data(0 == shape, rotation, next_rotation))
        {
            bool can_turn = 1;

            for (Position& mino : minos)
            {
                if (0 > mino.x + wall_kick.x || COLUMNS <= mino.x + wall_kick.x || ROWS <= mino.y + wall_kick.y)//verificam fiecare coliziune
                {
                    can_turn = 0;

                    break;
                }

                if (0 > mino.y + wall_kick.y)
                {
                    continue;
                }
                else if (0 < i_matrix[mino.x + wall_kick.x][mino.y + wall_kick.y])
                {
                    can_turn = 0;

                    break;
                }
            }
            if (1 == can_turn)
            {
                rotation = next_rotation;

                for (Position& mino : minos)
                {
                    mino.x += wall_kick.x;
                    mino.y += wall_kick.y;
                }

                return;
            }
        }
        minos = current_minos;
    }
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
    }//aici cand si-a terminat traseul, tetromino-ul devine parte din matrice
}
std::vector<Position> Tetromino::get_ghost_minos(const std::vector<std::vector<unsigned char>>& i_matrix)
{
    bool keep_falling = 1;//preview-ul de tetromino e doar el in sine mutat in jos de tot
    unsigned char total_movement = 0;
    std::vector<Position> ghost_minos = minos;
    while (1 == keep_falling)
    {
        total_movement++;
        for (Position& mino : minos)
        {
            if (ROWS == total_movement + mino.y)
            {
                keep_falling = 0;
                break;
            }
            if (0 > total_movement + mino.y)
            {
                continue;
            }
            else if (0 < i_matrix[mino.x][total_movement + mino.y])
            {
                keep_falling = 0;
                break;
            }
        }
    }
    for (Position& mino : ghost_minos)
    {
        mino.y += total_movement - 1;
    }

    return ghost_minos;
}
std::vector<Position> Tetromino::get_minos()
{
    return minos;
}