#include<chrono>
#include<random>
#include<SFML/Graphics.hpp>

#include "Global.hpp"
#include "Tetromino.hpp"
#include "GetTetromino.hpp"
#include "scoresystem.hpp"
int main() {
    bool game_over = 0;
    bool hard_drop_pressed = 0;
    bool rotate_pressed = 0;
    unsigned lag = 0;
    unsigned lines_cleared = 0;
    unsigned char clear_effect_timer = 0;
    unsigned char current_fall_speed = START_FALL_SPEED;
    unsigned char fall_timer = 0;
    unsigned char move_timer = 0;
    unsigned char next_shape;
    unsigned char soft_drop_timer = 0;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    std::random_device random_device;
    std::default_random_engine random_engine(random_device());
    std::uniform_int_distribution<unsigned short> shape_distribution(0, 6);
    std::vector<bool> clear_lines(ROWS, 0);
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
    sf::Font font;
    if (!font.loadFromFile("../assets/Retro_Gaming.ttf")) {
        std::cout << "Error loading font\n";
    }
    std::vector<std::vector<unsigned char>> matrix(COLUMNS, std::vector<unsigned char>(ROWS));
    sf::Event event{};
    sf::RenderWindow window(sf::VideoMode(2 * CELL_SIZE * COLUMNS * SCREEN_RESIZE, CELL_SIZE * ROWS * SCREEN_RESIZE), "Tetris", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, 2 * CELL_SIZE * COLUMNS, CELL_SIZE * ROWS)));
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
                    case sf::Event::KeyReleased:
                    {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::C:
                            case sf::Keyboard::Z:
                            {
                                rotate_pressed = 0;
                                break;
                            }
                            case sf::Keyboard::Down:
                            {
                                soft_drop_timer = 0;
                                break;
                            }
                            case sf::Keyboard::Left:
                            case sf::Keyboard::Right:
                            {
                                move_timer = 0;
                                break;
                            }
                            case sf::Keyboard::Space:
                            {
                                hard_drop_pressed = 0;
                            }
                            default:
                                continue;
                        }
                    }
                    default:
                        continue;
                }
            }
            if (0 == clear_effect_timer)
            {
                if (0 == game_over) {
                    if (0 == rotate_pressed) {
                        if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
                            rotate_pressed = 1;
                            tetromino.rotate(1, matrix);
                        } else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                            rotate_pressed = 1;
                            tetromino.rotate(0, matrix);
                        }
                    }
                    if (0 == move_timer) {
                        if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                            move_timer = 1;
                            tetromino.move_left(matrix);
                        } else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                            move_timer = 1;
                            tetromino.move_right(matrix);
                        }
                    } else {
                        move_timer = (1 + move_timer) % MOVE_SPEED;
                    }
                    if (0 == hard_drop_pressed) {
                        if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                            hard_drop_pressed = 1;
                            fall_timer = current_fall_speed;
                            tetromino.hard_drop(matrix);
                        }
                    }
                    if (0 == soft_drop_timer) {
                        if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                            if (1 == tetromino.move_down(matrix)) {
                                fall_timer = 0;
                                soft_drop_timer = 1;
                            }
                        }
                    } else {
                        soft_drop_timer = (1 + soft_drop_timer) % SOFT_DROP_SPEED;
                    }
                    if (current_fall_speed == fall_timer) {
                        if (0 == tetromino.move_down(matrix)) {
                            tetromino.update_matrix(matrix);
                            for (unsigned char a = 0; a < ROWS; a++) {
                                bool clear_line = 1;
                                for (unsigned char b = 0; b < COLUMNS; b++) {
                                    if (0 == matrix[b][a]) {
                                        clear_line = 0;
                                        break;
                                    }
                                }
                                if (1 == clear_line) {
                                    lines_cleared++;
                                    clear_effect_timer = CLEAR_EFFECT_DURATION;
                                    clear_lines[a] = 1;
                                    if (0 == lines_cleared % LINES_TO_INCREASE_SPEED) {
                                        current_fall_speed = std::max<unsigned char>(SOFT_DROP_SPEED,
                                                                                     current_fall_speed - 1);
                                    }
                                }
                            }
                            if (0 == clear_effect_timer) {
                                game_over = 0 == tetromino.reset(next_shape, matrix);
                                next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
                            }
                        }
                        fall_timer = 0;
                    }
                    else
                    {
                        fall_timer++;
                    }
                }
                else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    game_over = 0;
                    hard_drop_pressed = 0;
                    rotate_pressed = 0;
                    lines_cleared = 0;
                    current_fall_speed = START_FALL_SPEED;
                    fall_timer = 0;
                    move_timer = 0;
                    soft_drop_timer = 0;
                    for (std::vector<unsigned char>& a : matrix)
                    {
                        std::fill(a.begin(), a.end(), 0);
                    }
                }
            }
            else
            {
                clear_effect_timer--;
                if (0 == clear_effect_timer)
                {
                    for (unsigned char a = 0; a < ROWS; a++)
                    {
                        if (1 == clear_lines[a])
                        {
                            for (unsigned char b = 0; b < COLUMNS; b++)
                            {
                                matrix[b][a] = 0;
                                for (unsigned char c = a; 0 < c; c--)
                                {
                                    matrix[b][c] = matrix[b][c - 1];
                                    matrix[b][c - 1] = 0;
                                }
                            }
                        }
                    }
                    game_over = 0 == tetromino.reset(next_shape, matrix);
                    next_shape = static_cast<unsigned char>(shape_distribution(random_engine));
                    std::fill(clear_lines.begin(), clear_lines.end(), 0);
                }
            }
            if (FRAME_DURATION > lag)
            {
                unsigned char clear_cell_size = static_cast<unsigned char>(2 * round(0.5f * CELL_SIZE * (clear_effect_timer / static_cast<float>(CLEAR_EFFECT_DURATION))));
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                sf::RectangleShape preview_border(sf::Vector2f(5 * CELL_SIZE, 5 * CELL_SIZE));
                preview_border.setFillColor(sf::Color(0, 0, 0));
                preview_border.setOutlineThickness(-1);
                preview_border.setPosition(CELL_SIZE * (1.5f * COLUMNS - 2.5f), CELL_SIZE * (0.25f * ROWS - 2.5f));
                window.clear();

                if (1 == game_over) {
                    // sf::sleep(sf::milliseconds(100));

                    sf::Text game_over_text("Game Over", font, 12);
                    game_over_text.setFillColor(sf::Color(255, 255, 255));
                    game_over_text.setPosition(CELL_SIZE * (0.5f * COLUMNS - 3.5f), CELL_SIZE * (0.5f * ROWS - 0.5f));
                    window.draw(game_over_text);


                    sf::Text restart_text("Press Enter to restart", font, 10);
                    restart_text.setFillColor(sf::Color(255, 255, 255));
                    restart_text.setPosition(CELL_SIZE * (0.5f * COLUMNS - 4.5f), CELL_SIZE * (0.5f * ROWS + 1.5f));
                    window.draw(restart_text);

                    window.display();
                    continue;
                }

                for (unsigned char a = 0; a < COLUMNS; a++)
                {
                    for (unsigned char b = 0; b < ROWS; b++)
                    {
                        if (0 == clear_lines[b])
                        {
                            cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
                            if (0 < matrix[a][b])
                            {
                                cell.setFillColor(cell_colors[8]);
                            }
                            else
                            {
                                cell.setFillColor(cell_colors[matrix[a][b]]);
                            }
                            window.draw(cell);
                        }
                    }
                }
                cell.setFillColor(cell_colors[8]);
                if (0 == game_over)
                {
                    for (Position& mino : tetromino.get_ghost_minos(matrix))
                    {
                        cell.setPosition(static_cast<float>(CELL_SIZE * mino.x), static_cast<float>(CELL_SIZE * mino.y));

                        window.draw(cell);
                    }

                    cell.setFillColor(cell_colors[1 + tetromino.get_shape()]);
                }
                for (Position& mino : tetromino.get_minos())
                {
                    cell.setPosition(static_cast<float>(CELL_SIZE * mino.x), static_cast<float>(CELL_SIZE * mino.y));
                    window.draw(cell);
                }

                for (unsigned char a = 0; a < COLUMNS; a++)
                {
                    for (unsigned char b = 0; b < ROWS; b++)
                    {
                        if (1 == clear_lines[b])
                        {
                            cell.setFillColor(cell_colors[0]);
                            cell.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
                            cell.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                            window.draw(cell);
                            cell.setFillColor(sf::Color(255, 255, 255));
                            cell.setPosition(floor(CELL_SIZE * (0.5f + a) - 0.5f * clear_cell_size), floor(CELL_SIZE * (0.5f + b) - 0.5f * clear_cell_size));
                            cell.setSize(sf::Vector2f(clear_cell_size, clear_cell_size));
                            window.draw(cell);
                        }
                    }
                }
                cell.setFillColor(cell_colors[1 + next_shape]);
                cell.setSize(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
                window.draw(preview_border);
                for (Position& mino : get_tetromino(next_shape, static_cast<unsigned char>(1.5f * COLUMNS), static_cast<unsigned char>(0.25f * ROWS)))
                {
                    unsigned short next_tetromino_x = CELL_SIZE * mino.x;
                    unsigned short next_tetromino_y = CELL_SIZE * mino.y;
                    if (0 == next_shape)
                    {
                        next_tetromino_y += static_cast<unsigned char>(round(0.5f * CELL_SIZE));
                    }
                    else if (3 != next_shape)
                    {
                        next_tetromino_x -= static_cast<unsigned char>(round(0.5f * CELL_SIZE));
                    }
                    cell.setPosition(next_tetromino_x, next_tetromino_y);
                    window.draw(cell);
                }
                window.display();
            }
        }
    }
}
