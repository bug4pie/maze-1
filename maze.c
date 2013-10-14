# include <stdio.h>

// 座標はmaze[x][y]と表す

# define WALL 999
# define ROAD 0
# define CURRENT 2
# define GOAL 3

// 現在位置をmaze[x][y]としたときに
# define UP    0 // [x-1][y]
# define DOWN  1 // [x+1][y]
# define RIGHT 2 // [x][y+1]
# define LEFT  3 // [x][y-1]

void init_maze(int maze[6][6]);
void init_log(int log[6][6]);
void print_maze(int maze[6][6]);
int get_current_x(int maze[6][6]);
int get_current_y(int maze[6][6]);
int get_next_x(int maze[6][6], int direction);
int get_next_y(int maze[6][6], int direction);
void set_seed();
int my_rand(int n);
int is_wall(int maze[6][6], int x, int y);
int at_goal(int maze[6][6]);
void move(int maze[6][6], int log[6][6], int* p_history, int* p_back_flag);
void print_result(int log[6][6]);
int get_directions_size(int maze[6][6], int log[6][6]);
void record_log(int maze[6][6], int log[6][6], int* p_history);
void set_directions(int maze[6][6], int log[6][6], int *directions);
int back(int maze[6][6], int log[6][6], int* p_back_flag);

void main()
{
    int maze[6][6], log[6][6];
    int history = 0;
    int back_flag = 0;

    set_seed();
    init_maze(maze);
    init_log(log);
    record_log(maze, log, &history);

    //print_maze(log);
    //printf("\n");

    while (1) {
        move(maze, log, &history, &back_flag);
        //print_maze(log);
        //printf("\n");

        if (at_goal(maze)) {
            printf("GOAL!!\n");
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

void init_log(int log[6][6])
{
    // 壁と通り道
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            if (i == 0 || i == 5) {
                log[i][j] = WALL;           
            } else if (j == 0 || j == 5) {
                log[i][j] = WALL;
            } else {
                log[i][j] = ROAD;
            }
        }
    }

    // はじめのログをとる
    log[1][1] = 1;
}

void print_maze(int maze[6][6])
{
    int i, j;
    for (i = 0; i < 6; i++) {
        for (j = 0; j < 6; j++) {
            printf("%3d ", maze[i][j]);
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

void move(int maze[6][6], int log[6][6], int* p_history, int* p_back_flag)
{
    int crr_x, crr_y, nxt_x, nxt_y, direction;

    crr_x = get_current_x(maze);
    crr_y = get_current_y(maze);

    // 進む方向を決める
    int directions_size = get_directions_size(maze, log);

    if (directions_size == 0) {
        // 未通行の地点が四方に存在しない時
        if (*p_back_flag == 0) {
            *p_back_flag = log[crr_x][crr_y] - 1;
        }
        direction = back(maze, log, p_back_flag);
    } else {
        *p_back_flag = 0;

        int directions[directions_size];
        set_directions(maze, log, directions);

        direction = directions[my_rand(directions_size)];
    }

    nxt_x = get_next_x(maze, direction);
    nxt_y = get_next_y(maze, direction);

    maze[crr_x][crr_y] = ROAD;
    maze[nxt_x][nxt_y] = CURRENT;

    record_log(maze, log, p_history);
}

int back(int maze[6][6], int log[6][6], int* p_back_flag)
{
    int back_direction, direction;
    int directions[4] = {UP, DOWN, RIGHT, LEFT};
    int nxt_x, nxt_y;

    int i;
    for (i = 0; i < 4; i++) {
        direction = directions[i];
        nxt_x = get_next_x(maze, direction);
        nxt_y = get_next_y(maze, direction);

        if (log[nxt_x][nxt_y] == *p_back_flag) {
            back_direction = direction;
            *p_back_flag = *p_back_flag - 1;
        }
    }

    return back_direction;
}

void record_log(int maze[6][6], int log[6][6], int* p_history)
{
    int crr_x = get_current_x(maze);
    int crr_y = get_current_y(maze);

    *p_history = *p_history + 1;
    log[crr_x][crr_y] = *p_history;
}

void print_result(int log[6][6])
{
    printf("------- RESULT --------\n");
    print_maze(log);
}

// 進むことのできる方向の個数を返す
int get_directions_size(int maze[6][6], int log[6][6])
{
    int directions_size = 0;

    int crr_x = get_current_x(maze);
    int crr_y = get_current_y(maze);

    int nxt_x, nxt_y, direction;
    int directions[4] = {UP, DOWN, RIGHT, LEFT};

    int i;
    for (i = 0; i < 4; i++) {
        direction = directions[i];
        nxt_x = get_next_x(maze, direction);
        nxt_y = get_next_y(maze, direction);

        // 未通行ならば、候補数をひとつ増やす
        if (log[nxt_x][nxt_y] == 0) {
            directions_size++;
        }
    }

    return directions_size;
}

void set_directions(int maze[6][6], int log[6][6], int *directions)
{
    int k = 0;

    int all_directions[4] = {UP, DOWN, RIGHT, LEFT};

    int direction, nxt_x, nxt_y;
    int i;
    for (i = 0; i < 4; i++) {
        direction = all_directions[i];
        nxt_x = get_next_x(maze, direction);
        nxt_y = get_next_y(maze, direction);

        // 未通行ならば、その方向を配列に格納
        if (log[nxt_x][nxt_y] == 0) {
            directions[k] = direction;
            k = k + 1;
        }
    }
}
