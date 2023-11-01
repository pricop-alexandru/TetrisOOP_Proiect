#include <chrono>
#include <random>
#include <SFML/Graphics.hpp>
#include "Global.hpp"
#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main()
{
    unsigned lag = 0;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    std::vector<std::vector<unsigned char>> matrix(COLUMNS, std::vector<unsigned char>(ROWS, 0)); // Initialize the matrix with all 0s
    sf::Event event;
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * COLUMNS * SCREEN_RESIZE, CELL_SIZE * ROWS * SCREEN_RESIZE), "Tetris", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));
    previous_time = std::chrono::steady_clock::now();

    while (1 == window.isOpen())
    {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
        lag += delta_time;

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
                    case sf::Event::MouseButtonPressed:
                    {
                        if (event.mouseButton.button == sf::Mouse::Left)
                        {
                            unsigned char cell_x = event.mouseButton.x / CELL_SIZE / SCREEN_RESIZE;
                            unsigned char cell_y = event.mouseButton.y / CELL_SIZE / SCREEN_RESIZE;

                            if (cell_x < COLUMNS && cell_y < ROWS) {
                                matrix[cell_x][cell_y] = 1 - matrix[cell_x][cell_y]; //extractie binara
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            if (FRAME_DURATION > lag)
            {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                window.clear();

                for (unsigned char a = 0; a < COLUMNS; a++)
                {
                    for (unsigned char b = 0; b < ROWS; b++)
                    {
                        cell.setPosition(CELL_SIZE * a, CELL_SIZE * b);

                        if (matrix[a][b] == 1) {
                            cell.setFillColor(sf::Color(255, 0, 0)); // Change color if matrix cell is 1
                        }
                        else {
                            cell.setFillColor(sf::Color(36,36,85)); // Default color
                        }

                        window.draw(cell);
                    }
                }

                window.display();
            }
        }
    }
}