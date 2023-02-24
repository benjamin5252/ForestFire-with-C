# ForestFire-with-C

![forest-fire](https://user-images.githubusercontent.com/31298786/147691103-377c0161-71d5-408b-8036-88cbb594b512.gif)

# Description
It is a c implementation of forest-fire model. According to the deffinition in Wiki (https://en.wikipedia.org/wiki/Forest-fire_model). The model is defined as a cellular automaton on a grid with Ld cells. L is the sidelength of the grid and d is its dimension. A cell can be empty, occupied by a tree, or burning. The model of Drossel and Schwabl (1992) is defined by four rules which are executed simultaneously: <br />
<br />
1. A burning cell turns into an empty cell <br />
2. A tree will burn if at least one neighbor is burning <br />
3. A tree ignites with probability f even if no neighbor is burning <br />
4. An empty space fills with a tree with probability p <br />


### @: Tree 
### *: Fire 

# Instruction
compile and run the forestFire.c. with gcc
