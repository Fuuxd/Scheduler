#include "config.h"
#include "node.h"

    std::vector<node> genElectives;
    std::vector<node> nodeLabs;

directedGraphCourses G;

//Regarding Complexities
// Math, science, dept core = 5
// Dept elective = 4
// Gen Ed = 3
// English = 2
// General Elective = 1

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

    std::vector<section> dummySecHardest = {
        {99999, 3, 5},
        {99999, 3, 5}
    };

    std::vector<section> dummySecHarder = {
        {99999, 3, 5},
        {99999, 3, 5}
    };

    std::vector<section> dummySecHard = {
        {99999, 3, 5},
        {99999, 3, 5}
    };

    std::vector<section> dummySecMedium = {
        {99999, 3, 5},
        {99999, 3, 5}
    };

    std::vector<section> dummySecEasy = {
        {99999, 3, 5},
        {99999, 3, 5}
    };
    

    // Declare and initialize a node object with CRS and the vector of sections
    // from sections - sections 6 correspond to these nodes, currently modified for difficulty
    node calc1(2281, "Calculus 1", dummySecHardest);  //0
    node calc2(2282, "Calculus 2", dummySecHardest); //1
    node calc3(2283, "Calculus 3", dummySecHardest); //2
    node comp1(1101, "Composition 1", dummySecMedium); //3
    node comp2(1102, "Compostion 2", dummySecMedium); //4
    node foundatEng(3000, "Foundations of Engineering", true, dummySecHard); //5

    node phys1(2048, "Physics 1", true, dummySecHardest); //6

    node cop1(2510, "Programming Concepts", dummySecHardest); //7
    node discStruct(3100, "Discrete Structures", dummySecHardest); //8
    node phys2(2049, "Physics 2", true, dummySecHardest); //9

    node cop2(3514, "Program Design", dummySecHardest); //10
    node cda1(3103, "Computer Organization", dummySecHardest); //11
    node cop3(4530, "Data Structures", dummySecHardest); //12
    node cda2(3201, "Computer Logic Design", true, dummySecHardest); //13

    node ProbNStats(3443, "Probability and Statistics for Engineers", dummySecHardest); //14
    node linSys(4450, "Linear Systems", dummySecHardest); //15
    node cda3(4205, "Computer Architecture", true, dummySecHardest); //16

    node cop4(4400, "Analysis of Algorithms", dummySecHardest); //17
    node CommEng(3246, "Communication for Engineers", dummySecHardest, 60); //18
    node secCode(4419, "Secure Coding", dummySecHardest); //19
    node opSys(4600, "Operating Systems", dummySecHardest); //20
    node softEng(4020, "Software Engineering", dummySecHardest); //21
    node ethics(4250, "Ethical Issues & Prof. Conduct", dummySecHardest, 112); //22

    node SoftEl1(7777, "CSE Elective", dummySecHard);//23
    node SoftEl2(7777, "CSE Elective", dummySecHard);//24
    node SoftEl3(7777, "CSE Elective", dummySecHard);//25
    node SoftEl4(7777, "CSE Elective", dummySecHard);//26
    node SoftEl5(7777, "CSE Elective", dummySecHard);//27
    node SoftEl6(7777, "CSE Elective", dummySecHard);//28

    //I will determine labs to be dummySecMed
    node foundatEngL(3000, "Foundations of Engineering Lab", dummySecMedium); //corresponds to sections7
    node phys1L(2048, "Physics 1 Lab", dummySecMedium); 
    node phys2L(2049, "Physics 2 Lab", dummySecMedium); 
    node cda2L(3201, "Computer Logic Design Lab", dummySecMedium); 
    node cda3L(4205, "Computer Architecture Lab", dummySecMedium); 

    node GenEd1(2999, "General Elective", dummySecEasy); 
    node GenEd2(2999, "General Elective", dummySecEasy); 
    node GenEd3(2999, "General Elective", dummySecEasy); 
    node GenEd4(2999, "General Elective", dummySecEasy); 
    node GenEd5(2999, "General Elective", dummySecEasy); 
    node GenEd6(2999, "General Elective", dummySecEasy); 
    node GenEd7(2999, "General Elective", dummySecEasy); 
    node GenEd8(2999, "General Elective", dummySecEasy); 
    node GenEd9(2999, "General Elective", dummySecEasy); 
    node GenEd10(2999, "General Elective", dummySecEasy); 

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
    boost::add_vertex(cda1, G);
    boost::add_vertex(cop3, G);
    boost::add_vertex(cda2, G);
    boost::add_vertex(ProbNStats, G);
    boost::add_vertex(linSys, G);
    boost::add_vertex(cda3, G);
    boost::add_vertex(cop4, G);
    boost::add_vertex(CommEng, G);
    boost::add_vertex(secCode, G);
    boost::add_vertex(opSys, G);
    boost::add_vertex(softEng, G);
    boost::add_vertex(ethics, G);
    boost::add_vertex(SoftEl1, G);
    boost::add_vertex(SoftEl2, G);
    boost::add_vertex(SoftEl3, G);
    boost::add_vertex(SoftEl4, G);
    boost::add_vertex(SoftEl5, G);
    boost::add_vertex(SoftEl6, G);
    //boost::add_vertex(GenEd1, G);
    //boost::add_vertex(GenEd2, G);
    //boost::add_vertex(GenEd3, G);
    //boost::add_vertex(GenEd4, G);
    //boost::add_vertex(GenEd5, G);
    //boost::add_vertex(GenEd6, G);
    //boost::add_vertex(GenEd7, G);
    //boost::add_vertex(GenEd8, G);
    //boost::add_vertex(GenEd9, G);
    //boost::add_vertex(GenEd10, G);

    boost::add_edge(0, 1, G);
    boost::add_edge(1, 2, G);
    boost::add_edge(3, 4, G);
    boost::add_edge(0, 6, G);
    boost::add_edge(0, 7, G);
    boost::add_edge(0, 8, G);
    boost::add_edge(6, 9, G);
    boost::add_edge(1, 9, G);
    boost::add_edge(1, 14, G);
    boost::add_edge(1, 15, G);
    boost::add_edge(7, 11, G);
    boost::add_edge(7, 10, G);
    boost::add_edge(8, 12, G);
    boost::add_edge(10, 12, G);
    boost::add_edge(10, 13, G);
    boost::add_edge(11, 12, G);
    boost::add_edge(11, 13, G);
    boost::add_edge(13, 16, G);
    boost::add_edge(12, 17, G);
    boost::add_edge(12, 19, G);
    boost::add_edge(12, 20, G);
    boost::add_edge(12, 21, G);

    //for electives: prereqs are cda2 and cop3

    boost::add_edge(12, 23, G);
    boost::add_edge(13, 23, G);
    boost::add_edge(12, 24, G);
    boost::add_edge(13, 24, G);
    boost::add_edge(12, 25, G);
    boost::add_edge(13, 25, G);
    boost::add_edge(12, 26, G);
    boost::add_edge(13, 26, G);
    boost::add_edge(12, 27, G);
    boost::add_edge(13, 27, G);
    boost::add_edge(12, 28, G);
    boost::add_edge(13, 28, G);


    genElectives = {GenEd1, GenEd2, GenEd3, GenEd4, GenEd5, GenEd6, GenEd7, GenEd8, GenEd9, GenEd10};
    nodeLabs = {foundatEngL, phys1L, phys2L, cda2L, cda3L };
}

