// CS 355 - Systems Programming Course Project - Spring 2022 
// Kevin Lynch, Quentin Carr, Lirim Mehmeti

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//Global Varibles
int next_snake_x, next_snake_y,snakesize, dir;
int max_x, max_y;
bool game_over = false;
struct point{
    int x;
    int y;
};
struct point snake[] = {};

//prototype functions
void draw_borders();
void snakemove();
void init_snake();
void refresh_screen();

// Kevin Lynch, Quentin Carr, Lirim Mehmeti
// main function
int main(){
    
    initscr(); // initialzes curses
    curs_set(false); // cursor hidden
    noecho(); // removes username
    draw_borders();
    getmaxyx(stdscr, max_y, max_x); //gets the max x and y values for the terminal screen
    int input;
    srand(time(NULL)); // randomizes the seed for every execution of the file
    init_snake(max_y, max_x); //prints snake in a random direction in the center of the screen
    for (int a = 0; a<snakesize; a++){
        mvprintw(snake[a].y, snake[a].x,"o");
        refresh();
    }
    //Main Loop
    while(1){
        input = getch();
        next_snake_x = snake[0].x;
        next_snake_y = snake[0].y;
        switch(input){
            case 'w':
                next_snake_y--;
                //mvprintw(1,1,"You pressed w");
                break;
            case 's':
                next_snake_y++;
                //mvprintw(1,1,"You pressed s");
                break;
            case 'd':
                next_snake_x++;
                //mvprintw(1,1,"You pressed d");
                break;
            case 'a':
                next_snake_x--;
                //mvprintw(1,1,"You pressed a");
                break;
            default:
                break;
        }
        snake[snakesize -1].x = next_snake_x;
        snake[snakesize -1].y = next_snake_y;
        snakemove();
        refresh_screen();
    

    }
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
// Kevin Lynch
void snakemove()
{
    struct point temp = snake[snakesize - 1]; // Temp struct used to hold the last position of the snake
    //Moves the snake body in the direction of the next segement of the snake
    for(int i = snakesize - 1; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }
    snake[0] = temp; //sets the tail of the snake
}
// Kevin Lynch
void init_snake(int max_y,int max_x) //set the start size of the snake and spawns the snake facing a random direction
{
    struct point current;
    snakesize = 5;
    dir = rand() % 4;
    int j = 0;
    if(dir == 0) //Starting direction up
    {
        for(int i = 0; i < snakesize; i++)
        {
            current.x = max_x / 2;
            current.y = (max_y / 2) - i;
            snake[j] = current;
            j++;
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
        }  
    }
refresh();    
    
}
void refresh_screen() //used to update the screen after every input so the snake maintains it size
{
    clear();
    draw_borders();
    for (int a = 0; a<snakesize; a++){            
            mvprintw(snake[a].y, snake[a].x,"o");
        }
    refresh();
}