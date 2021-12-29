# ForestFire-with-C

![forest-fire](https://user-images.githubusercontent.com/31298786/147686647-b067dc26-97d4-4401-b15c-fe58c09ca222.gif)

# Description
It is a c implementation of forest-fire model. According to the deffinition in Wiki (https://en.wikipedia.org/wiki/Forest-fire_model). The model is defined as a cellular automaton on a grid with Ld cells. L is the sidelength of the grid and d is its dimension. A cell can be empty, occupied by a tree, or burning. The model of Drossel and Schwabl (1992) is defined by four rules which are executed simultaneously: <br />
<br />
1. A burning cell turns into an empty cell <br />
2. A tree will burn if at least one neighbor is burning <br />
3. A tree ignites with probability f even if no neighbor is burning <br />
4. An empty space fills with a tree with probability p <br />

<br />
### @: Tree 
### *: Fire 

# Instructions
Press enter to print the next state of the forestfire after compiling and executing the code.
