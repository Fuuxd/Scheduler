#pragma once

#include "config.h"
#include "toposorts.h"
#include "semesterVector.h"
#include "semesterVecVertex.h"

template <typename T>
void printVectorOfVectors(const std::vector<std::vector<T>>& vec) {
    for (const auto& innerVec : vec) {
        std::cout << "{ ";
        for (const auto& elem : innerVec) {
            std::cout << elem << " ";
        }
        std::cout << "}";
    }
    std::cout << std::endl;
}


void removeCompletedCourses(std::vector<std::vector<Vertex>>& baseSchedule, const std::set<Vertex>* coursesTaken) {
    // Check for null pointer
    if (!coursesTaken) {
        return;
    }

    // For each semester in the schedule
    for(std::vector<Vertex> bin : baseSchedule){
        // Iterate through the courses in the bin
        auto it = bin.begin();
        while (it != bin.end()) {
            node& nodeData = boost::get(boost::vertex_name, G, *it);
            
            bool courseCompleted = false;
            for (const Vertex& v2 : *coursesTaken) {
                node& nodeData2 = boost::get(boost::vertex_name, G, v2);
                
                if (nodeData2.getCRS() == nodeData.getCRS()) {
                    courseCompleted = true;
                    break;
                }
            }

            if (courseCompleted) {
                // Remove the completed course from the bin
                it = bin.erase(it);
            } else {
                ++it; // Only increment if we don't erase
            }
        }
    }

    // Remove any empty semesters
    baseSchedule.erase(
        std::remove_if(
            baseSchedule.begin(),
            baseSchedule.end(),
            [](const std::vector<Vertex>& semester) {
                return semester.empty();
            }
        ),
        baseSchedule.end()
    );
}

void optimizeSchedule(std::vector<semesterVecVertex>& schedule, directedGraphCourses& G) {
    if (schedule.size() <= 1) {
        return;  // Nothing to optimize
    }

    // Iterate through semesters from last to second
    for (int currentSemIndex = schedule.size() - 1; currentSemIndex > 0; --currentSemIndex) {
        auto& currentSemester = schedule[currentSemIndex];
        auto& previousSemester = schedule[currentSemIndex - 1];
        
        // Create a vector to mark vertices that should stay in current semester
        std::vector<bool> shouldStay(currentSemester.courses.size(), false);
        // Create a vector to store credits of each course
        std::vector<uint8_t> courseCredits(currentSemester.courses.size());

        // For each vertex in current semester
        for (size_t i = 0; i < currentSemester.courses.size(); ++i) {
            const auto& currentVertex = currentSemester.courses[i];
            
            // Get course credits
            node& currentNodeData = boost::get(boost::vertex_name, G, currentVertex);
            courseCredits[i] = currentNodeData.getSection().credits;
            
            // Check if any vertex in previous semester depends on this vertex
            for (const auto& prevVertex : previousSemester.courses) {
                // Get incoming edges to the previous semester vertex
                boost::graph_traits<directedGraphCourses>::in_edge_iterator ei, ei_end;
                for (boost::tie(ei, ei_end) = boost::in_edges(prevVertex, G); ei != ei_end; ++ei) {
                    if (boost::source(*ei, G) == currentVertex) {
                        shouldStay[i] = true;
                        break;
                    }
                }
                if (shouldStay[i]) break;
            }
        }

        // Create new semester structures
        semesterVecVertex newCurrentSemester;
        semesterVecVertex newPreviousSemester = previousSemester;
        
        newCurrentSemester.maxCredits = currentSemester.maxCredits;
        newCurrentSemester.credits = 0;
        newCurrentSemester.difficulty = 0;

        // Move vertices to their appropriate semesters
        for (size_t i = 0; i < currentSemester.courses.size(); ++i) {
            const auto& currentVertex = currentSemester.courses[i];
            node& currentNodeData = boost::get(boost::vertex_name, G, currentVertex);
            uint8_t courseCredit = courseCredits[i];
            uint16_t courseDifficulty = currentNodeData.getSection().difficulty;

            if (shouldStay[i]) {
                // Keep course in current semester
                newCurrentSemester.courses.push_back(currentVertex);
                newCurrentSemester.credits += courseCredit;
                newCurrentSemester.difficulty += courseDifficulty;
            } else {
                // Check if moving the course up would exceed credit limit
                if (newPreviousSemester.credits + courseCredit <= newPreviousSemester.maxCredits) {
                    // Move course up to previous semester
                    newPreviousSemester.courses.push_back(currentVertex);
                    newPreviousSemester.credits += courseCredit;
                    newPreviousSemester.difficulty += courseDifficulty;
                } else {
                    // Keep in current semester if moving up would exceed credit limit
                    newCurrentSemester.courses.push_back(currentVertex);
                    newCurrentSemester.credits += courseCredit;
                    newCurrentSemester.difficulty += courseDifficulty;
                }
            }
        }

        // Update the semesters in the schedule
        currentSemester = newCurrentSemester;
        previousSemester = newPreviousSemester;
    }

    // Create a new schedule without empty semesters
    std::vector<semesterVecVertex> newSchedule;
    
    // Only add non-empty semesters to the new schedule
    for (const auto& semester : schedule) {
        if (!semester.courses.empty()) {
            newSchedule.push_back(semester);
        }
    }

    // Update the schedule
    schedule = newSchedule;
}

std::vector<semesterVecVertex> scheduleFit(directedGraphCourses& g, std::set<Vertex>* coursesTaken) {

    directedGraphCourses graphCopy = g;
    std::vector<std::vector<Vertex>> layers;
    
    removeCompletedCourses(baseSchedule, coursesTaken);

    std::vector<semesterVecVertex> schedule;
    for (size_t semesterIndex = 0; semesterIndex < baseSchedule.size(); ++semesterIndex) {
        const auto& semesterVertices = baseSchedule[semesterIndex];
        std::vector<Vertex> semesterCourses;
        uint8_t totalCredits = 0;
        uint16_t totalDifficulty = 0;
        
        // Convert vertices to nodes and calculate totals
        for (const auto& vertex : semesterVertices) {
            node& nodeData = boost::get(boost::vertex_name, g, vertex);
            semesterCourses.push_back(vertex);
            // Add up credits from sections (assuming first section's credits are representative)
            if (!nodeData.sections.empty()) {
                totalCredits += nodeData.sections[0].credits;
                totalDifficulty += nodeData.sections[0].difficulty;
            }
        }
        
        if (semesterIndex == 4) { // Check if we're at the fifth index (zero-based indexing)
            schedule.emplace_back(semesterCourses, totalCredits, totalDifficulty, 8);
            continue;
        }
        schedule.emplace_back(semesterCourses, totalCredits, totalDifficulty);
    }

    optimizeSchedule(schedule, g);

    return schedule;
}