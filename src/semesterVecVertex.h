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

    semesterVecVertex() : credits(0), difficulty(0), maxCredits(15) {}
    semesterVecVertex(std::vector<Vertex> courses_, uint8_t credits_, uint16_t difficulty_): courses(courses_), credits(credits_), maxCredits(15), difficulty(difficulty_) {} 
    semesterVecVertex(std::vector<Vertex> courses_, uint8_t credits_, uint16_t difficulty_, uint16_t maxCredits_): courses(courses_), credits(credits_), maxCredits(maxCredits_), difficulty(difficulty_) {} 
    semesterVecVertex(uint8_t maxCredits_): credits(0), maxCredits(maxCredits_), difficulty(0) {}
    ~semesterVecVertex() = default;
};

void printScheduleVertexes(std::vector<semesterVecVertex> &scheduleToPrint){
    std::cout<<"Schedule plan:\n";
    for(auto sem : scheduleToPrint){
        for(auto v : sem.courses){
            node& nodeData = boost::get(boost::vertex_name, G, v);
            std::cout << std::to_string(nodeData.getCRS()) << " " << nodeData.getName() << "    ";
        }
        std::cout << std::endl;
    }
}