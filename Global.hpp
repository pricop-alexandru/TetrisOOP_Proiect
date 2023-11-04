//
// Created by Alex Pricop on 10/31/2023.
//
#pragma once

constexpr unsigned char CELL_SIZE = 8;
constexpr unsigned char CLEAR_EFFECT_DURATION = 8;
constexpr unsigned char COLUMNS = 10;
constexpr unsigned char ROWS = 20;
constexpr unsigned char SCREEN_RESIZE = 4;
constexpr unsigned char MOVE_SPEED = 4;
constexpr unsigned char LINES_TO_INCREASE_SPEED = 2;
constexpr unsigned char START_FALL_SPEED = 32;
constexpr unsigned char SOFT_DROP_SPEED = 4;
constexpr unsigned short FRAME_DURATION = 16667;
struct Position
{
    char x;
    char y;
};