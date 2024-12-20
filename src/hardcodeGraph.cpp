#pragma once
#include "config.h"
#include "node.h"

std::vector<node> genElectives;
std::vector<node> nodeLabs;

std::vector<std::vector<Vertex>> baseSchedule;

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
        {89775, 3, 4},
        {96439, 3, 4}
    };

    std::vector<section> sections0 = {
        {99999, 3, 1},
        {99999, 3, 1}
    };

    std::vector<section> dummySecHardest = {
        {99999, 3, 5},
        {99999, 3, 5}
    };

    std::vector<section> dummySecHardest2Cred = {
        {99999, 2, 5},
        {99999, 2, 5}
    };

    std::vector<section> dummySecHardest4Cred = {
        {99999, 4, 5},
        {99999, 4, 5}
    };

    std::vector<section> dummySecHarder = {
        {99999, 3, 4},
        {99999, 3, 4}
    };

    std::vector<section> dummySecHard = {
        {99999, 3, 3},
        {99999, 3, 3}
    };

    std::vector<section> dummySecMedium = {
        {99999, 3, 2},
        {99999, 3, 2}
    };

    std::vector<section> dummySecMediumLabs = {
        {99999, 1, 2},
        {99999, 1, 2}
    };

    std::vector<section> dummySecEasy = {
        {99999, 3, 1},
        {99999, 3, 1}
    };
    

    // Declare and initialize a node object with CRS and the vector of sections
    // from sections - sections 6 correspond to these nodes, currently modified for difficulty
    node calc1(2281, "Calculus 1", dummySecHardest4Cred);  //0
    node calc2(2282, "Calculus 2", dummySecHardest4Cred); //1
    node calc3(2283, "Calculus 3", dummySecHardest); //2
    node comp1(1101, "Composition 1", dummySecMedium); //3
    node comp2(1102, "Composition 2", dummySecMedium); //4
    node foundatEng(3000, "Foundations of Engineering", true, dummySecHard); //5

    node phys1(2048, "Physics 1", true, dummySecHardest4Cred); //6

    node cop1(2510, "Programming Concepts", dummySecHardest); //7
    node discStruct(3100, "Discrete Structures", dummySecHardest); //8
    node phys2(2049, "Physics 2", true, dummySecHardest4Cred); //9

    node cop2(3514, "Program Design", dummySecHardest); //10
    node cda1(3103, "Computer Organization", dummySecHardest); //11
    node cop3(4530, "Data Structures", dummySecHardest); //12
    node cda2(3201, "Computer Logic Design", true, dummySecHardest4Cred); //13

    node ProbNStats(3443, "Probability and Statistics for Engineers", dummySecHardest); //14
    node linSys(4450, "Linear Systems", dummySecHardest2Cred); //15
    node cda3(4205, "Computer Architecture", true, dummySecHardest4Cred); //16

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
    node foundatEngL(3000, "Foundations of Engineering Lab", sections7); //corresponds to sections7
    node phys1L(2048, "Physics 1 Lab", dummySecMediumLabs); 
    node phys2L(2049, "Physics 2 Lab", dummySecMediumLabs); 
    node cda2L(3201, "Computer Logic Design Lab", dummySecMediumLabs); 
    node cda3L(4205, "Computer Architecture Lab", dummySecMediumLabs); 

    node GenEd1(2999, "General Elective", dummySecMedium); 
    node GenEd2(2999, "General Elective", dummySecMedium); 
    node GenEd3(2999, "General Elective", dummySecMedium); 
    node GenEd4(2999, "General Elective", dummySecMedium); 
    node GenEd5(2999, "General Elective", dummySecMedium); 
    node GenEd6(2999, "General Elective", dummySecMedium); 
    node GenEd7(2999, "General Elective", dummySecMedium); 
    node GenEd8(2999, "General Elective", dummySecMedium); 
    node GenEd9(2999, "General Elective", dummySecMedium); 
    node GenEd10(2999, "General Elective", dummySecMedium); 

    boost::add_vertex(calc1, G); //0
    boost::add_vertex(calc2, G); //1
    boost::add_vertex(comp1, G); //2
    boost::add_vertex(comp2, G); //3
    boost::add_vertex(foundatEng, G); //4 
    boost::add_vertex(phys1, G); //5 
    boost::add_vertex(cop1, G); //6
    boost::add_vertex(discStruct, G); //7
    boost::add_vertex(phys2, G); //8
    boost::add_vertex(cop2, G); //9
    boost::add_vertex(cda1, G); //10
    boost::add_vertex(cop3, G); //11
    boost::add_vertex(cda2, G); //12
    boost::add_vertex(ProbNStats, G); //13
    boost::add_vertex(linSys, G); //14
    boost::add_vertex(cda3, G); //15
    boost::add_vertex(cop4, G); //16
    boost::add_vertex(CommEng, G); //17
    boost::add_vertex(secCode, G); //18
    boost::add_vertex(opSys, G); //19
    boost::add_vertex(softEng, G); //20
    boost::add_vertex(ethics, G); //21
    boost::add_vertex(SoftEl1, G); //22
    boost::add_vertex(SoftEl2, G); //23
    boost::add_vertex(SoftEl3, G); //24
    boost::add_vertex(SoftEl4, G); //25
    boost::add_vertex(SoftEl5, G); //26
    boost::add_vertex(SoftEl6, G); //27
    boost::add_vertex(GenEd1, G); //28
    boost::add_vertex(GenEd2, G); //29
    boost::add_vertex(GenEd3, G); //30
    boost::add_vertex(GenEd4, G); //31
    boost::add_vertex(GenEd5, G); //32
    boost::add_vertex(GenEd6, G); //33
    boost::add_vertex(GenEd7, G); //34
    boost::add_vertex(GenEd8, G); //35
    boost::add_vertex(GenEd9, G); //36
    boost::add_vertex(GenEd10, G); //37

    boost::add_edge(0, 1, G);  // calc1 -> calc2
    boost::add_edge(2, 3, G);  // comp1 -> comp2
    boost::add_edge(0, 5, G);  // calc1 -> phys1
    boost::add_edge(0, 6, G);  // calc1 -> cop1
    boost::add_edge(0, 7, G);  // calc1 -> discStruct
    boost::add_edge(5, 8, G);  // phys1 -> phys2
    boost::add_edge(1, 8, G);  // calc2 -> phys2
    boost::add_edge(1, 13, G); // calc2 -> ProbNStats
    boost::add_edge(1, 14, G); // calc2 -> linSys
    boost::add_edge(6, 10, G); // cop1 -> cda1
    boost::add_edge(6, 9, G);  // cop1 -> cop2
    boost::add_edge(7, 11, G); // discStruct -> cop3
    boost::add_edge(9, 11, G); // cop2 -> cop3
    boost::add_edge(9, 12, G); // cop2 -> cda2
    boost::add_edge(10, 11, G); // cda1 -> cop3
    boost::add_edge(10, 12, G); // cda1 -> cda2
    boost::add_edge(12, 15, G); // cda2 -> cda3
    boost::add_edge(11, 16, G); // cop3 -> cop4
    boost::add_edge(11, 18, G); // cop3 -> secCode
    boost::add_edge(11, 19, G); // cop3 -> opSys
    boost::add_edge(11, 20, G); // cop3 -> softEng

    // Elective prerequisites
    boost::add_edge(11, 22, G); // cop3 -> SoftEl1
    boost::add_edge(12, 22, G); // cda2 -> SoftEl1
    boost::add_edge(11, 23, G); // cop3 -> SoftEl2
    boost::add_edge(12, 23, G); // cda2 -> SoftEl2
    boost::add_edge(11, 24, G); // cop3 -> SoftEl3
    boost::add_edge(12, 24, G); // cda2 -> SoftEl3
    boost::add_edge(11, 25, G); // cop3 -> SoftEl4
    boost::add_edge(12, 25, G); // cda2 -> SoftEl4
    boost::add_edge(11, 26, G); // cop3 -> SoftEl5
    boost::add_edge(12, 26, G); // cda2 -> SoftEl5
    boost::add_edge(11, 27, G); // cop3 -> SoftEl6
    boost::add_edge(12, 27, G); // cda2 -> SoftEl6


    genElectives = {GenEd1, GenEd2, GenEd3, GenEd4, GenEd5, GenEd6, GenEd7, GenEd8, GenEd9, GenEd10};
    nodeLabs = {foundatEngL, phys1L, phys2L, cda2L, cda3L };

    baseSchedule = {
        {boost::vertex(28, G), boost::vertex(0, G), boost::vertex(4, G), boost::vertex(2, G)},   // GenEd1, calc1, foundatEng, foundatEngL, comp1
        {boost::vertex(5, G), boost::vertex(1, G), boost::vertex(6, G), boost::vertex(3, G)},    // phys1, phys1L, calc2, cop1, comp2
        {boost::vertex(8, G), boost::vertex(10, G), boost::vertex(9, G), boost::vertex(7, G)},   // phys2, phys2L, cda1, cop2, discStruct
        {boost::vertex(12, G), boost::vertex(11, G), boost::vertex(29, G), boost::vertex(30, G)}, // cda2, cda2L, cop3, GenEd2, GenEd3
        {boost::vertex(13, G), boost::vertex(31, G), boost::vertex(32, G)},  // ProbNStats, GenEd4, GenEd5
        {boost::vertex(14, G), boost::vertex(22, G), boost::vertex(15, G), boost::vertex(33, G), boost::vertex(34, G)},  // linSys, SoftEl1, cda3, cda3L, GenEd6, GenEd7
        {boost::vertex(23, G), boost::vertex(24, G), boost::vertex(16, G), boost::vertex(17, G), boost::vertex(35, G)},  // SoftEl2, SoftEl3, cop4, CommEng, GenEd8
        {boost::vertex(25, G), boost::vertex(26, G), boost::vertex(18, G), boost::vertex(19, G), boost::vertex(36, G)},  // SoftEl4, SoftEl5, secCode, opSys, GenEd9
        {boost::vertex(27, G), boost::vertex(20, G), boost::vertex(21, G), boost::vertex(37, G)}  // SoftEl6, softEng, ethics, GenEd10
    };
}

