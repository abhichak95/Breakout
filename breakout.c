
// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"
#include "ginteractors.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 15

// lives
#define LIVES 3

//Paddle dimensions
#define HEIGHT_PADDLE 13
#define WIDTH_PADDLE 80

//Brick Dimensions
#define HEIGHT_BRICK 10

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));
    double velocityX = 2;
    while(velocityX <= 2)
    {
        velocityX = drand48() * 4;
    }
    double velocityY = 3;

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    // Object for collision checking
    //GObject object;

    waitForClick();

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // TODO

        // Scoreboard
        updateScoreboard(window, label, points);

        // Movement of Ball
        move(ball, velocityX, velocityY);
        pause(10);
        // Movement of Ball Configured

        // Movement of Paddle with Mouse
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                double y = (HEIGHT / 10) * 9;
                setLocation(paddle, x, y);
            }
        }
        // Movement of Paddle Configured

        // Collision with objects
        GObject ob = detectCollision(window, ball);
        if(ob != NULL)
        {
            if(ob == paddle)
            {
                velocityY = -velocityY;
                int tmp1 = getX(paddle) + WIDTH_PADDLE/2 + 20;
                int tmp2 = getX(ball) + RADIUS;
                int tmp3 = getX(paddle) + WIDTH_PADDLE/2 - 20;
              /*  int tmp4 = tmp1 + 10;
                int tmp5 = tmp3 - 10;
                if(tmp2 > tmp4)
                {
                    if(velocityX < 0)
                    {
                        velocityX = -velocityX * 1.5;
                    }
                }
                else if(tmp2 < tmp5)
                {
                    if(velocityX > 0)
                    {
                        velocityX = -velocityX * 1.5;
                    }
                }
                else */if(tmp2 > tmp1)
                {
                    if(velocityX < 0)
                    {
                        velocityX = -velocityX;
                    }
                }
                else if(tmp2 < tmp3)
                {
                    if(velocityX > 0)
                    {
                        velocityX = -velocityX;
                    }
                }
            }
            else if(strcmp(getType(ob), "GRect") == 0)
            {
                removeGWindow(window, ob);
                velocityY = -velocityY;
                points++;
                bricks--;
            }
        }
        // Colliion with objects configured

        updateScoreboard(window, label, points);

        // Collision with walls

        if(getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityX = -velocityX;
        }
        else if(getX(ball) <= 0)
        {
            velocityX = -velocityX;
        }

        if(getY(ball) <= 0)
        {
            velocityY = -velocityY;
        }
        else if(getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives--;
            setLocation(ball, WIDTH/2 - RADIUS/2, HEIGHT/2 - RADIUS/2);
            setLocation(paddle, WIDTH / 2 - WIDTH_PADDLE/2, HEIGHT/10*9);
            if(lives == 0)
                break;
            waitForClick();
        }
        //Collision with walls configured
    }
    if(bricks > 0)
    {
        GLabel result = newGLabel("YOU LOSE!!!");
        setColor(result, "RED");
        setFont(result, "SansSerif-50");
        add(window, result);
        double x = (getWidth(window) - getWidth(result)) / 2;
        double y = (getHeight(window) - getHeight(result)) / 2 - 80;
        setLocation(result, x, y);
    }
    else
    {
        GLabel result = newGLabel("YOU WIN!!!");
        setColor(result, "RED");
        setFont(result, "SansSerif-50");
        add(window, result);
        double x = (getWidth(window) - getWidth(result)) / 2;
        double y = (getHeight(window) - getHeight(result)) / 2 - 80;
        setLocation(result, x, y);
    }
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // TODO
    int i, j;
    double x = 5;
    double y = 50;
    for(i = 0; i < ROWS; i++)
    {
        if(i > 0)
            y += 20;
        x = 5;
        for(j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(x, y, WIDTH/10-10, HEIGHT_BRICK);
            setFilled(brick, true);
            if(i == 0)
                setColor(brick, "YELLOW");
            if(i == 1)
                setColor(brick, "CYAN");
            if(i == 2)
                setColor(brick, "MAGENTA");
            if(i == 3)
                setColor(brick, "GREEN");
            if(i == 4)
                setColor(brick, "BLACK");
            add(window, brick);
            x = x + WIDTH/10;
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    GOval ball = newGOval(0, 0, RADIUS, RADIUS);
    setFilled(ball, true);
    setColor(ball, "ORANGE");
    double x = WIDTH/2 - RADIUS/2;
    double y = HEIGHT/2 - RADIUS/2;
    setLocation(ball, x, y);
    add(window, ball);
    return ball;
    //return NULL;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // TODO
    GRect paddle = newGRect(0, 0, WIDTH_PADDLE, HEIGHT_PADDLE);
    setFilled(paddle, true);
    setColor(paddle, "BLUE");
    add(window, paddle);
    double x = WIDTH / 2 - WIDTH_PADDLE/2;
    double y = (HEIGHT / 10) * 9;
    setLocation(paddle, x, y);
    return paddle;
    //return NULL;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    GLabel score = newGLabel("");
    setFont(score, "SansSerif-36");
    add(window, score);
    double x = (getWidth(window) - getWidth(score)) / 2;
    double y = (getHeight(window) - getHeight(score)) / 2;
    setLocation(score, x, y);
    return score;
    //return NULL;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
