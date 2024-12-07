#include "config.h"
#include "node.h"
#include "semesterVector.h"
#include "holePuncher.cpp"

#include "hardcodeGraph.cpp"
#include "numberOfTopo.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <cmath>

// Function to convert a number to a set of vertices
std::set<Vertex> numberToVertexSet(size_t number, size_t totalVertices, size_t maxVerticesInSet) {
    std::set<Vertex> currentSet;
    
    for (size_t j = 0; j < totalVertices; ++j) {
        if (number & (1ULL << j)) {
            currentSet.insert(j);
            
            // Stop if we've reached max vertices
            if (currentSet.size() >= maxVerticesInSet) {
                break;
            }
        }
    }

    return currentSet;
}

// Function to convert set to comma-separated string
std::string setToCSVString(const std::set<Vertex>& vertexSet) {
    std::stringstream ss;
    for (auto it = vertexSet.begin(); it != vertexSet.end(); ) {
        ss << *it;
        if (++it != vertexSet.end()) ss << ",";
    }
    return ss.str();
}

int main() {
    hardcodeGraph();

    // Total number of vertices in the graph
    size_t totalVertices = boost::num_vertices(G);
    
    // Maximum number of vertices to consider in a set
    size_t maxVerticesInSet = 10;  // Adjust this value as needed
    
    // Maximum number of combinations to process
    size_t maxCombinations = 100000;  // Adjust as needed

    // Open CSV file for writing
    std::ofstream csvFile("schedule_combinations.csv");
    
    // Write CSV header
    csvFile << "Vertices Fit:,Option0_Size,Diff,Option1_Size,Diff,Option2_Size,Diff,Option3_Size,DIff, Mov: Opt0,Diff, Opt1,Diff, Opt2,Diff, Opt3, Diff\n";

    // Total possible combinations
    size_t totalCombinations = 1ULL << totalVertices;
    
    std::cout << "Total possible combinations: " << totalCombinations 
              << ". Processing max " << maxCombinations << " combinations.\n";

    // Iterate through combinations without storing them in memory
    size_t processedCombinations = 0;
    for (size_t number = 1; number < totalCombinations && processedCombinations < maxCombinations; ++number) {
        // Convert number to vertex set
        std::set<Vertex> currentSet = numberToVertexSet(number, totalVertices, maxVerticesInSet);

        // Skip empty sets
        if (currentSet.empty()) continue;

        // Write vertex set to first column
        csvFile << "\"" << setToCSVString(currentSet) << "\"";

        // Run scheduleFit for multiple options and write sizes
        for (int option = 0; option < 4; ++option) {
            std::vector<semesterVecVertex> fittedSchedule = scheduleFit(G, const_cast<std::set<Vertex>*>(&currentSet), option);
            
            csvFile << "," << fittedSchedule.size()<< "," << (getAverageDifficulty(fittedSchedule));


            fittedSchedule = scheduleMov(G, const_cast<std::set<Vertex>*>(&currentSet), option);

            csvFile << "," << fittedSchedule.size()<< "," << (getAverageDifficulty(fittedSchedule));
        }
        
        // End of row
        csvFile << "\n";

        processedCombinations++;

        // Optional: Print progress
        if (processedCombinations % 10000 == 0) {
            std::cout << "Processed " << processedCombinations << " combinations...\n";
        }

        if(processedCombinations >= maxCombinations){
            std::cout << "GOTTEM";
            break;
        }


    }

    csvFile.close();
    std::cout << "CSV output written to schedule_combinations.csv\n";
    std::cout << "Total combinations processed: " << processedCombinations << std::endl;

    return 0;
}