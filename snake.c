// CS 355 - Systems Programming Course Project - Spring 2022 
// Kevin Lynch, Quentin Carr, Lirim Mehmeti

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>

//Global Varibles
int next_snake_x, next_snake_y,snakesize, dir, curdir;
int max_x, max_y;
int input, lastin;
time_t currt, ttl;
bool game_over = false;
bool moving = true;
struct point{
    int x;
    int y;
};
struct trophy{
    int x;
    int y;
    int number;
    char* str;
};
struct trophy trophy1; 
struct point snake[] = {};

//prototype functions
void draw_borders();
void move_snake(int nextx, int nexty);
void init_snake();
void refresh_screen();
void no_blocking();
void lose_game();
void detect_collision();
void gen_trph();
void print_trph();

// Lirim Mehmeti, Kevin Lynch, Quentin Carr
// main function
int main(){
    initscr(); // initialzes curses
    curs_set(false); // cursor hidden
    noecho(); // removes username
    no_blocking(); // turns off blocking mode for the keyboard terminal
    getmaxyx(stdscr, max_y, max_x); //gets the max x and y values for the terminal screen
    init_snake(max_y, max_x); //prints snake in a random direction in the center of the screen
    refresh_screen(); // updates the screen 
    
    // a random direction at the start of the game
    int startingDirection = rand() % 4;
    switch (startingDirection) {
        // move up
        case 0:
            next_snake_y--;
            curdir= -1;
            dir=0;
            break;
        // move down
        case 1:
            next_snake_y++;
            curdir = +1;
            dir=2;
            break;
        // move right
        case 2:
            next_snake_x++;
            curdir = +1;
            dir=1;
            break;
        // move left
        case 3:
            next_snake_x--;
            curdir= -1;
            dir=3;
            break;
    }
    
    next_snake_x = snake[0].x;
    next_snake_y = snake[0].y;
    //Main Loop
    while(!game_over){
        input = getch();
        switch(input){
            case 'w': // input to move up
                if(lastin == 's') // check if snake is reversing direction
                    game_over = true;
                next_snake_y--;
                curdir= -1;
                dir=0;
                moving = true;
                lastin = input;
                break;
            case 's': // input to move down
            if(lastin == 'w') // check if snake is reversing direction
                    game_over = true;
                next_snake_y++;
                curdir = +1;
                dir=2;
                moving = true;
                lastin = input;
                break;
            case 'd': // input to move right
            if(lastin == 'a') // check if snake is reversing direction
                    game_over = true;
                next_snake_x++;
                curdir = +1;
                dir=1;
                moving = true;
                lastin = input;
                break;
            case 'a': // input to move left
            if(lastin == 'd') // check if snake is reversing direction
                    game_over = true;
                next_snake_x--;
                curdir= -1;
                dir=3;
                moving = true;
                lastin = input;
                break;
            default:
                break;
        }
        if(moving) //if the snake is moving continue in the same direction
        {
            if(dir == 0 || dir == 2){
                move_snake(next_snake_x, next_snake_y);
                refresh_screen();
                next_snake_y += curdir;
                usleep (200000);
                detect_collision(); // check if snake passes the boundary of the pit

            }
            if(dir == 1 || dir == 3){
                move_snake(next_snake_x, next_snake_y);
                refresh_screen();
                next_snake_x += curdir;
                usleep (200000);
                detect_collision(); // check if snake passes the boundary of the pit
            }
        }
    
    refresh(); // refreshes screen
    }
    erase();
    while(game_over) // display game over screen
        lose_game();
    endwin();
}
// Lirim Mehmeti
// Using mvhline and mvvline to create border/pit
void draw_borders(){

    mvhline(0, 1, '#', COLS-2);// top of pit
    mvhline(LINES-1,1,'#',COLS-2);//draws bottom of bit

    mvvline(0,0,'#',LINES);// Left line
    mvvline(1,COLS-2,'#',LINES-2);// Right line
            
}
/*Kevin Lynch
Used to update the x and y postions of each segement of the snake array*/
void move_snake(int nextx, int nexty)
{
    struct point temp = snake[snakesize - 1];
    struct point temp2; // Temp struct used to hold the last position of the snake
    snake[snakesize -1].x = nextx;
    snake[snakesize -1].y = nexty; 
    for(int i = snakesize - 2; i > 0; i--)//Moves the snake body in the direction of the next segement of the snake
    {
        temp2 = snake[i];
        snake[i] = temp;
        temp = temp2;
    }
    snake[0] = temp; //sets the tail of the snake
}
/*Kevin Lynch
Used to set the starting positions x and y postions of each segemtn of the snake array in a random direction*/
void init_snake(int max_y,int max_x) 
{
    struct point current;
    srand(time(NULL)); // randomizes the seed for every execution of the file
    snakesize = 5;
    int j = 0;
    if(dir == 0) //Starting direction up
    {
        for(int i = 0; i < snakesize; i++) 
        {
            current.x = max_x / 2;
            current.y = (max_y / 2) - i;
            snake[j] = current;
            j++;
            lastin = 'w';    
        }
    }
    else if (dir == 1) //Starting direction right
    {
        for(int i = 0; i < snakesize; i++)
        {
            current.x = (max_x / 2) - i;
            current.y = max_y / 2;
            snake[j] = current;
            j++;
            lastin = 'd';
        } 
    }
    else if(dir == 2) //Starting direction down
    {
        for(int i = 0; i < snakesize; i++)
        {
            current.x = max_x / 2;
            current.y = (max_y / 2) + i;
            snake[j] = current;
            j++;
            lastin = 's';
        }  
    }
    else if (dir == 3) //Starting direction left
    {
        for(int i = 0; i < snakesize; i++)
        {
            current.x = (max_x / 2) + i;
            current.y = max_y / 2;
            snake[j] = current;
            j++;
            lastin = 'a';
        } 
    }
    
    gen_trph(); //Creates the starting trophy

    refresh();       
}
/*Kevin Lynch, Lirim Mehmeti(made some fixes to stop flickering)
Used to update the screen and print the snake*/
void refresh_screen()
{
    erase();
    draw_borders();
    for (int a = 0; a<snakesize; a++){            
            mvprintw(snake[a].y, snake[a].x,"S");
        }
    if (!moving){
        mvprintw(1,1,"%s");
    }
    print_trph();
    
    refresh();
    
}
/*Kevin Lynch
Used to turn off Blocking input so the program doesn't wait for an input (ch. 6 of Molay book, pg 183)*/
void no_blocking() 
{
    int termflag;
    termflag = fcntl(0, F_GETFL);
    termflag |= O_NDELAY;
    fcntl(0,F_SETFL, termflag);

}
/*Kevin Lynch, Lirim Mehmeti(minor fix)
Used to displays game over screen*/
void lose_game() 
{
    mvprintw(max_y/2, (max_x/2) - 7,"Game Over, You lost!");

    refresh();
   
}
/*Kevin Lynch, Quentin Carr
Used to checks if snake is past the pit boundaries or got a trophy*/
void detect_collision() 
{
    //collision with wall
    if (snake[snakesize -1].x <= 0 || snake[snakesize -1].x >= max_x)
        game_over = true;
    if (snake[snakesize -1].y <= 0 || snake[snakesize -1].y >= max_y)
        game_over = true;
    //collision with trophy
    if (snake[snakesize -1].y == trophy1.y && snake[snakesize -1].x == trophy1.x){
        snakesize += trophy1.number;
        gen_trph();
    }

}

/*Quentin Carr
Generates variables for trophy structure*/
void gen_trph(){
    //Set Random Position for the trophy
    trophy1.x = (rand() % (max_x-2))+1;
    trophy1.y = (rand() % (max_y-2))+1;
    //set random integer for trophy
    trophy1.number = (rand() % 9) + 1;
    //set random time-to-live for the trophy
    ttl = time(NULL) + ((rand() % 9) + 1);

    //convert int into char*
    switch(trophy1.number){
        case 1:
            trophy1.str = "1";
            break;
        case 2:
            trophy1.str = "2";
            break;
        case 3:
            trophy1.str = "3";
            break;
        case 4:
            trophy1.str = "4";
            break;
        case 5:
            trophy1.str = "5";
            break;
        case 6:
            trophy1.str = "6";
            break;
        case 7:
            trophy1.str = "7";
            break;
        case 8:
            trophy1.str = "8";
            break;
        case 9:
            trophy1.str = "9";
            break;
        default:
            trophy1.str = "0";
            break;
    }
}

/*Quentin Carr
prints the current trophy in the window*/
void print_trph(){
    //get current time
    currt = time(NULL);

    //print the current trophy
    mvprintw(trophy1.y, trophy1.x, trophy1.str);

    //if the current time exceeds time-to-live generate a new trophy
    if(currt > ttl)
        gen_trph();

}
