#pragma once

#include "config.h"
#include "node.h"
#include "hardcodeGraph.cpp"

class semesterVecVertex
{
public:
    std::vector<Vertex> courses;
    uint8_t credits;
    uint16_t difficulty;
    uint8_t maxCredits;
    bool isSummer;

    semesterVecVertex() : credits(0), difficulty(0), maxCredits(15), isSummer(0){}
    semesterVecVertex(std::vector<Vertex> courses_, uint8_t credits_, uint16_t difficulty_): courses(courses_), credits(credits_), maxCredits(15), difficulty(difficulty_), isSummer(0) {} 
    semesterVecVertex(std::vector<Vertex> courses_, uint8_t credits_, uint16_t difficulty_, uint16_t maxCredits_): courses(courses_), credits(credits_), maxCredits(maxCredits_), difficulty(difficulty_), isSummer(0) {} 
    semesterVecVertex(uint8_t maxCredits_): credits(0), maxCredits(maxCredits_), difficulty(0), isSummer(0) {}
    ~semesterVecVertex() = default;
};

void printScheduleVertexes(std::vector<semesterVecVertex> &scheduleToPrint){
    std::cout<<"Schedule plan:\n";
    size_t semNum = 1;
    for(auto sem : scheduleToPrint){
        if(sem.isSummer){
            std::cout << "Summer :\n";
        }else{
            std::cout<< "Semester " << std::to_string(semNum) << ":\n";
        }
        for(auto v : sem.courses){
            node& nodeData = boost::get(boost::vertex_name, G, v);
            std::cout << std::to_string(nodeData.getCRS()) << " " << nodeData.getName() << "\n";
        }
        std::cout << std::to_string(sem.credits) << " " << std::to_string(sem.maxCredits);
        std::cout << std::endl << std::endl;
        semNum++;
    }
}