#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define WIDTH 80
#define HEIGHT 30
#define NUMBER_OF_GENERATION 1000
#define INITIAL_G 250
#define INITIAL_L 2500

typedef enum state {empty, tree, fire} state;
typedef enum bool {false, true} bool;

void test(void);
void state_equal(state a[HEIGHT][WIDTH], state b[HEIGHT][WIDTH]);
void initial_state(state c[HEIGHT][WIDTH]);
void set_safeguard(state a[HEIGHT][WIDTH], state b[HEIGHT + 2][WIDTH + 2]);
void quit_safeguard(state a[HEIGHT + 2][WIDTH + 2], state b[HEIGHT][WIDTH]);
bool fire_surround(state* a, int width);
void next_state(state c[HEIGHT][WIDTH], int g, int l);
void print_state(state c[HEIGHT][WIDTH]);


int main(void)
{
    int g = INITIAL_G;
    int l = INITIAL_L;
    state x[HEIGHT][WIDTH];
    int i;
    int j;
    char next_fram;

    test();

    initial_state(x);

    for(i = 0; i < NUMBER_OF_GENERATION; i++){
        

        for(j = 0; j < 80; j++){
            printf("=");
        }
        printf("\n%d\n", i+1);
        next_state(x, g, l);
        print_state(x);
        printf("\nPress enter to next state\n");
        next_fram = getchar(); 
    }
    return 0;
}

void test(void)
{   
    int i;
    int j;
    state a[HEIGHT][WIDTH];
    state b[HEIGHT][WIDTH];
    state c[HEIGHT][WIDTH];
    state a_safe[HEIGHT + 2][WIDTH + 2];
    state d[6][6] = {{empty,  empty, empty, empty, empty, empty}, 
                    {empty,  empty, empty,  tree, empty, empty}, 
                    {empty,  tree , empty,  empty, fire, empty},
                    {empty,  empty,  fire, empty,  tree, empty},
                    {empty,  empty,  tree, empty,  tree, empty},
                    {empty,  empty, empty, empty, empty, empty}};
    /* test for bool fire_surround(state* a, int width) */
    assert(fire_surround(&d[1][1], 6) == false);
    assert(fire_surround(&d[2][2], 6) == true );
    assert(fire_surround(&d[3][4], 6) == true );
    assert(fire_surround(&d[4][4], 6) == false );

    /* test void initial_state(state c[HEIGHT][WIDTH]) */
    initial_state(a);
    assert(a[0][0] == empty);
    assert(a[0][1] == empty);
    assert(a[4][4] == empty);

    /* make array-a switch between tree and fire 
    like {{tree, fire, tree, fire, tree......fire},
          {fire, tree, fire, tree, fire......tree},
          {tree, fire, tree, fire, tree......fire},
          {fire, tree, fire, tree, fire......tree},
          {tree, fire, tree, fire, tree......fire},
                           .
                           .
                           .
          {fire, tree, fire, tree, fire......tree}}*/
    for(i = 0; i < HEIGHT; i++){
        for(j = 0; j < WIDTH; j++){
            if((i + j)%2 == 0){
                a[i][j] = tree;
            }
            else{
                a[i][j] = fire;
            }
        }
    }

    /* test void state_equal(state a[HEIGHT][WIDTH], state b[HEIGHT][WIDTH]) */
    state_equal(b, a);
    assert(b[0][0] == tree);
    assert(b[0][1] == fire);
    assert(b[4][4] == tree);

    /* test void set_safeguard(state a[HEIGHT][WIDTH], state a_safe[HEIGHT + 2][WIDTH + 2]) */
    set_safeguard(a, a_safe);
    assert(a_safe[0][0]           == empty);
    assert(a_safe[HEIGHT + 1][20] == empty);
    assert(a_safe[13][WIDTH + 1]  == empty);
    assert(a_safe[HEIGHT + 1][WIDTH - 1]  == empty);
    assert(a_safe[1][1]           == tree);
    assert(a_safe[1][2]           == fire);

    /* test void quit_safeguard(state a_safe[HEIGHT + 2][WIDTH + 2], state a[HEIGHT][WIDTH]);
    The array-c here will be the same as array-a */
    quit_safeguard(a_safe, c);
    assert(c[0][0] == tree);
    assert(c[0][1] == fire);
    assert(c[4][4] == tree);

    /* test void next_state(state c[HEIGHT][WIDTH], int g, int l);
    in this state the array-a should turn into switching between fire and empty */
    next_state(a, INITIAL_G, INITIAL_L);
    assert(a[0][0] == fire);
    assert(a[26][37] == empty);
    assert(a[HEIGHT - 1][WIDTH - 1] == fire);
    assert(a[15][40] == empty);

   

}

/* make generation state-a equal to state-b */
void state_equal(state a[HEIGHT][WIDTH], state b[HEIGHT][WIDTH])
{
    int i;
    int j;
    for(i = 0; i < HEIGHT; i++){
        for(j = 0; j < WIDTH; j++){
            a[i][j] = b[i][j];
        }
    }
}

/* make the input generation state into all empty */
void initial_state(state a[HEIGHT][WIDTH])
{
    int i;
    int j;
     for(i = 0; i < HEIGHT; i++){
        for(j = 0; j < WIDTH; j++){
            a[i][j] = empty;
        }
    }
}

/* make a new generation state-a_safe with a ring of empty outside the original state-a to prevent testing ouside the array  */
void set_safeguard(state a[HEIGHT][WIDTH], state a_safe[HEIGHT + 2][WIDTH + 2])
{
    int i;
    int j;
    for(i = 0; i < HEIGHT + 2; i++){
        for(j = 0; j < WIDTH + 2; j++){
            a_safe[i][j] = empty;
        }
    }

    for(i = 1; i < HEIGHT + 1; i++){
        for(j = 1; j < WIDTH + 1; j++){
            a_safe[i][j] = a[i-1][j-1];
        }
    }
}

/* take off the outer empty ring of the state-a_safe into state-a; 
the width and height of a_safe is bigger than original one by 2 because plus one layer on 4 sides  */
void quit_safeguard(state a_safe[HEIGHT + 2][WIDTH + 2], state a[HEIGHT][WIDTH])
{
    int i;
    int j;
    for(i = 1; i < HEIGHT + 1; i++){
        for(j = 1; j < WIDTH + 1; j++){
            a[i-1][j-1] = a_safe[i][j];
        }
    }
}

/* find out if the input data point is within the 8-neighbourhood of a ‘fire’ or not */
bool fire_surround(state* a, int width)
{
    if(    *(a - width - 1) == fire 
        ||  *(a - width    ) == fire 
        ||  *(a - width + 1) == fire 
        ||  *(a         - 1) == fire
        ||  *(a         + 1) == fire
        ||  *(a + width - 1) == fire
        ||  *(a + width    ) == fire
        ||  *(a + width + 1) == fire
    )
    {return true;}
    else
    {return false;}
}

/* make the input generation state into the next generation */
void next_state(state a[HEIGHT][WIDTH], int g, int l)
{
    int i;
    int j;
    state next[HEIGHT][WIDTH];
    state a_safe[HEIGHT + 2][WIDTH + 2];
    state next_safe[HEIGHT + 2][WIDTH + 2];
    set_safeguard(next, next_safe);
    set_safeguard(a, a_safe);
    state_equal(next, a);
    /* iterate from 1 to (HEIGHT + 1) and from 1 to (HEIGHT + 1) 
    because it is only focus on updating the state inside the empty ring */
     for(i = 1; i < HEIGHT + 1; i++){
        for(j = 1; j < WIDTH + 1; j++){
            switch(a_safe[i][j]){
                case empty:
                if (rand() % g == 0){
                    next_safe[i][j] = tree;
                }else{
                    next_safe[i][j] = empty;
                }
                break;
                case tree:
                if (
                    rand() % l == 0 
                    || fire_surround(&a_safe[i][j], WIDTH + 2) == true
                    ){
                    next_safe[i][j] = fire;
                }else{
                    next_safe[i][j] = tree;
                }
                break;
                case fire:
                next_safe[i][j] = empty;
                break;
            }
        }
    }
    quit_safeguard(next_safe, next);
    state_equal(a, next);
}

/* print out the input generation state */
void print_state(state a[HEIGHT][WIDTH])
{
    int i;
    int j;
     for(i = 0; i < HEIGHT; i++){
        for(j = 0; j < WIDTH; j++){
            switch(a[i][j]){
                case empty:
                printf(" ");
                break;
                case tree:
                printf("@");
                break;
                case fire:
                printf("*");
                break;
            }
        }
        printf("\n");
    }
    printf("\n");
}
