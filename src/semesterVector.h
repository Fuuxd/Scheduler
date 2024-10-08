#pragma once

#include "config.h"
#include "node.h"

class semesterVector
{
public:
    std::vector<node> courses;
    uint8_t credits;
    uint8_t difficulty;
    uint8_t nonElectiveCredits;

    semesterVector() : credits(0), difficulty(0), nonElectiveCredits(0) {}
    semesterVector(std::vector<node> courses_, uint8_t credits_, uint8_t difficulty_): courses(courses_), credits(credits_), nonElectiveCredits((credits_ * 21)/25), difficulty(difficulty_) {} //ARBITRARY IMPORTANCE OF ELECTIVES, determining here what proportion of "important courses" to take
    semesterVector(uint8_t maxCredits_): credits(0), nonElectiveCredits((maxCredits_ * 21)/25) {}
    ~semesterVector() = default;

    void printCourses() const{
        for (size_t i = 0; i < courses.size(); i++) {
            // Print each course with a width of 40 characters for alignment
            std::cout << std::to_string(courses[i].getCRS()) + " " + courses[i].getName() << "    ";  
        }
    }
};



void printSchedule(const std::vector<semesterVector>* schedule) {
    for (size_t i = 0; i < schedule->size(); i++) {
        (*schedule)[i].printCourses();
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

float getComplexity(std::vector<semesterVector>* schedule){
    uint8_t maxCredDiffic = INT8_MIN;
    uint8_t minCredDiffic = INT8_MAX;

    for (size_t i = 0; i < schedule->size(); i++) {

        uint8_t CredDiff = (*schedule)[i].credits + (*schedule)[i].difficulty;
        if(CredDiff > maxCredDiffic){
            maxCredDiffic = CredDiff;
        }
        if(CredDiff < minCredDiffic){
            minCredDiffic = CredDiff;
        }
    }

    int16_t difference = maxCredDiffic - minCredDiffic;

    if( difference < 0){
        //std::cerr << " error obtaining Complexity of a schedule. check for possible empty schedules";
        return FLT_MAX;
    }

    return  ((difference) + (schedule->size() * 2.5) + (maxCredDiffic * 0.6));
}