#include<ncurses.h> // for creating the UI
#include<stdlib.h> // for standard functions like rand ()
#include<unistd.h> // for usleep()
#include<time.h>

#define DELAY 100000 // Delay between each frame in micro seconds
#define MAX_SNAKE_LENGTH 100 // MAXIMUM LENGTH FOR SNAKE

typedef struct{
	int x, y;// coordinates for the snake segments and food
}Point;

void initialize_snake(Point *snake, int length){
	for (int=0; i<length; i++){
		snake[i].x=10-i; // set the initial x position
		snake[i].y=10; // set the initial y position
		
	}
}

void draw_snake(Point *snake, int length){
	for (int i=0; i<length; i++){
		mvaddch(snake[i].y,snake[i].x,'#'); // Draw each segments as #
	}
}
void move_snake(Point *snake,int length, int direction){
	for(int i=length-1; i>0; i--){
		snake[i]=snake[i-1]; // move each segment to the position of previous
	}
	switch (direction){
		case KEY_UP:
		snake[0].y--; // move snake head up
		break;
		case KEY_DOWN:
		snake[0].y++;// move snake head down
		break;
		case KEY_LEFT:
		snake[0].x--; // move snake head left
		break;
		case KEY_RIGHT:
		snake[0].x++; // move snake head up
		break;		
	}
}
int check_collision (Point *snake, int length,int max_y, int max_x){
	// check wall collision
	if (snake[0].x>=max_x || snake[0].x<0 || snake[0].y>=max_y || snake[0].y<0){
		retun 1;
	}
	// check self collision
	for (int i=1; i<length; i++){
		if(snake[0].x==snake[i].x && snake[0].y==snake[i].y){
			return 1;
		}
	}
	return 0;
}

void place_food(Point *food, int max_y, int max_x){
	food->x=rand()%max_x; // Random coordinate within screen
	food->y=rand()%max_y; // Random coordinate within screen
}
int main (){
	Point snake[MAX_SNAKE_LENGTH]; // arry to hold snake segment
	int length =5; // initial length of snake
	int direction =KEY_RIGHT; // initial direction
	int max_y=0, max_x=0; // screen dimesion
	Point food; // food position
	int ch;

	initscr(); // initialize ncurses
	noecho(); // Disable character echoing
	curs_set(FALSE); // HIDE CURSOR
	keypad(stdscr,TRUE); // Enable special keys
	timeout(0); // Non-blocking input
	srand(time(NULL)); // SEED RAND

	getmaxyx(stdscr, max_y,max_x); // Get screen dimensions
	initialize_snake(snake, length);
	place_food(&food, max_y, max_x);// place the first food

	while (1){
		clear(); // clear screen
		mvaddch(food.y,food.x, 'o'); // draw food
		draw_snake(snake, length); //draw snake

		move_snake(snake,length, direction); // move the snake

		if (check_collision(snake,length,max_y,max_x)){
			// collision check
			mvprintw(max_y/2,(max_x/2)-5, "Game Over");
			// display game over message
			refresh();
			usleep(2000000);// wait for 2 seconds
			break; // exit the loop
		}
		if (snake[0].x==food.x && snake[0].y==food.y){
			length++;
			place_food(&food, max_y, max_x)// place the food
		}

		refresh ();
		usleep(DELAY);

		ch = getch (); // Get user input
		switch (ch){
			case KEY_UP:
			case KEY_DOWN:
			case KEY_LEFT:
			case KEY_RIGHT:
			direction =ch; // update direction based on user input
			break;
		}
	}
	endwin(); // end ncurses
	return 0;
}
