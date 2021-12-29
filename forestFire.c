#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 30
#define NUMBER_OF_GENERATION 1000
#define G 250
#define L 10*G
#define WAIT_TIME 0.1

typedef enum state {empty, tree, fire} state;
typedef enum bool {false, true} bool;
enum color {black=30, red, green, yellow, blue, magenta, cyan, white};
typedef enum color color;

void test(void);
void state_equal(state a[HEIGHT][WIDTH], state b[HEIGHT][WIDTH]);
void initial_state(state c[HEIGHT][WIDTH]);
void set_safeguard(state a[HEIGHT][WIDTH], state b[HEIGHT + 2][WIDTH + 2]);
void quit_safeguard(state a[HEIGHT + 2][WIDTH + 2], state b[HEIGHT][WIDTH]);
bool fire_surround(state* a, int width);
void next_state(state c[HEIGHT][WIDTH], int g, int l);
void clear_screen(void);
void ch_color(const color c);
void busy_wait(const double secs);
void print_state(state c[HEIGHT][WIDTH]);


int main(void)
{
    state a[HEIGHT][WIDTH];
    int i;
    int j;
    
    test();

    initial_state(a);
    clear_screen();
    for(i = 0; i < NUMBER_OF_GENERATION; i++){
        for(j = 0; j < WIDTH; j++){
            printf("=");
        }
        printf("\n%d\n", i+1);
        next_state(a, G, L);
        print_state(a);
        busy_wait(WAIT_TIME);
    }
    return 0;
}

void test(void)
{   
    int x;
    int y;
    state a[HEIGHT][WIDTH];
    state b[HEIGHT][WIDTH];
    state c[HEIGHT][WIDTH];
    state a_safe[HEIGHT + 2][WIDTH + 2];
    state d[6][6] = {{empty,  empty, empty, empty, empty, empty}, 
                     {empty,  empty, empty,  tree, empty, empty}, 
                     {empty,  tree , empty, empty,  fire, empty}, 
                     {empty,  empty,  fire, empty,  tree, empty},
                     {empty,  empty,  tree, empty,  tree, empty},
                     {empty,  empty, empty, empty, empty, empty}};

    assert(fire_surround(&d[1][1], 6) == false);
    assert(fire_surround(&d[2][2], 6) == true );
    assert(fire_surround(&d[3][4], 6) == true );
    assert(fire_surround(&d[4][4], 6) == false);

    initial_state(a);
    assert(a[0][0] == empty);
    assert(a[0][1] == empty);
    assert(a[4][4] == empty);

    /* make a[][] switch between tree and fire */
    for(y = 0; y < HEIGHT; y++){
        for(x = 0; x < WIDTH; x++){
            if((y + x)%2 == 0){
                a[y][x] = tree;
            }
            else{
                a[y][x] = fire;
            }
        }
    }

    state_equal(b, a);
    assert(b[0][0] == tree);
    assert(b[0][1] == fire);
    assert(b[4][4] == tree);

    set_safeguard(a, a_safe);
    assert(a_safe[0][0]           == empty);
    assert(a_safe[HEIGHT + 1][20] == empty);
    assert(a_safe[13][WIDTH + 1]  == empty);
    assert(a_safe[HEIGHT + 1][WIDTH - 1]  == empty);
    assert(a_safe[1][1]           == tree);
    assert(a_safe[1][2]           == fire);

    quit_safeguard(a_safe, c);
    assert(c[0][0] == tree);
    assert(c[0][1] == fire);
    assert(c[4][4] == tree);

    /* after next_state() the a[][] should turn into switching between fire and empty */
    next_state(a, G, L);
    assert(a[0][0] == fire);
    assert(a[26][37] == empty);
    assert(a[HEIGHT - 1][WIDTH - 1] == fire);
    assert(a[15][40] == empty);
}

void state_equal(state a[HEIGHT][WIDTH], state b[HEIGHT][WIDTH])
{
    int x;
    int y;
    for(y = 0; y < HEIGHT; y++){
        for(x = 0; x < WIDTH; x++){
            a[y][x] = b[y][x];
        }
    }
}

void initial_state(state a[HEIGHT][WIDTH])
{
    int x;
    int y;
    for(y = 0; y < HEIGHT; y++){
        for(x = 0; x < WIDTH; x++){
            a[y][x] = empty;
        }
    }
}

/* make a a_safe[][] with a ring of empty outside a[][] 
to prevent testing ouside the array  */
void set_safeguard(state a[HEIGHT][WIDTH], state a_safe[HEIGHT + 2][WIDTH + 2])
{
    int x;
    int y;
    for(y = 0; y < HEIGHT + 2; y++){
        for(x = 0; x < WIDTH + 2; x++){
            a_safe[y][x] = empty;
        }
    }

    for(y = 1; y < HEIGHT + 1; y++){
        for(x = 1; x < WIDTH + 1; x++){
            a_safe[y][x] = a[y-1][x-1];
        }
    }
}

/* take off the outer empty ring of the a_safe[][] into a[][] */
void quit_safeguard(state a_safe[HEIGHT + 2][WIDTH + 2], state a[HEIGHT][WIDTH])
{
    int x;
    int y;
    for(y = 1; y < HEIGHT + 1; y++){
        for(x = 1; x < WIDTH + 1; x++){
            a[y-1][x-1] = a_safe[y][x];
        }
    }
}

/* test if the input point is within the 8-neighbourhood of a ‘fire’ */
bool fire_surround(state* a, int width)
{
    if(     *(a - width - 1) == fire 
        ||  *(a - width    ) == fire 
        ||  *(a - width + 1) == fire 
        ||  *(a         - 1) == fire
        ||  *(a         + 1) == fire
        ||  *(a + width - 1) == fire
        ||  *(a + width    ) == fire
        ||  *(a + width + 1) == fire){
            return true;
    }
    else{
        return false;
    }
}

/* make the input generation state into the next generation */
void next_state(state a[HEIGHT][WIDTH], int g, int l)
{
    int x;
    int y;
    state next[HEIGHT][WIDTH];
    state a_safe[HEIGHT + 2][WIDTH + 2];
    state next_safe[HEIGHT + 2][WIDTH + 2];
    set_safeguard(next, next_safe);
    set_safeguard(a, a_safe);
    state_equal(next, a);
    /* iterate from 1 to (HEIGHT + 1) and from 1 to (WIDTH + 1) 
    because of only updating the state inside the empty ring */
    for(y = 1; y < HEIGHT + 1; y++){
        for(x = 1; x < WIDTH + 1; x++){
            switch(a_safe[y][x]){
                case empty:
                if(rand() % g == 0){
                    next_safe[y][x] = tree;
                }
                else{
                    next_safe[y][x] = empty;
                }
                break;
                case tree:
                if(    rand() % l == 0 
                    || fire_surround(&a_safe[y][x], WIDTH + 2) == true){
                    next_safe[y][x] = fire;
                }
                else{
                    next_safe[y][x] = tree;
                }
                break;
                case fire:
                next_safe[y][x] = empty;
                break;
            }
        }
    }
    quit_safeguard(next_safe, next);
    state_equal(a, next);
}



/* code to clear screen */
void clear_screen(void)
{
   printf("\033[2J");
}

/* color setting function */
void ch_color(const color c)
{
   printf("\033[%dm", c);
}

/* function to define the time between frames */
void busy_wait(const double secs)
{
   clock_t t2;
   const clock_t t1 = clock();
   do{
      t2 = clock();
   }while((t2-t1) < (clock_t)((double)CLOCKS_PER_SEC*secs));
}

/* print out the input generation state */
void print_state(state a[HEIGHT][WIDTH])
{
    int x;
    int y;
    for(y = 0; y < HEIGHT; y++){
        for(x = 0; x < WIDTH; x++){
            switch(a[y][x]){
                case empty:
                printf(" ");
                break;
                case tree:
                ch_color(green);
                printf("@");
                ch_color(white);
                break;
                case fire:
                ch_color(red);
                printf("*");
                ch_color(white);
                break;
            }
        }
        printf("\n");
    }
    printf("\n");
}
