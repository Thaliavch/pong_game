/* --------------------------------------------------------
 *    File: main.cpp
 *  Author: Thalia Valle Chavez
 *   Class: COP 2006, CRN: 13969
 * Purpose: Add collision detection between paddle and walls. Final stage of a basic Ping-Pong game program.
 * -------------------------------------------------------- */
#include <SFML/Graphics.hpp>
#include "pong_defs.h"


// Function declarations (prototypes)
// --------------------------------------------------------

void setup(Ball &ball, Borders &walls, MovingBlock &paddle, MovingBlock &paddleAI );
Direction processInput() ;
bool update(Direction &input, Ball &ball, float delta, Borders walls, MovingBlock &paddle, MovingBlock &paddleAI, bool &start);
void render(sf::RenderWindow &window, Ball &ball, float delta, Borders walls, MovingBlock paddle, MovingBlock paddleAI);
bool collisionDetection(Ball &ball, Block Wall );
bool blockCollisionDetection(Block wall, Block paddle);
bool doCollisionDetection(Ball &ball, Borders &walls, MovingBlock &paddle, MovingBlock &paddleAI);


/**
 * The main application
 * @return OS status message (0=Success)
 */
int main() {
    // create a 2d graphics window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Ping-Pong!");
    window.clear(WINDOW_COLOR);
    // declare a ball variable and populate it in the center of the window
    Ball theBall;

    // Borders variable to set the properties of the wall
    // Left Block
    Borders theWalls;

    // Paddle variable for player's paddle
    MovingBlock playerPaddle;

    // Paddle variable fot AI paddle
    MovingBlock aiPaddle;

    // setup function that where all the properties for the ball, walls and paddles are initialized.
    setup(theBall, theWalls, playerPaddle, aiPaddle);

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;
    bool started = false;
    bool gameOver = false;

    // Game loop
    // -----------------------------------------------------------------------------
    while (!gameOver)
    {
        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;
        // process events
        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }

        // Process user input
        // ------------------------------------------------
        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;

        // Process Updates
        // ------------------------------------------------
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time

            update(userInput, theBall, delta, theWalls, playerPaddle, aiPaddle, started);
            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update
            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }
        // Render the window
        // ------------------------------------------------
        render(window, theBall, delta, theWalls, playerPaddle, aiPaddle);

        // returns true when there is a collision between the ball and the right or left wall, and false otherwise.
        gameOver = update(userInput, theBall, delta, theWalls, playerPaddle, aiPaddle, started );
    } // end main game loop

    // game ending, close the graphics window
    window.close();

    return 0;   // return success to the OS

} // end main


/**
 * Initialize the values of the ball, walls, and paddles.
 * @param ball - structure type with the properties of the ball.
 * @param walls - structure type with the properties of the walls for the walls.
 * @param paddle - nested structure with the properties of the paddle.
 */
void setup(Ball &ball, Borders &walls, MovingBlock &paddle, MovingBlock &paddleAI){

    // Left Block
    walls.leftWall.left = 0.0;
    walls.leftWall.top = 0.0;
    walls.leftWall.width = WALL_THICKNESS;
    walls.leftWall.height = WINDOW_HEIGHT;
    walls.leftWall.rectangle.setSize(sf::Vector2(walls.leftWall.width, walls.leftWall.height));
    walls.leftWall.rectangle.setPosition(walls.leftWall.left, walls.leftWall.top);
    walls.leftWall.rectangle.setFillColor(WALL_COLOR);

    // Top Block
    walls.topWall.left = 0.0;
    walls.topWall.top = 0.0;
    walls.topWall.width = WINDOW_WIDTH;
    walls.topWall.height = WALL_THICKNESS;
    walls.topWall.rectangle.setSize(sf::Vector2(walls.topWall.width, walls.topWall.height));
    walls.topWall.rectangle.setPosition(walls.topWall.left, walls.topWall.top);
    walls.topWall.rectangle.setFillColor(WALL_COLOR);

    // Right Block
    walls.rightWall.left = WINDOW_WIDTH - WALL_THICKNESS;
    walls.rightWall.top = 0.0;
    walls.rightWall.width = WALL_THICKNESS;
    walls.rightWall.height = WINDOW_HEIGHT;
    walls.rightWall.rectangle.setSize(sf::Vector2(walls.rightWall.width, walls.rightWall.height));
    walls.rightWall.rectangle.setPosition(walls.rightWall.left, walls.rightWall.top);
    walls.rightWall.rectangle.setFillColor(WALL_COLOR);

    // Bottom Block
    walls.bottomWall.left = 0.0;
    walls.bottomWall.top = WINDOW_HEIGHT - WALL_THICKNESS;
    walls.bottomWall.width = WINDOW_WIDTH;
    walls.bottomWall.height = WALL_THICKNESS;
    walls.bottomWall.rectangle.setSize(sf::Vector2(walls.bottomWall.width, walls.bottomWall.height));
    walls.bottomWall.rectangle.setPosition(walls.bottomWall.left, walls.bottomWall.top);
    walls.bottomWall.rectangle.setFillColor(WALL_COLOR);

    // Player Paddle properties
    paddle.paddleBlock.left = PADDLE_THICKNESS;
    paddle.paddleBlock.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0;
    paddle.paddleBlock.width = PADDLE_THICKNESS;
    paddle.paddleBlock.height = PADDLE_HEIGHT;
    paddle.paddleBlock.color = PADDLE_COLOR;
    paddle.paddleBlock.rectangle.setSize(sf::Vector2f(paddle.paddleBlock.width, paddle.paddleBlock.height));
    paddle.paddleBlock.rectangle.setFillColor(PADDLE_COLOR);
    paddle.velocityX = 0.0;
    paddle.velocityY = 0.0;

    // AI Paddle properties
    paddleAI.paddleBlock.left = WINDOW_WIDTH - 2 * PADDLE_THICKNESS - WALL_THICKNESS;//(walls.rightWall.left - 2) * PADDLE_THICKNESS;
    paddleAI.paddleBlock.top = (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0;
    paddleAI.paddleBlock.width = PADDLE_THICKNESS;
    paddleAI.paddleBlock.height = PADDLE_HEIGHT;
    paddleAI.paddleBlock.color = PADDLE_COLOR;
    paddleAI.paddleBlock.rectangle.setSize(sf::Vector2f(paddle.paddleBlock.width, paddle.paddleBlock.height));
    paddleAI.paddleBlock.rectangle.setFillColor(PADDLE_COLOR);
    paddleAI.velocityX = 0.0;
    paddleAI.velocityY = 0.0;

    // Ball properties
    ball.radius = BALL_RADIUS;
    ball.coordinateX = paddle.paddleBlock.left + paddle.paddleBlock.width + ball.radius +1;
    ball.coordinateY = paddle.paddleBlock.top + (paddle.paddleBlock.height / 2.0);
    ball.velocityX = 0.0;
    ball.velocityY = 0.0;
    ball.color = BALL_COLOR;

} // end setup.


/**
 * convert user keyboard input into recognized integer values
 * for left=Left/up=Up/right=Right/down=Down
 * @return int - user input (default no-input=0, quit=-1)
 */
Direction processInput() {
    Direction input = None;  // no input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        input = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        input = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        input = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        input = Down;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        input = Exit;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
        input = Start;
    }
    return input;

} // end getUserInput


/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param delta - current frame time
 * @param walls - update collision between the ball and the walls.
 * @param paddle - update the position of the paddle and collision between the paddle and the ball.
 * @param start - return type bool for the set the start of the game.
 */
bool update(Direction &input, Ball &ball, float delta, Borders walls, MovingBlock &paddle, MovingBlock &paddleAI, bool &start) {
    // return variable
    bool gameOver = false;
    // adjust velocity directions for user input
    if (input) {
        switch (input) {
            /*case Left:
                ball.velocityX -= BALL_SPEED_X;
                break;*/
            case Up:
                paddle.velocityY -= PADDLE_SPEED;
                break;
                /*case Right:
                    ball.velocityX += BALL_SPEED_X;
                    break;*/
            case Down:
                paddle.velocityY += PADDLE_SPEED;
                break;
            case Start:
                if (!start){
                    ball.velocityX = BALL_SPEED_X;
                    ball.velocityY = BALL_SPEED_Y;
                    if ((int(delta*10)&1)%2) {
                        ball.velocityY *= -1;
                        start = true;
                    }
                }


        }
        // clear input
        input = None;
    }

    // adjust player's paddle location
    paddle.paddleBlock.top += paddle.velocityY * delta;

    // adjust AI paddle location
    paddleAI.paddleBlock.top += paddleAI.velocityY * delta;

    // adjust the location of the ball for speed * time
    if (start) {
        ball.coordinateX += ball.velocityX * delta;
        ball.coordinateY += ball.velocityY * delta;

        // Adjusting AI paddle velocity and location in function of the position of the ball on the screen
        if (ball.coordinateY < (paddleAI.paddleBlock.height + paddleAI.paddleBlock.top) / 2.0) {
            paddleAI.velocityY -= PADDLE_SPEED;
        } else if ( ball.coordinateY > (paddleAI.paddleBlock.height + paddleAI.paddleBlock.top)/2.0) {
            paddleAI.velocityY += PADDLE_SPEED;
        }
    }
    else{
        ball.coordinateX = paddle.paddleBlock.left + paddle.paddleBlock.width + ball.radius +1;
        ball.coordinateY = paddle.paddleBlock.top + (paddle.paddleBlock.height / 2.0);
    }

    // set gameOver to true if there is collision and false otherwise.
    gameOver = doCollisionDetection(ball, walls, paddle, paddleAI);

    return gameOver;

}// end update


/**
 * draw the ball on the graphics window
 * @param window - handle to open graphics window.
 * @param ball   - structure variable to draw the ball on the screen.
 * @param delta  - amount of frame time plus lag (in ms).
 * @param borders - nested structure variable with properties to draw the walls on the screen.
 * @param paddle - draw the paddle on the screen.
 */
void render(sf::RenderWindow &window, Ball &ball, float delta, Borders walls, MovingBlock paddle, MovingBlock paddleAI) {

    // Render drawing objects
    // ------------------------------------------------
    // clear the window with the background color
    window.clear(WINDOW_COLOR);

    // draw the ball
    // ------------------------------------------------
    sf::CircleShape circle;
    circle.setFillColor(BALL_COLOR);
    circle.setRadius(ball.radius);

    // set screen coordinates relative to the center of the circle
    circle.setOrigin(ball.radius,ball.radius);

    // calculate current drawing location of ball relative to speed and frame-time
    float xPosition = ball.coordinateX + ball.velocityX * delta;
    float yPosition = ball.coordinateY + ball.velocityY * delta;
    circle.setPosition(xPosition, yPosition);
    window.draw(circle);

    // Drawing the walls
    window.draw(walls.leftWall.rectangle);
    window.draw(walls.topWall.rectangle);
    window.draw(walls.rightWall.rectangle);
    window.draw(walls.bottomWall.rectangle);

    // Player Paddle drawing on the window
    float paddleX = paddle.paddleBlock.left + paddle.velocityX * delta;
    float paddleY = paddle.paddleBlock.top + paddle.velocityY * delta;
    paddle.paddleBlock.rectangle.setPosition(paddleX, paddleY);
    window.draw(paddle.paddleBlock.rectangle);

    // AI Paddle drawing on the window
    float paddleAIx = paddleAI.paddleBlock.left + paddleAI.velocityX * delta;
    float paddleAIy = paddleAI.paddleBlock.top + paddleAI.velocityY * delta;
    paddleAI.paddleBlock.rectangle.setPosition(paddleAIx, paddleAIy);
    window.draw(paddleAI.paddleBlock.rectangle);

    // show the new window
    // ------------------------------------------------
    window.display();

} // end render


/**
 * Detect collision of the ball with the walls.
 * @param Ball   - structure variable with properties for the ball.
 * @param Wall  - nested structure variable with properties for the walls.
 * @return bool - True if there is collision, False if not.
 */
bool collisionDetection(Ball &ball, Block wall) {
    // function return variable
    bool collision = false;

    // Local variables for the ball's coordinates
    float ballLeft = ball.coordinateX - ball.radius;
    float ballTop = ball.coordinateY - ball.radius;
    float ballRight = ball.coordinateX + ball.radius;
    float ballBottom = ball.coordinateY + ball.radius;

    // Local variables for the walls' coordinates.
    float wallRight = wall.left + wall.width;
    float wallBottom = wall.top + wall.height;

    // If ball's coordinates intercept with the walls' coordinates, collision is true.
    if (ballLeft < wallRight) {
        if (ballRight > wall.left) {
            if (ballTop < wallBottom) {
                if (ballBottom > wall.top) {
                    collision = true;
                }
            }
        }
    }

    return collision;

} // end collisionDetection


/**
 * Detect collision of the paddles with the wall
 * @param wall - structure with properties for the walls
 * @param paddle - structure with properties for the paddles
 * @return bool - true if there is collision, false otherwise.
 */
bool blockCollisionDetection(Block wall, Block paddle){
    bool collision = false;

    // Local variables for the paddle coordinates
    float paddleTop = paddle.top;
    float paddleBottom = paddle.top + paddle.height;

    // Local variables for the wall' coordinates.
    float wallBottom = wall.top + wall.height;

    // If ball's coordinates intercept with the wall' coordinates, collision is true.
    if (paddleTop < wallBottom) {
        if (paddleBottom > wall.top) {
            collision = true;
        }
    }

    return collision;

} // end blockCollisionDetection


/**
 * detect collision between the ball and the walls, the ball and the paddles, and the paddles with the top and bottom
 * walls. Return a bool value of true if there is collision between the ball and the left or right wall. Update the
 * velocities and positions of the ball and the paddles after collision.
 * @param ball - structure with the properties for the ball
 * @param wall - structure with the properties for the walls
 * @param paddle - structure with the properties for the paddles
 * @return bool - true if there is collision between the ball and the left or right wall.
 */
bool doCollisionDetection(Ball &ball, Borders &walls, MovingBlock &paddle, MovingBlock &paddleAI) {
    // return variable
    bool gameOver = false;

    // if collision between the paddles and the ball is true update position of the ball and its velocity.
    // ------------------------------------------------------------------------------
    if (collisionDetection(ball, paddle.paddleBlock)) {
        ball.velocityX *= -1;
        ball.coordinateX = paddle.paddleBlock.left + paddle.paddleBlock.width + ball.radius + 1;
    } else if (collisionDetection(ball, paddleAI.paddleBlock)) {
        ball.velocityX *= -1;
        ball.coordinateX = paddleAI.paddleBlock.left - paddleAI.paddleBlock.width - ball.radius - 1;
        // if collision between the ball and the walls is true, gameOver is true.
        // -----------------------------------------------
    } else if (collisionDetection(ball, walls.leftWall)) {
            ball.velocityX *= -1;
            ball.coordinateX = walls.leftWall.left + walls.leftWall.width + ball.radius + 1;
            gameOver = true;
    } else if (collisionDetection(ball, walls.rightWall)) {
            ball.velocityX *= -1;
            ball.coordinateX = walls.rightWall.left - ball.radius - 1;
            gameOver = true;
    }

    // if collision between the ball and the walls is true, update its positions and velocity in the Y direction.
    // ---------------------------------------------------------------------------------
    if (collisionDetection(ball, walls.topWall)) {
        ball.velocityY *= -1;
        ball.coordinateY = walls.topWall.top + walls.topWall.height + ball.radius + 1;
    } else if (collisionDetection(ball, walls.bottomWall)) {
        ball.velocityY *= -1;
        ball.coordinateY = walls.bottomWall.top - ball.radius - 1;
    }
    // if collision between the paddle and the walls ins true update paddle's position and velocity
    // --------------------------------------------------
    if (blockCollisionDetection(walls.topWall, paddle.paddleBlock)) {
        paddle.velocityY = 0.0;
        paddle.paddleBlock.top = walls.topWall.top + walls.topWall.height + 1;
    } else if (blockCollisionDetection(walls.bottomWall, paddle.paddleBlock)) {
        paddle.velocityY = 0.0;
        paddle.paddleBlock.top = walls.bottomWall.top - paddle.paddleBlock.height - 1;
    }
    if (blockCollisionDetection(walls.topWall, paddleAI.paddleBlock)) {
        paddleAI.velocityY = 0.0;
        paddleAI.paddleBlock.top = walls.topWall.top + walls.topWall.height + 1;;
    } else if (blockCollisionDetection(walls.bottomWall, paddleAI.paddleBlock)) {
        paddleAI.velocityY = 0.0;
        paddleAI.paddleBlock.top = walls.bottomWall.top - paddleAI.paddleBlock.height - 1;
    }

    return gameOver;

} // end doCollisionDetection

