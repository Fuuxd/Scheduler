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
#include <chrono>

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

// Function to calculate the average of a vector
double calculateAverage(const std::vector<double>& values) {
    double total = 0;
    for (double value : values) {
        total += value;
    }
    return values.empty() ? 0 : total / values.size();
}

// Function to get current memory usage in KB
size_t getMemoryUsageKB() {
    std::ifstream statusFile("/proc/self/status");
    std::string line;
    size_t memoryUsage = 0;

    while (std::getline(statusFile, line)) {
        if (line.find("VmRSS:") == 0) { // Look for "VmRSS"
            std::istringstream iss(line);
            std::string key, unit;
            iss >> key >> memoryUsage >> unit; // Extract memory usage in KB
            break;
        }
    }

    return memoryUsage; // Return memory usage in KB
}

int main() {
    hardcodeGraph();

    size_t totalVertices = boost::num_vertices(G);
    size_t maxVerticesInSet = 10;
    size_t maxCombinations = 100000;

    std::ofstream csvFile("schedule_combinations.csv");
    std::ofstream timingFile("average_times.csv");

    csvFile << "Vertices Fit:,Option0_Size,Diff,Option1_Size,Diff,Option2_Size,Diff,Option3_Size,DIff, Mov: Opt0,Diff, Opt1,Diff, Opt2,Diff, Opt3, Diff\n";
    timingFile << "Set_Size,Function,Average_Time_ms,Average_Memory_KB\n";

    size_t totalCombinations = 1ULL << totalVertices;
    std::cout << "Total possible combinations: " << totalCombinations 
              << ". Processing max " << maxCombinations << " combinations.\n";

    std::map<size_t, std::vector<double>> fitTimes, movTimes;
    std::map<size_t, std::vector<double>> fitMemory, movMemory;

    size_t processedCombinations = 0;
    for (size_t number = 1; number < totalCombinations && processedCombinations < maxCombinations; ++number) {
        std::set<Vertex> currentSet = numberToVertexSet(number, totalVertices, maxVerticesInSet);
        if (currentSet.empty()) continue;

        size_t setSize = currentSet.size();
        csvFile << "\"" << setToCSVString(currentSet) << "\"";

        for (int option = 0; option < 4; ++option) {
            // Measure scheduleFit execution time and memory
            auto startFit = std::chrono::high_resolution_clock::now();
            size_t memBeforeFit = getMemoryUsageKB();
            std::vector<semesterVecVertex> fittedSchedule = scheduleFit(G, const_cast<std::set<Vertex>*>(&currentSet), option);
            size_t memAfterFit = getMemoryUsageKB();
            auto endFit = std::chrono::high_resolution_clock::now();

            double fitTime = std::chrono::duration<double, std::milli>(endFit - startFit).count();
            fitTimes[setSize].push_back(fitTime);
            fitMemory[setSize].push_back(static_cast<double>(memAfterFit - memBeforeFit));

            csvFile << "," << fittedSchedule.size() << "," << (getAverageDifficulty(fittedSchedule));

            // Measure scheduleMov execution time and memory
            auto startMov = std::chrono::high_resolution_clock::now();
            size_t memBeforeMov = getMemoryUsageKB();
            fittedSchedule = scheduleMov(G, const_cast<std::set<Vertex>*>(&currentSet), option);
            size_t memAfterMov = getMemoryUsageKB();
            auto endMov = std::chrono::high_resolution_clock::now();

            double movTime = std::chrono::duration<double, std::milli>(endMov - startMov).count();
            movTimes[setSize].push_back(movTime);
            movMemory[setSize].push_back(static_cast<double>(memAfterMov - memBeforeMov));

            csvFile << "," << fittedSchedule.size() << "," << (getAverageDifficulty(fittedSchedule));
        }

        csvFile << "\n";
        processedCombinations++;

        if (processedCombinations % 10000 == 0) {
            std::cout << "Processed " << processedCombinations << " combinations...\n";
        }
    }

    csvFile.close();
    std::cout << "CSV output written to schedule_combinations.csv\n";

    // Write timing and memory results to the second CSV file
    for (const auto& [setSize, times] : fitTimes) {
        double averageFitTime = calculateAverage(times);
        double averageFitMemory = calculateAverage(fitMemory[setSize]);
        timingFile << setSize << ",scheduleFit," << averageFitTime << "," << averageFitMemory << "\n";
    }

    for (const auto& [setSize, times] : movTimes) {
        double averageMovTime = calculateAverage(times);
        double averageMovMemory = calculateAverage(movMemory[setSize]);
        timingFile << setSize << ",scheduleMov," << averageMovTime << "," << averageMovMemory << "\n";
    }

    timingFile.close();
    std::cout << "Timing and memory data written to average_times.csv\n";
    std::cout << "Total combinations processed: " << processedCombinations << std::endl;

    return 0;
}