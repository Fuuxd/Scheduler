# About

This repository develops a program that creates a graduation plan for a student based on a graph of the courses in their career

# Building

Developed for Linux-based machines,
Run first [installDependencies.sh](installDependencies.sh)
And afterwards, every time you wish to use the program run [webUI.sh](webUI.sh)

`chmod +x installDependencies.sh webUI.sh`
`sudo ./installDependencies.sh`
`sudo ./webUI.sh`

# Setting up the Catalog Four-Year Plan

In case the Catalog Four-Year Plan changes changing [hardcodeGraph.cpp](./src/hardcodeGraph.cpp) will be needed, specifically to global vector 'baseSchedule'

When doing so, this graph can be checked after compiling and running main.cpp through its graphConverted.svg and schedule.html outputs.