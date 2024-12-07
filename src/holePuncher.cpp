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

void removeCompletedCourses(const std::set<Vertex>* coursesTaken, std::vector<semesterVecVertex> *schedule, bool isTakingSummer, int summerIndex, int extraCredLimit){
    // Check for null pointer
    if (!schedule || !coursesTaken) {
        std::cout << "Empty Schedule or coursesTaken";
        return;
    }

    if (summerIndex < 0 || summerIndex >= schedule->size()) {
        std::cerr << "Invalid summerIndex.";
        return;
    }

    // Iterate over the schedule to remove completed courses
    for (auto& semester : *schedule) {
        auto it = semester.courses.begin();
        while (it != semester.courses.end()) {
            // Check if the course is in the set of completed courses
            if (coursesTaken->count(*it)) {
                node& nodeData = boost::get(boost::vertex_name, G, (*it));
                semester.credits -= nodeData.getSection().credits;
                semester.difficulty -= nodeData.getSection().difficulty;
                it = semester.courses.erase(it); // Remove course and update iterator
                
            } else {
                ++it; // Advance iterator
            }
        }
    }

    // Remove empty semesters from the schedule
    size_t index = 0;
    schedule->erase(
        std::remove_if(
            schedule->begin(), 
            schedule->end(), 
            [&isTakingSummer](const semesterVecVertex& semester) mutable {
                return semester.courses.empty() && !(isTakingSummer && semester.isSummer);
            }
        ), 
        schedule->end()
    );

    if(isTakingSummer && !(*schedule)[summerIndex].isSummer){// handle case where out of range
        for(auto& sem : *schedule){
            if(sem.isSummer){
                sem.isSummer = false;
                sem.maxCredits = 15 + extraCredLimit;
            }
        }
        if((*schedule)[summerIndex].credits <= 9){
            (*schedule)[summerIndex].isSummer = true;
            (*schedule)[summerIndex].maxCredits = 9;
        }else{
            (*schedule).insert((*schedule).begin() + summerIndex, {});
            (*schedule)[summerIndex].isSummer = true;
            (*schedule)[summerIndex].maxCredits = 9;
        }
    }

    if(!isTakingSummer){
        for(auto& sem : *schedule){
            if(sem.isSummer){
                sem.isSummer = false;
                sem.maxCredits = 15 + extraCredLimit;
            }
        }
    }

    return;
    
}

//Works on baseSchedule rather than a given schedule
//void removeCompletedCourses(const std::set<Vertex>* coursesTaken) {
//    // Check for null pointer
//    if (!coursesTaken) {
//        return;
//    }
//
//    // For each semester in the schedule
//    for(auto& bin : baseSchedule) {  // Use reference to modify original vector
//        // Iterate through the courses in the bin
//        auto it = bin.begin();
//        while (it != bin.end()) {
//            // Simply check if the vertex index exists in coursesTaken
//            auto found = coursesTaken->find(*it);
//            
//            if (found != coursesTaken->end()) {
//                // Remove the completed course from the bin
//                it = bin.erase(it);
//            } else {
//                ++it;
//            }
//        }
//    }
//
//    // Remove any empty semesters
//    //baseSchedule.erase(
//    //    std::remove_if(
//    //        baseSchedule.begin(),
//    //        baseSchedule.end(),
//    //        [](const std::vector<Vertex>& semester) {
//    //            return semester.empty();
//    //        }
//    //    ),
//    //    baseSchedule.end()
//    //);
//}

bool optimizeSchedule(std::vector<semesterVecVertex>& schedule, directedGraphCourses& G) {
    if (schedule.size() <= 1) {
        return false;  // Nothing to optimize
    }

    bool didSomething = false;

    // Iterate through semesters from last to second
    for (int currentSemIndex = schedule.size() - 1; currentSemIndex > 0; --currentSemIndex) {
        auto& currentSemester = schedule[currentSemIndex];
        auto& previousSemester = schedule[currentSemIndex - 1];
        
        // Create a vector to mark vertices that should stay in current semester
        std::vector<bool> shouldStay(currentSemester.courses.size(), false);
        std::vector<uint8_t> courseCredits(currentSemester.courses.size());

        // For each vertex in current semester
        for (size_t i = 0; i < currentSemester.courses.size(); ++i) {
            const auto& currentVertex = currentSemester.courses[i];
            
            // Get course credits
            node& currentNodeData = boost::get(boost::vertex_name, G, currentVertex);
            courseCredits[i] = currentNodeData.getSection().credits;
            
            // Check if this vertex depends on any vertex in the previous semester
            for (const auto& prevVertex : previousSemester.courses) {
                // Check outgoing edges from previous semester vertex to current vertex
                boost::graph_traits<directedGraphCourses>::out_edge_iterator eo, eo_end;
                for (boost::tie(eo, eo_end) = boost::out_edges(prevVertex, G); eo != eo_end; ++eo) {
                    if (boost::target(*eo, G) == currentVertex) {
                        shouldStay[i] = true;
                        break;
                    }
                }
                if (shouldStay[i]) break;

                // Also check if any vertex in previous semester depends on this vertex
                boost::graph_traits<directedGraphCourses>::in_edge_iterator ei, ei_end;
                for (boost::tie(ei, ei_end) = boost::in_edges(prevVertex, G); ei != ei_end; ++ei) {
                    if (boost::source(*ei, G) == currentVertex) {
                        shouldStay[i] = true;
                        break;
                    }
                }
                if (shouldStay[i]) break;

                if( (previousSemester.credits + currentNodeData.getSection().credits) > previousSemester.maxCredits){
                    shouldStay[i] = true;
                }
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
                    didSomething = true;  // Mark that we moved a course
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
    for (const auto& semester : schedule) {
        if (!semester.courses.empty()) {
            newSchedule.push_back(semester);
        }
    }

    // Update the schedule
    schedule = newSchedule;

    return didSomething;
}

std::vector<semesterVecVertex> minimizeScheduleHeight(std::vector<semesterVecVertex>& initialSchedule, directedGraphCourses& G) {
    std::vector<semesterVecVertex> bestSchedule = initialSchedule;
    bool madeChanges;
    
    do {
        madeChanges = false;
        
        // Try to optimize the current schedule
        madeChanges = optimizeSchedule(bestSchedule, G);
        
        // If no changes were made by optimizeSchedule, try to compress further
        if (!madeChanges) {
            // Check each semester from bottom to top if courses can move up multiple semesters
            for (int currentSemIndex = bestSchedule.size() - 1; currentSemIndex > 0; --currentSemIndex) {
                auto& currentSem = bestSchedule[currentSemIndex];
                
                // Try to move each course up as far as possible
                auto courseIt = currentSem.courses.begin();
                while (courseIt != currentSem.courses.end()) {
                    const auto& currentCourse = *courseIt;
                    node& courseNode = boost::get(boost::vertex_name, G, currentCourse);
                    
                    // Skip if course number is 4250
                    if (courseNode.getCRS() == 4250) {
                        ++courseIt;
                        continue;
                    }
                    
                    // Find the highest semester this course can move to
                    int targetSemester = -1;
                    for (int testSem = 0; testSem < currentSemIndex; ++testSem) {
                        bool canMoveTo = true;
                        
                        // Check prerequisites
                        for (int checkSem = testSem; checkSem < currentSemIndex; ++checkSem) {
                            for (const auto& checkCourse : bestSchedule[checkSem].courses) {
                                // Check if current course depends on any course in between
                                boost::graph_traits<directedGraphCourses>::out_edge_iterator eo, eo_end;
                                for (boost::tie(eo, eo_end) = boost::out_edges(checkCourse, G); eo != eo_end; ++eo) {
                                    if (boost::target(*eo, G) == currentCourse) {
                                        canMoveTo = false;
                                        break;
                                    }
                                }
                                
                                // Check if any course depends on current course
                                boost::graph_traits<directedGraphCourses>::in_edge_iterator ei, ei_end;
                                for (boost::tie(ei, ei_end) = boost::in_edges(checkCourse, G); ei != ei_end; ++ei) {
                                    if (boost::source(*ei, G) == currentCourse) {
                                        canMoveTo = false;
                                        break;
                                    }
                                }
                            }
                            if (!canMoveTo) break;
                        }
                        
                        // Check credit limit
                        uint8_t courseCredits = courseNode.getSection().credits;
                        if (bestSchedule[testSem].credits + courseCredits > bestSchedule[testSem].maxCredits) {
                            canMoveTo = false;
                        }
                        
                        if (canMoveTo) {
                            targetSemester = testSem;
                            break;
                        }
                    }
                    
                    // If we found a valid target semester, move the course
                    if (targetSemester != -1) {
                        uint8_t courseCredits = courseNode.getSection().credits;
                        uint16_t courseDifficulty = courseNode.getSection().difficulty;
                        
                        // Add to target semester
                        bestSchedule[targetSemester].courses.push_back(currentCourse);
                        bestSchedule[targetSemester].credits += courseCredits;
                        bestSchedule[targetSemester].difficulty += courseDifficulty;
                        
                        // Remove from current semester
                        currentSem.credits -= courseCredits;
                        currentSem.difficulty -= courseDifficulty;
                        courseIt = currentSem.courses.erase(courseIt);
                        
                        madeChanges = true;
                    } else {
                        ++courseIt;
                    }
                }
            }
        }
        
        // Remove empty semesters
        bestSchedule.erase(
            std::remove_if(
                bestSchedule.begin(),
                bestSchedule.end(),
                [](const semesterVecVertex& semester) { return semester.courses.empty() && !(semester.isSummer); }
            ),
            bestSchedule.end()
        );
        
    } while (madeChanges);
    
    return bestSchedule;
}

bool isCandidate(directedGraphCourses& g, std::vector<semesterVecVertex>* schedule, size_t vertexIndex, 
                 size_t currIndex, size_t holeIndex, int extraCredLimit = 0) {
    if (currIndex > holeIndex) {
        std::cout << "Logic Error in passed values to isCandidate. Curr:" << currIndex << " HI:" << holeIndex;
        return false; // Added return false for this error case
    }

    for (size_t i = currIndex; i > holeIndex; i--) {
        for (auto v : (*schedule)[i].courses) {
            // Fixed vertex comparison and edge checking
            if (boost::edge(v, vertexIndex, g).second) {
                return false;
            }

            node& nodeData = boost::get(boost::vertex_name, g, v);
            if (nodeData.getSection().credits + (*schedule)[holeIndex].credits > 15 + extraCredLimit) {
                return false;
            }
        }
    }

    return true; // Added return true if all checks pass
}

std::vector<semesterVecVertex> fillSched(directedGraphCourses& g, std::vector<semesterVecVertex>* schedule) {
    // Iterate through semesters up to the second-to-last semester
    for (size_t currentSemester = 0; currentSemester < schedule->size() - 1; currentSemester++) {
        // Only process if current semester has room for more credits
        if ((*schedule)[currentSemester].credits <= ((*schedule)[currentSemester].maxCredits - 3)) {
            std::vector<std::pair<size_t, float>> marked;

            // Look ahead through future semesters
            for (size_t futureSemester = currentSemester + 1; futureSemester < schedule->size(); futureSemester++) {
                // Check each course in future semesters
                for (auto futureVertex : (*schedule)[futureSemester].courses) {
                    // Check if this course is a candidate to move
                    if (isCandidate(g, schedule, futureVertex, currentSemester, futureSemester)) {
                        // Get node data for the course
                        node& nodeData = boost::get(boost::vertex_name, g, futureVertex);
                        
                        // Calculate a scoring metric for the course
                        float score = ((futureSemester - 1)*2 + 
                            (((*schedule)[futureSemester].difficulty + nodeData.getSection().difficulty) / 
                            ((*schedule)[futureSemester].courses.size() + 1)) - 3);
                        
                        marked.emplace_back(futureVertex, score);
                    }
                }
            }

            // Add courses to current semester until credit limit is reached
            while (!marked.empty() && 
                   (*schedule)[currentSemester].credits <= ((*schedule)[currentSemester].maxCredits - 3)) {
                // Find the best course to move
                auto bestIt = std::min_element(marked.begin(), marked.end(),
                    [](const auto& a, const auto& b) { return a.second < b.second; });
                
                size_t bestVertex = bestIt->first;

                // Remove this vertex from all other semesters
                for (size_t otherSemester = 0; otherSemester < schedule->size(); otherSemester++) {
                    if (otherSemester != currentSemester) {
                        auto& semesterCourses = (*schedule)[otherSemester].courses;
                        for (auto it = semesterCourses.begin(); it != semesterCourses.end(); ) {
                            if (*it == bestVertex) {
                                node& nodeData = boost::get(boost::vertex_name, g, *it);

                                (*schedule)[otherSemester].credits -= nodeData.getSection().credits;
                                (*schedule)[otherSemester].difficulty -= nodeData.getSection().difficulty;   

                                it = semesterCourses.erase(it);
                                break;  // Exit inner loop once vertex is found and removed
                            } else {
                                ++it;
                            }
                        }
                    }
                }

                // Add the course to the current semester
                node& nodeData = boost::get(boost::vertex_name, g, bestVertex);
                (*schedule)[currentSemester].courses.emplace_back(bestVertex);
                (*schedule)[currentSemester].credits += nodeData.getSection().credits;
                (*schedule)[currentSemester].difficulty += nodeData.getSection().difficulty;

                // Remove this course from consideration
                marked.erase(bestIt);
            }
        }
    }

    return *schedule;
}

std::vector<semesterVecVertex> scheduleFit(directedGraphCourses& g, std::set<Vertex>* coursesTaken, int extraCredLimit = 0, bool isTakingSummer = true, uint summerIndex = 1) {

    directedGraphCourses graphCopy = g;
    std::vector<std::vector<Vertex>> layers;

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
            schedule.emplace_back(semesterCourses, totalCredits, totalDifficulty, 9);
            schedule.back().isSummer = true;
            continue;
        }
        schedule.emplace_back(semesterCourses, totalCredits, totalDifficulty, 15+extraCredLimit);
    }

    

    //If removed summer (what removeCompletedCourses returns), and is taking summer add a summer semester at index specified.
    removeCompletedCourses(coursesTaken, &schedule, isTakingSummer, summerIndex, extraCredLimit);

    //minimizeScheduleHeight(schedule, g)

    //while(optimizeSchedule(schedule, g)){
    //    ;
    //}

    //fillSched(g, &schedule)

    schedule = fillSched(g, &schedule);

    auto it = std::remove_if(schedule.begin(), schedule.end(), 
        [](const auto& vec) { return vec.courses.empty(); });
    schedule.erase(it, schedule.end());

    return schedule;
}

std::vector<semesterVecVertex> scheduleMov(directedGraphCourses& g, std::set<Vertex>* coursesTaken, int extraCredLimit = 0, bool isTakingSummer = true, uint summerIndex = 1) {

    directedGraphCourses graphCopy = g;
    std::vector<std::vector<Vertex>> layers;

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
            schedule.emplace_back(semesterCourses, totalCredits, totalDifficulty, 9);
            schedule.back().isSummer = true;
            continue;
        }
        schedule.emplace_back(semesterCourses, totalCredits, totalDifficulty, 15+extraCredLimit);
    }

    

    //If removed summer (what removeCompletedCourses returns), and is taking summer add a summer semester at index specified.
    removeCompletedCourses(coursesTaken, &schedule, isTakingSummer, summerIndex, extraCredLimit);

    //minimizeScheduleHeight(schedule, g)

    //while(optimizeSchedule(schedule, g)){
    //    ;
    //}

    //fillSched(g, &schedule)

    return minimizeScheduleHeight(schedule, g);
}

bool isFullTime(directedGraphCourses& g, std::vector<semesterVecVertex> *schedule){
    
    for(size_t i = 0; i < schedule->size(); i++){
        int sum = 0;
        for(size_t j = 0; j < (*schedule)[i].courses.size(); j++)
        {
            if( (*schedule)[i].isSummer ){
                continue;
            }
            node& nodeData = boost::get(boost::vertex_name, g, (*schedule)[i].courses[j]);
            sum += nodeData.getSection().credits;
        }
        if(sum < 12){
            return false;
        }
    }
    return true;
}
