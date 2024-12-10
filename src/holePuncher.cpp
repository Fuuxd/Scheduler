#pragma once

#include "config.h"
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

bool isCandidate(directedGraphCourses& g, std::vector<semesterVecVertex>* schedule, size_t vertexIndex, 
                 size_t currIndex, size_t holeIndex, int extraCredLimit = 0) {
    
    // Ensure that the holeIndex is earlier than the currIndex
    if (currIndex <= holeIndex) {
        std::cout << "Logic Error in passed values to isCandidate. Curr:" << currIndex << " HI:" << holeIndex;
        return false; 
    }

    // Check semesters between holeIndex and currIndex for prerequisites
    for (size_t i = holeIndex; i < currIndex; i++) {
        for (auto v : (*schedule)[i].courses) {
            // Check if there is an outgoing edge from any course in the intermediate semester to vertexIndex
            if (boost::edge(v, vertexIndex, g).second) {
                //std::cout << "Prerequisite edge from course " << v << " to " << vertexIndex << " found. Cannot move course." << std::endl;
                return false; // Found an edge, so it's not a valid move
            }
        }
    }

    // Check credit limits for the move
    for (size_t i = currIndex; i > holeIndex; i--) {
        for (auto v : (*schedule)[i].courses) {
            node& nodeData = boost::get(boost::vertex_name, g, v);
            if (nodeData.getSection().credits + (*schedule)[holeIndex].credits > 15 + extraCredLimit) {
                return false; // Too many credits
            }
            if(nodeData.CRS == 4250 && (holeIndex-currIndex) == 1){
                return false; //Edge case for ethics course to be taken last semester
            }
        }
    }

    return true; // If no prerequisite edges and credit limits are fine
}

bool isInputValid(directedGraphCourses& G, std::set<Vertex>* coursesTaken) {
    // Check for null pointer
    if (!coursesTaken) {
        return false;
    }

    // Iterate through each course in coursesTaken
    for (const auto& course : *coursesTaken) {
        // Get incoming edges to this vertex
        boost::graph_traits<directedGraphCourses>::in_edge_iterator ei, ei_end;
        
        // Iterate through all incoming edges
        for (boost::tie(ei, ei_end) = boost::in_edges(course, G); ei != ei_end; ++ei) {
            // Get the source vertex of the incoming edge (the prerequisite course)
            Vertex prerequisite = boost::source(*ei, G);
            
            // Check if the prerequisite is not in coursesTaken
            if (coursesTaken->find(prerequisite) == coursesTaken->end()) {
                // Prerequisite is missing from coursesTaken
                return false;
            }
        }
    }

    // All prerequisites are present
    return true;
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
                    if (isCandidate(g, schedule, futureVertex, futureSemester, currentSemester)) {
                        // Get node data for the course
                        node& nodeData = boost::get(boost::vertex_name, g, futureVertex);
                        
                        // Calculate a scoring metric for the course
                        float score = ((futureSemester - 1) * 0.7+ 
                        std::abs(static_cast<float>((((*schedule)[futureSemester].difficulty + nodeData.getSection().difficulty) / 
                        ((*schedule)[futureSemester].courses.size() + 1)) - 3)));
                        
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

    if(!isInputValid(g, coursesTaken)){
        std::cout << "Invalid Schedule. See an advisor";
        exit(0);
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
