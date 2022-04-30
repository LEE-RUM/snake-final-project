// CS 355 - Systems Programming Course Project - Spring 2022 
// Kevin Lynch, Quentin Carr, Lirim Mehmeti

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>

//Global Varibles
int next_snake_x, next_snake_y, snakesize, dir, curdir, invin = 4, speed = 200000;
int max_x, max_y, winsize, input, lastin;
time_t currt, ttl;
bool game_over = false, moving = true, hlight = true;
struct point{
    int x;
    int y;
};
struct trophy{
    int x;
    int y;
    int value;
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
void win_game();
void detect_collision();
void gen_trph();
void print_trph();

// Lirim Mehmeti, Kevin Lynch, Quentin Carr
// main function
int main(){
    initscr(); // initialzes curses
    start_color(); 
    curs_set(false); // cursor hidden
    noecho(); // removes username
    no_blocking(); // turns off blocking mode for the keyboard terminal
    getmaxyx(stdscr, max_y, max_x); //gets the max x and y values for the terminal screen
    winsize = max_x + max_y; //Sets the snake size needed to win
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
                next_snake_y--;
                curdir= -1;
                dir=0;
                moving = true;
                lastin = input;
                break;
            case 's': // input to move down
                next_snake_y++;
                curdir = +1;
                dir=2;
                moving = true;
                lastin = input;
                break;
            case 'd': // input to move right
                next_snake_x++;
                curdir = +1;
                dir=1;
                moving = true;
                lastin = input;
                break;
            case 'a': // input to move left
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
                usleep (speed);
                // gives the snake invinciblitiy for the first 4 frames
                if(invin == 0)                
                    detect_collision(); // check if snake passes the boundary of the pit
                else
                    invin--;

            }
            if(dir == 1 || dir == 3){
                move_snake(next_snake_x, next_snake_y);
                refresh_screen();
                next_snake_x += curdir;
                usleep (speed);
                // gives the snake invinciblitiy for the first 4 frames
                if(invin == 0)
                    detect_collision(); // check if snake passes the boundary of the pit
                else
                    invin--;
            }
        }
    
    refresh(); // refreshes screen
    }
    erase();
    while(game_over) // display game over screen
        lose_game();

    endwin();
}
// Lirim Mehmeti, Quentin Carr (Graphics)
// Using mvhline and mvvline to create border/pit
void draw_borders(){

    start_color();
    init_pair(2, COLOR_BLUE, COLOR_BLUE);
    init_pair(5, COLOR_WHITE, COLOR_BLUE);

    attron(COLOR_PAIR(2));
        mvhline(0, 1, '#', COLS-1);// top of pit
        mvhline(LINES-1,1,'#',COLS-1);//draws bottom of bit

        mvvline(0,0,'#',LINES);// Left line
        mvvline(1,COLS-1,'#',LINES-2);// Right line
    attroff(COLOR_PAIR(2));

    int tleft = ttl-currt;
    attron(COLOR_PAIR(5));
        mvprintw(0, (max_x/2) - 23,"Size: %d     Size To Win: %d     Trophy Life: %d", snakesize, winsize, tleft);
    attron(COLOR_PAIR(5));
            
}
/*Kevin Lynch
Used to update the x and y positions of each segment of the snake array*/
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
/*Kevin Lynch, Lirim Mehmeti (bug fixes)
Used to set the starting positions x and y postions of each segment of the snake array in a random direction*/
void init_snake(int max_y,int max_x) 
{
    struct point current;
    srand(time(NULL)); // randomizes the seed for every execution of the file
    dir = rand() % 4; // gets random starting direction
    snakesize = 3;
    int j = 0;
    if(dir == 0) //Starting direction up
    {
        for(int i = 0; i < snakesize; i++) 
        {
            current.x = max_x / 2;
            current.y = (max_y / 2);
            snake[j] = current;
            j++;
            lastin = 'w';    
        }
    }
    else if (dir == 1) //Starting direction right
    {
        for(int i = 0; i < snakesize; i++)
        {
            current.x = (max_x / 2);
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
            current.y = (max_y / 2);
            snake[j] = current;
            j++;
            lastin = 's';
        }  
    }
    else if (dir == 3) //Starting direction left
    {
        for(int i = 0; i < snakesize; i++)
        {
            current.x = (max_x / 2);
            current.y = max_y / 2;
            snake[j] = current;
            j++;
            lastin = 'a';
        } 
    }

    //Creates the starting trophy
    gen_trph(); 

    refresh();       
}
/*Kevin Lynch, Lirim Mehmeti(made some fixes to stop flickering), Quentin Carr (Graphics)
Used to update the screen and print the snake, borders, and trophies*/
void refresh_screen()
{
    erase();
    draw_borders();
    
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    
    attron(COLOR_PAIR(1));
        for (int a = 0; a<snakesize; a++){           
                mvprintw(snake[a].y, snake[a].x,"S");
            }
    attroff(COLOR_PAIR(1));
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
/*Kevin Lynch, Lirim Mehmeti(minor fix), Quentin Carr (graphics)
Used to displays game over screen*/
void lose_game() 
{
    start_color();
    init_pair(3, COLOR_BLACK, COLOR_RED);

    //Make message flash
    if(hlight){    
        standout();
        hlight = false;
    }
    else {
        standend();
        hlight = true;
    }
    attron(COLOR_PAIR(3));
    attron(A_BOLD);
        mvprintw(max_y/2, (max_x/2) - 7,"Game Over, You lost!");
        mvprintw(max_y/2 + 1, (max_x/2) - 4,"Final score: %d", snakesize);
    attroff(A_BOLD);
    attroff(COLOR_PAIR(3));
    refresh();
    sleep(1);
}
/*Quentin Carr, Kevin Lynch, Lirim Mehmeti
Used to displays game won screen*/
void win_game() 
{
    start_color();
    init_pair(4, COLOR_BLUE, COLOR_YELLOW);

    //Make message flash
    if(hlight){    
        standout();
        hlight = false;
    }
    else {
        standend();
        hlight = true;
    }
    attron(COLOR_PAIR(4));
    attron(A_BOLD);
        mvprintw(max_y/2, (max_x/2) - 9,"Congratulations, You Won!!!");
        mvprintw(max_y/2 + 1, (max_x/2) - 4,"Final score: %d", snakesize);
    attroff(A_BOLD);
    attroff(COLOR_PAIR(4));
    refresh();
    sleep(1);
}
/*Kevin Lynch, Quentin Carr
Used to checks if snake is past the pit boundaries, hits itself, or got a trophy*/
void detect_collision() 
{
    //collision with wall
    if (snake[snakesize -1].x <= 0 || snake[snakesize - 1].x >= max_x -1){
        game_over = true;
        return;
    }
    if (snake[snakesize -1].y <= 0 || snake[snakesize - 1].y >= max_y - 1){
        game_over = true;
        return;
    }
    
    //collison with self
    char* nxtpos = (char*) calloc(2, sizeof(char));

    mvinnstr(next_snake_y, next_snake_x, nxtpos, 1);
    if(nxtpos[0] == 'S')
        game_over = true;

    free(nxtpos);

    //collision with trophy
    if (snake[snakesize - 1].y == trophy1.y && snake[snakesize - 1].x == trophy1.x){
        //increase snake size and speed
        snakesize += trophy1.value;
        //set max speed to 80000
        if (speed>80000)
            speed -= (trophy1.value * 500);

        //if snakesize bigger than half the parameter, player wins
        if(winsize > snakesize)        
            gen_trph();
        else{
            erase();
            // display game win screen
            while(true)
                win_game();
        }
        return;
    }
}

/*Quentin Carr
Generates variables for trophy structure such that the tophy spawns as follows
    -> Within the terminal boundries
    -> Not on snake body    
    -> Random value 1-9
    -> Random time-to-live 1-9 seconds
    -> Within reach of the snake head before ttl expires*/
void gen_trph(){
    //initialize variables to prevent floating point exception
    int txmax=1, txmin=1, tymax=1, tymin=1, snakex, snakey;
    bool i = false;
    char* pos = (char*) calloc(2, sizeof(char));

    //make sure first trophy is within reach of the snake
    if(invin > 0){
        snakex = max_x/2;
        snakey = max_y/2;
    }
    else{
        snakex = next_snake_x;
        snakey = next_snake_y;
    }

    //set random integer for trophy
    trophy1.value = ((rand() % 9) + 1);
    //set random time-to-live value for the trophy
    double ttlv = ((rand() % 9) + 1);
    ttl = time(NULL)+ttlv;

    //get number of characters per second the snake can move
    double cps = speed/1000000.0;
    //set max distance of trophy based off cps
    double maxdis = ttlv/cps;
    
    do{
        //set max and min for x position of trophy
        txmax = snakex + maxdis;
        txmin = snakex - maxdis;

        //make sure the max and min are within the terminal window
        if(txmax > max_x-2)
            txmax = max_x-2;
        if(txmin < 1)
            txmin = 1;

        //check for floating point exception, set random x position for trophy
        if((txmax-txmin+1) == 0)
            trophy1.x = snakex + 1;
        else
            trophy1.x = (rand() % (txmax-txmin+1))+txmin;

        //get max y position based off x psoition 
        maxdis -= abs(snakex-trophy1.x);

        if(maxdis == 0)
            trophy1.y = snakey;
        else{
            //set max and min for y position of trophy
            tymax = snakey + maxdis;
            tymin = snakey - maxdis;

            //make sure the max and min are within the terminal window
            if(tymax > max_y-2)
                tymax = max_y-2;
            if(tymin < 1)
                tymin = 1;

            //set random y position for trophy and check for floating point exception
            if((tymax-tymin+1) == 0)
                trophy1.y = snakey+1;
            else
                trophy1.y = (rand() % (tymax-tymin+1))+tymin;
        }

        //check if the trophy will spawn on snake body, if true get a different position
        mvinnstr(trophy1.y, trophy1.x, pos, 1);
        if(pos[0] == 'S')
            i = true;
        else
            i = false;
      }while(i);

    free(pos);

    //convert int into char* to output trophy in window
    switch(trophy1.value){
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
    }
}

/*Quentin Carr
prints the current trophy in the window*/
void print_trph(){
    //get current time
    currt = time(NULL);
    start_color();
    init_pair(6, COLOR_WHITE, COLOR_YELLOW);

    //print the current trophy
    attron(COLOR_PAIR(6));
    attron(A_BOLD);
        mvprintw(trophy1.y, trophy1.x, trophy1.str);
    attroff(A_BOLD);
    attroff(COLOR_PAIR(6));

    //if the current time exceeds time-to-live generate a new trophy
    if(currt > ttl)
        gen_trph();
}
