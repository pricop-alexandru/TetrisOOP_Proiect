#include<chrono>
#include<random>
#include<SFML/Graphics.hpp>
#include"Global.hpp"
#ifdef __linux__
#include <X11/Xlib.h>
#endif

int main()
{
    unsigned lag = 0;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    sf::Event event;
    sf::RenderWindow window(sf::VideoMode( CELL_SIZE * COLUMNS * SCREEN_RESIZE, CELL_SIZE * ROWS * SCREEN_RESIZE), "Tetris", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0,  CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));
    previous_time = std::chrono::steady_clock::now();
    while (1 == window.isOpen())
    {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
        lag += delta_time;
        while (FRAME_DURATION <= lag)
        {
            //Subtract the right thing from the left thing
            lag -= FRAME_DURATION;

            //Looping through the events
            while (1 == window.pollEvent(event))
            {
                //Check the event type
                switch (event.type)
                {
                    //If the user closed the game
                    case sf::Event::Closed:
                    {
                        //Close the window
                        window.close();

                        break;
                    }
                    default:
                        break;
                }
            }
            if (FRAME_DURATION > lag)
            {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE-1,CELL_SIZE-1));
                cell.setFillColor(sf::Color(255,0,0));
                window.clear();
                for(unsigned char a = 0; a < COLUMNS; a++)
                {
                    for(unsigned char b = 0; b < ROWS; b++)
                    {
                        cell.setPosition(CELL_SIZE * a, CELL_SIZE * b);
                        window.draw(cell);
                    }
                }
                window.display();
            }
        }
    }
}