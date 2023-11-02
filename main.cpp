#include <chrono>
#include <random>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Global.hpp"
#include "Tetromino.hpp"
#include "GetTetromino.hpp"
#ifdef __linux__
#include <X11/Xlib.h>
#endif
int main()
{
    unsigned lag = 0;
    unsigned char next_shape;
    unsigned char fall_timer = 0;
    unsigned char current_fall_speed = START_FALL_SPEED;
    unsigned char clear_effect_timer = 0;
    unsigned char soft_drop_timer = 0;

    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_int_distribution<unsigned short> shape_distribution(0, 6);
    std::vector<sf::Color> cell_colors = {
            sf::Color(36, 36, 85),
            sf::Color(0, 219, 255),
            sf::Color(0, 36, 255),
            sf::Color(255, 146, 0),
            sf::Color(255, 219, 0),
            sf::Color(0, 219, 0),
            sf::Color(146, 0, 255),
            sf::Color(219, 0, 0),
            sf::Color(73, 73, 85)
    };
    std::vector<std::vector<unsigned char>> matrix(COLUMNS, std::vector<unsigned char>(ROWS, 0));
    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * COLUMNS * SCREEN_RESIZE, CELL_SIZE * ROWS * SCREEN_RESIZE), "Tetris", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));
    Tetromino tetromino(static_cast<unsigned char>(shape_distribution(random_engine)), matrix);
    next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
    previous_time = std::chrono::steady_clock::now();

    while (1 == window.isOpen())
    {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
        lag += delta_time;
        previous_time += std::chrono::microseconds(delta_time);
        while (FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION;

            while (1 == window.pollEvent(event))
            {
                switch (event.type)
                {
                    case sf::Event::Closed:
                    {
                        window.close();
                        break;
                    }
                    default:
                        break;
                }
            }
            if (0 == clear_effect_timer)
            {
                if (current_fall_speed == fall_timer)
                {

                    if (0 == tetromino.move_down(matrix))
                    {
                        tetromino.update_matrix(matrix);
                            next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
                    }
                fall_timer = 0;
                }
                else
                {
                    fall_timer++;
                }
            }
            else
            {
                next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
            }
            if (FRAME_DURATION > lag)
            {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                window.clear();

                for (unsigned char a = 0; a < COLUMNS; a++)
                    for (unsigned char b = 0; b < ROWS; b++)
                    {
                        cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
                        cell.setFillColor(cell_colors[matrix[a][b]]);
                        window.draw(cell);
                    }
                cell.setFillColor(cell_colors[1 + tetromino.get_shape()]);
                for (Position& mino : tetromino.get_minos())
                {
                    cell.setPosition(static_cast<float>(CELL_SIZE * mino.x), static_cast<float>(CELL_SIZE * mino.y));

                    window.draw(cell);
                }
                cell.setFillColor(cell_colors[1 + next_shape]);
                cell.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                window.display();
            }
        }
    }
}