#pragma once

#include "config.h"
#include "node.h"
#include "hardcodeGraph.cpp"

class semesterVector
{
public:
    std::vector<node> courses;
    uint8_t credits;
    uint16_t difficulty;
    uint8_t nonElectiveCredits;

    semesterVector() : credits(0), difficulty(0), nonElectiveCredits(0) {}
    semesterVector(std::vector<node> courses_, uint8_t credits_, uint16_t difficulty_): courses(courses_), credits(credits_), nonElectiveCredits((credits_ * 21)/25), difficulty(difficulty_) {} //ARBITRARY IMPORTANCE OF ELECTIVES, determining here what proportion of "important courses" to take
    semesterVector(uint8_t maxCredits_): credits(0), nonElectiveCredits((maxCredits_ * 21)/25), difficulty(0) {}
    ~semesterVector() = default;

    void printCourses() const{
        for (size_t i = 0; i < courses.size(); i++) {
            // Print each course with a width of 40 characters for alignment
            std::cout << std::to_string(courses[i].getCRS()) + " " + courses[i].getName() << "    ";  
        }
    }

    void printDifficulty() const{
        std::cout << std::to_string(difficulty) + " " + std::to_string(credits) + " \n";
    }
};



void printSchedule(const std::vector<semesterVector>* schedule) {
    for (size_t i = 0; i < schedule->size(); i++) {
        (*schedule)[i].printCourses();
        (*schedule)[i].printDifficulty();

    }
    std::cout << std::endl;
}

float getComplexity(std::vector<semesterVector>* schedule){
    uint16_t maxCredDiffic = INT8_MIN;
    uint16_t minCredDiffic = INT8_MAX;

    for (size_t i = 0; i < schedule->size(); i++) {

        uint16_t CredDiff = (*schedule)[i].credits + (*schedule)[i].difficulty;
        if(CredDiff > maxCredDiffic){
            maxCredDiffic = CredDiff;
        }
        if(CredDiff < minCredDiffic){
            minCredDiffic = CredDiff;
        }
    }

    int32_t difference = maxCredDiffic - minCredDiffic;

    if( difference < 0){
        //std::cerr << " error obtaining Complexity of a schedule. check for possible empty schedules";
        return FLT_MAX;
    }

    return  ((difference) + (schedule->size() * 2.5) + (maxCredDiffic * 0.6));
}

//float getComplexity(std::vector<semesterVector>* schedule) {
//    // ASSUMES baseSchedule is already hardCoded
//    float totalDistance = 0.0f;
//
//    // For each semester in input schedule
//    for (size_t i = 0; i < schedule->size(); i++) {
//        // For each course in current semester
//        for (const auto& course : (*schedule)[i].courses) {
//            bool found = false;
//            
//            // Search for this course in base schedule
//            for (size_t j = 0; j < baseSchedule.size(); j++) {
//                for (const auto& baseNode : baseSchedule[j]) {
//                    if (baseNode.getCRS() == course.getCRS()) {
//                        // Add distance between semester positions
//                        totalDistance += abs(static_cast<float>(i) - static_cast<float>(j));
//                        found = true;
//                        break;
//                    }
//                }
//                if (found) break;
//            }
//            
//            // If course wasn't found, add maximum penalty
//            if (!found) {
//                totalDistance += baseSchedule.size();
//            }
//        }
//    }
//    
//    return totalDistance;
//}