#pragma once

#include "config.h"
#include "node.h"
#include "semesterVector.h"
#include "toposorts.h"

bool hasDependencies(directedGraphCourses& g, std::vector<node>& courses) {
    // Convert the vector of nodes to a set of vertex descriptors for fast lookup
    std::unordered_set<Vertex> courseVertices = getCourseVerticesSet(g, courses);

    // Iterate over the vertices in the set
    for (Vertex v : courseVertices) {
        InEdgeIterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::in_edges(v, g); ei != ei_end; ++ei) {
            Vertex parent = boost::source(*ei, g);
            // If the source of the incoming edge is also in the set, return true
            if (courseVertices.find(parent) != courseVertices.end()) {
                return true;
            }
        }
    }

    // No dependencies found
    return false;
}

bool has3Electives(std::vector<node>& courses){
    int electiveCount = 0;

    // Iterate through the vector of nodes
    for (const auto& course : courses) {
        // Check if the course number is 7777 (elective)
        if (course.CRS == 7777) {
            electiveCount++;
        }

        // If we already have 4 electives, we can return true early
        if (electiveCount >= 3) {
            return true;
        }
    }

    // If we finish the loop and don't have 4 electives, return false
    return false;
}

bool isSortValid(const std::vector<node>& sort, directedGraphCourses& g, uint8_t creditsPerSemester) {
    // Create a schedule from this topological sort
    std::vector<semesterVector> schedule;
    semesterVector currSemester(creditsPerSemester);

    for (const auto& course : sort) {
        currSemester.courses.push_back(course);
        currSemester.credits += course.sections[0].credits;  // assumes only one section for simplicity
        currSemester.difficulty += course.sections[0].credits;
        
        if (currSemester.credits >= creditsPerSemester) {
            schedule.push_back(currSemester);
            currSemester = semesterVector(creditsPerSemester);
        }
    }

    // If there are remaining courses, add the last semester
    if (!currSemester.courses.empty()) {
        schedule.push_back(currSemester);
    }

    // Validate and evaluate the schedule

    for (semesterVector semester : schedule) {
        if (hasDependencies(g, semester.courses) || has3Electives(semester.courses)) {
            return false;
            break;
        }
    }

    return true;
}


// Recursive function to explore all topological sorts
void allValidTopologicalSortsUtil(directedGraphCourses& g, std::vector<int>& in_degree, std::vector<Vertex>& result, 
                             std::vector<bool>& visited, std::vector<std::vector<node>>& all_sorts, uint8_t creditsPerSemester) {
    bool flag = false;


    VertexIterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        if (!visited[*vi] && in_degree[*vi] == 0) {
            // Mark as visited and add to the result
            visited[*vi] = true;
            result.push_back(*vi);

            // Reduce in-degree of adjacent vertices
            OutEdgeIterator ei, ei_end;
            for (boost::tie(ei, ei_end) = boost::out_edges(*vi, g); ei != ei_end; ++ei) {
                --in_degree[boost::target(*ei, g)];
            }

            // Recursively continue exploring
            allValidTopologicalSortsUtil(g, in_degree, result, visited, all_sorts, creditsPerSemester);

            // Backtrack: undo the changes
            visited[*vi] = false;
            result.pop_back();
            for (boost::tie(ei, ei_end) = boost::out_edges(*vi, g); ei != ei_end; ++ei) {
                ++in_degree[boost::target(*ei, g)];
            }

            flag = true;  // We found at least one vertex to visit
        }
    }

    // If no more vertices to process, we found one valid topological order
    if (!flag) {
        std::vector<node> current_sort;
        for (Vertex v : result) {
            //std::cout << v << " ";
            node& nodeData = boost::get(boost::vertex_name, g, v);
            current_sort.push_back(nodeData);  // Store the actual node in the current sort
        }
        if(isSortValid(current_sort, g, creditsPerSemester)){
            for(node n : current_sort){
                std::cout << std::to_string(n.getCRS()) << " ";
            }
            std::cout << std::endl;
            all_sorts.push_back(current_sort);  // Add the current sort to the collection of all sorts

        }
        //std::cout << std::endl;
    }
}

std::vector<semesterVector> binningBrute(directedGraphCourses& g, uint8_t creditsPerSemester, std::vector<node> genElectives, std::vector<node> labs) {
    std::vector<int> in_degree = computeInDegrees(g);
    std::vector<Vertex> result;
    std::vector<bool> visited(boost::num_vertices(g), false);
    std::vector<std::vector<node>> topoSorts;

    // Call the recursive utility function
    allValidTopologicalSortsUtil(g, in_degree, result, visited, topoSorts, creditsPerSemester);
    
    std::vector<std::vector<semesterVector>> allTopoSchedules;

    for(size_t i = 0; i < topoSorts.size(); i++){
        std::vector<semesterVector> schedule;
        semesterVector currSemester(creditsPerSemester);

        for(size_t j = 0; j < topoSorts[i].size(); j++){
            
            currSemester.courses.push_back(topoSorts[i][j]);
            currSemester.credits += topoSorts[i][j].sections[0].credits;   //assumes only one section for simplicity
            currSemester.difficulty += topoSorts[i][j].sections[0].credits;
            
            if(currSemester.credits >= creditsPerSemester){
                schedule.push_back(currSemester);
                currSemester = semesterVector(creditsPerSemester);
            }


        }
        
        allTopoSchedules.push_back(schedule);
    }
    

    //So now we have all schedules in allTopoSchedules, we can now run the true algorithm:

    //can make pair for this later;
    std::vector<float> scheduleComplexities (allTopoSchedules.size());

    for(size_t i = 0; i < allTopoSchedules.size(); i++){

        scheduleComplexities[i] = getComplexity(&allTopoSchedules[i]);
    }

    
    int minIndex = -1;         // To store the index of the minimum non-zero value
    float minValue = FLT_MAX; // Start with a very large number
    
    // Iterate through validSchedules and scheduleComplexities
    for (size_t i = 0; i < scheduleComplexities.size(); ++i) {
        if(scheduleComplexities[i] < minValue){
            minValue = scheduleComplexities[i];
            minIndex = i;
        }
    }

    if(minIndex < 0){
        std::cerr << "failed to find easiest schedule";
        exit(EXIT_FAILURE);
    }

    return allTopoSchedules[minIndex];

}