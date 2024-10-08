# About

This repository develops a program that creates a graduation plan for a student based on a graph of the courses in their carreer

# Building

Developed for Linux-based machines, check [install_dependencies.sh](install_dependencies.sh)

`g++ main.cpp`

## Converting graph to png

To primarily check the integrity of the main graph of all courses and their dependencies (edges), graphviz is used to visualize the graph.

The program produces a .gv file called min.gv, where the graph was written to. This file can be changed to a .png file for better visualization by:
`dot -Tpng min.gv -o graph.png`

Either the course names or course number can be displayed as label for each vertex, check vertexWriter in [node.h](node.h)

