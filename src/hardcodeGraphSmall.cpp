#include "config.h"
#include "node.h"

std::vector<node> genElectives;
std::vector<node> nodeLabs;

directedGraphCourses G;

//for easy collapsing

void hardcodeGraph(){

    ////  HARDCODED nodes to test

    // ENG CALC1
    std::vector<section> sections = {
        {92256, 4, 2},
        {92256, 4, 2},
        {92257, 4, 2},

        {92258, 4, 2},
        {92264, 4, 2}
    };

    // ENG CALC2
    std::vector<section> sections2 = {
        {92273, 4, 3},
        {92272, 4, 3}
    };

    // ENG CALC3
    std::vector<section> sections3 = {
        {92312, 4, 3},
        {92314, 4, 3}
    };

    // COMP 1   
    std::vector<section> sections4 = {
        {89232, 3, 1},
        {89233, 3, 1},
        {89246, 3, 1}
    };

    // COMP 2
    std::vector<section> sections5 = {
        {89232, 3, 1},
        {89233, 3, 1},
        {89246, 3, 1}
    };

    // FOUNDATIONS OF ENGINEERING 
    std::vector<section> sections6 = {
        {89793, 0, 1},
        {89802, 0, 1}
    };

    // FOUNDATIONS OF ENGINEERING LAB
    std::vector<section> sections7 = {
        {89775, 3, 1},
        {96439, 3, 1}
    };

    std::vector<section> sections0 = {
        {99999, 3, 1},
        {99999, 3, 1}
    };

    // Declare and initialize a node object with CRS and the vector of sections
    node calc1(2281, "Calculus 1", sections);  //0
    node calc2(2282, "Calculus 2", sections2); //1
    node calc3(2283, "Calculus 3", sections3); //2
    node comp1(1101, "Composition 1", sections4); //3
    node comp2(1102, "Compostion 2", sections5); //4
    node foundatEng(3000, "Foundations of Engineering", true, sections6); //5

    node phys1(2048, "Physics 1", true, sections0); //6

    node cop1(2510, "Programming Concepts", sections0); //7
    node discStruct(3100, "Discrete Structures", sections0); //8
    node phys2(2049, "Physics 2", true, sections0); //9

    node cop2(3514, "Program Design", sections0); //10

    node foundatEngL(3000, "Foundations of Engineering Lab", sections7); 
    node phys1L(2048, "Physics 1 Lab", sections0); 
    node phys2L(2049, "Physics 2 Lab", sections0); 
    node cda2L(3201, "Computer Logic Design Lab", sections0); 
    node cda3L(4205, "Computer Architecture Lab", sections0); 

    node GenEd1(2999, "General Elective", sections0); 
    node GenEd2(2999, "General Elective", sections0); 
    node GenEd3(2999, "General Elective", sections0); 
    node GenEd4(2999, "General Elective", sections0); 
    node GenEd5(2999, "General Elective", sections0); 
    node GenEd6(2999, "General Elective", sections0); 
    node GenEd7(2999, "General Elective", sections0); 
    node GenEd8(2999, "General Elective", sections0); 
    node GenEd9(2999, "General Elective", sections0); 
    node GenEd10(2999, "General Elective",  sections0); 

    boost::add_vertex(calc1, G);
    boost::add_vertex(calc2, G);
    boost::add_vertex(calc3, G);
    boost::add_vertex(comp1, G);
    boost::add_vertex(comp2, G);
    boost::add_vertex(foundatEng, G);
    boost::add_vertex(phys1, G);
    boost::add_vertex(cop1, G);
    boost::add_vertex(discStruct, G);
    boost::add_vertex(phys2, G);
    boost::add_vertex(cop2, G);

    boost::add_edge(0, 1, G);
    boost::add_edge(1, 2, G);
    boost::add_edge(3, 4, G);
    boost::add_edge(0, 6, G);
    boost::add_edge(0, 7, G);
    boost::add_edge(0, 8, G);
    boost::add_edge(6, 9, G);
    boost::add_edge(1, 9, G);
    boost::add_edge(7, 10, G);


    std::vector<node> genElectives = {GenEd1, GenEd2, GenEd3, GenEd4, GenEd5, GenEd6, GenEd7, GenEd8, GenEd9, GenEd10};
    std::vector<node> nodeLabs = {foundatEngL, phys1L, phys2L, cda2L, cda3L };
}

