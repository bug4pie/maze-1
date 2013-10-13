# include <stdio.h>

// 座標はmaze[x][y]と表す

# define WALL 9999
# define ROAD 0
# define CURRENT 2
# define GOAL 3

// 現在位置をmaze[x][y]としたときに
# define UP    0 // [x-1][y]
# define DOWN  1 // [x+1][y]
# define RIGHT 2 // [x][y+1]
# define LEFT  3 // [x][y-1]

void init_maze(int maze[6][6]);
void print_maze(int maze[6][6]);
int get_current_x(int maze[6][6]);
int get_current_y(int maze[6][6]);
int get_next_x(int maze[6][6], int direction);
int get_next_y(int maze[6][6], int direction);
void set_seed();
int my_rand(int n);
int is_wall(int maze[6][6], int x, int y);
int at_goal(int maze[6][6]);
int decide_direction(int maze[6][6]);
void move(int maze[6][6]);
void print_result(int log[6][6]);

void main()
{
    int maze[6][6], log[6][6];

    set_seed();
    init_maze(maze);
    init_maze(log);

    int crr_x = get_current_x(maze);
    int crr_y = get_current_y(maze);
    printf("(%d, %d)\n", crr_x, crr_y);
    int i = 1;
    log[crr_x][crr_y] = i;

    while (1) {
        move(maze);
        i++;

        int crr_x = get_current_x(maze);
        int crr_y = get_current_y(maze);
        printf("(%d, %d)\n", crr_x, crr_y);
        log[crr_x][crr_y] = i;

        if (at_goal(maze)) {
            printf("REACHED GOAL!!\n");
            print_result(log);
            break;
        }
    }

}

// rand()を呼ぶ前に一度呼んでおく
void set_seed()
{
    srand(time(NULL));
}

void init_maze(int maze[6][6])
{
    // 壁と通り道
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            if (i == 0 || i == 5) {
                maze[i][j] = WALL;           
            } else if (j == 0 || j == 5) {
                maze[i][j] = WALL;
            } else {
                maze[i][j] = ROAD;
            }
        }
    }

    // 初期位置とゴール位置
    maze[1][1] = CURRENT;
    maze[4][4] = GOAL;
}

void print_maze(int maze[6][6])
{
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            printf("%4d ", maze[i][j]);
        }
        printf("\n");
    }
}

int get_current_x(int maze[6][6])
{
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            if (maze[i][j] == CURRENT) {
                return i;
            }   
        }
    }
}

int get_current_y(int maze[6][6])
{
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            if (maze[i][j] == CURRENT) {
                return j;
            }   
        }
    }
}

int get_next_x(int maze[6][6], int direction)
{
    int crr_x, crr_y, nxt_x, nxt_y;

    crr_x = get_current_x(maze);
    crr_y = get_current_y(maze);

    switch (direction) {
        case UP:
            nxt_x = crr_x-1;
            nxt_y = crr_y;
            break;
        case DOWN:
            nxt_x = crr_x+1;
            nxt_y = crr_y;
            break;
        case RIGHT:
            nxt_x = crr_x;
            nxt_y = crr_y+1;
            break;
        default: // case LEFT:
            nxt_x = crr_x;
            nxt_y = crr_y-1;
            break;
    }

    return nxt_x;
}

int get_next_y(int maze[6][6], int direction)
{
    int crr_x, crr_y, nxt_x, nxt_y;

    crr_x = get_current_x(maze);
    crr_y = get_current_y(maze);

    switch (direction) {
        case UP:
            nxt_x = crr_x-1;
            nxt_y = crr_y;
            break;
        case DOWN:
            nxt_x = crr_x+1;
            nxt_y = crr_y;
            break;
        case RIGHT:
            nxt_x = crr_x;
            nxt_y = crr_y+1;
            break;
        default: // case LEFT:
            nxt_x = crr_x;
            nxt_y = crr_y-1;
            break;
    }

    return nxt_y;
}


// 0 ~ n-1 までの整数乱数を返す
int my_rand(int n)
{
    return rand() % n;
}

// 指定した座標が壁であるかどうかを判定
int is_wall(int maze[6][6], int x, int y)
{
    if (maze[x][y] == WALL) {
        return 1;
    } else {
        return 0;
    }
}

// 値がGOALである座標が存在しない場合には、今ゴール地点にいると判定
int at_goal(int maze[6][6])
{
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            if (maze[i][j] == GOAL) {
                return 0;
            }
        }
    }

    return 1;
}

int decide_direction(int maze[6][6])
{
    int crr_x, crr_y, nxt_x, nxt_y, direction;

    crr_x = get_current_x(maze);
    crr_y = get_current_y(maze);

    while (1) {
        direction = my_rand(4);

        nxt_x = get_next_x(maze, direction);
        nxt_y = get_next_y(maze, direction);

        if (is_wall(maze, nxt_x, nxt_y)) {
            continue;
        } else {
            return direction;
        }
    }
}

void move(int maze[6][6])
{
    int crr_x, crr_y, nxt_x, nxt_y, direction;

    direction = decide_direction(maze);
    crr_x = get_current_x(maze);
    crr_y = get_current_y(maze);
    nxt_x = get_next_x(maze, direction);
    nxt_y = get_next_y(maze, direction);

    maze[crr_x][crr_y] = ROAD;
    maze[nxt_x][nxt_y] = CURRENT;
}

void print_result(int log[6][6])
{
    printf("---------- RESULT -----------\n");
    print_maze(log);
}
