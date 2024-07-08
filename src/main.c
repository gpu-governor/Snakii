#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>

#define DELAY 100000
#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x, y;
} Point;

void initialize_snake(Point *snake, int length) {
    for (int i = 0; i < length; i++) {
        snake[i].x = 10 - i;
        snake[i].y = 10;
    }
}

void draw_snake(Point *snake, int length) {
    for (int i = 0; i < length; i++) {
        mvaddch(snake[i].y, snake[i].x, '#');
    }
}

void move_snake(Point *snake, int length, int direction) {
    for (int i = length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    switch (direction) {
        case KEY_UP:
            snake[0].y--;
            break;
        case KEY_DOWN:
            snake[0].y++;
            break;
        case KEY_LEFT:
            snake[0].x--;
            break;
        case KEY_RIGHT:
            snake[0].x++;
            break;
    }
}

int check_collision(Point *snake, int length, int max_y, int max_x) {
    // Check wall collisions
    if (snake[0].x >= max_x || snake[0].x < 0 || snake[0].y >= max_y || snake[0].y < 0) {
        return 1;
    }
    // Check self-collisions
    for (int i = 1; i < length; i++) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return 1;
        }
    }
    return 0;
}

void place_food(Point *food, int max_y, int max_x) {
    food->x = rand() % max_x;
    food->y = rand() % max_y;
}

int main() {
    Point snake[MAX_SNAKE_LENGTH];
    int length = 5;
    int direction = KEY_RIGHT;
    int max_y = 0, max_x = 0;
    Point food;
    int ch;

    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    timeout(0);
    srand(time(NULL));

    getmaxyx(stdscr, max_y, max_x);
    initialize_snake(snake, length);
    place_food(&food, max_y, max_x);

    while (1) {
        clear();

        // Draw food
        mvaddch(food.y, food.x, 'O');

        // Draw snake
        draw_snake(snake, length);

        // Move snake
        move_snake(snake, length, direction);

        // Check collisions
        if (check_collision(snake, length, max_y, max_x)) {
            mvprintw(max_y / 2, (max_x / 2) - 5, "Game Over");
            refresh();
            usleep(2000000);
            break;
        }

        // Check if food is eaten
        if (snake[0].x == food.x && snake[0].y == food.y) {
            length++;
            place_food(&food, max_y, max_x);
        }

        refresh();
        usleep(DELAY);

        // Change direction based on user input
        ch = getch();
        switch (ch) {
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                direction = ch;
                break;
        }
    }

    endwin();
    return 0;
}
