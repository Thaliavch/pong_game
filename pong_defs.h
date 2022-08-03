/* --------------------------------------------------------
 *    File: main.cpp
 *  Author: Thalia Valle Chavez
 *   Class: COP 2006, CRN: 13969
 * Purpose: Ping-Pong Game elements' definitions.
 * -------------------------------------------------------- */
#include <SFML/Graphics.hpp>
#ifndef FINALPONG_PONG_DEFS_H
#define FINALPONG_PONG_DEFS_H

// Global Constants
// --------------------------------------------------------
// window properties
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const sf::Color WINDOW_COLOR = sf::Color::Black;

// drawing properties
const float FRAME_RATE = (1.0/30.0) * 1000.0;// FPS in ms
const sf::Color BALL_COLOR = sf::Color::Cyan;
const float BALL_RADIUS = 10.0;

// These are just for fun
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame
const float SPEED_TIME = (3.0 * 1000.0) * 30.0;     //
const float BALL_SPEED_X =  BALL_RADIUS * 10.0 / 10000.0;    // speed horizontally
const float BALL_SPEED_Y =  BALL_RADIUS * 8.5 / 10000.0;   // span  vertically

//Block properties
const float WALL_THICKNESS = 4.0;
const sf::Color WALL_COLOR = sf::Color::Cyan;

// Paddle constants
const float PADDLE_HEIGHT = 80.0;
const float PADDLE_THICKNESS = 10.0;
const sf::Color PADDLE_COLOR = sf::Color::White;
const float PADDLE_SPEED = PADDLE_HEIGHT/1000.0/1000.0;


// Type definitions
// --------------------------------------------------------

enum Direction {
    Exit = -1,
    None = 0,
    Left = 1,
    Up = 2,
    Right = 3,
    Down = 4,
    Start = 5
};

// ball properties
struct Ball {

    float radius;
    float coordinateX;
    float coordinateY;
    float velocityX;
    float velocityY;
    sf::Color color;
};

// Block properties
struct Block{
    float left;
    float top;
    float width;
    float height;
    sf::Color color;
    sf::RectangleShape rectangle;
};

// MovingBlock structure for the Paddle
struct MovingBlock{
    Block paddleBlock;
    float velocityX;
    float velocityY;
};


// Borders properties
struct Borders{
    Block leftWall;
    Block topWall;
    Block rightWall;
    Block bottomWall;
};

#endif //PADDLE_PONG_DEFS_H
