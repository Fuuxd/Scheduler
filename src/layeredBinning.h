/**
 * @file layeredBinning.h
 * 
 * @details
 * This file deals with a heuristic-like method that does a type of topological sorting to generate a schedule.
 * 
 *  THE USE OF ANY LAYERED BINNING THAT INCLUDES THE USE OF layeredTopoSort() from toposorts.h IS DESCTRUCTIVE TO THE GRAPH
 * 
 * 
 * @author German Becerra
 * 
 * Usage:
 * @code
 * 
 *
 * 
 * 
 * 
 *
 * 
 * 
 * 
 * 
 * 
 *
 * @endcode
 */

#pragma once

#include "config.h"
#include "toposorts.h"
#include "semesterVector.h"

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

// CHECK CHATGPT GENERATED CODE IN layeredAIGEN.cpp

node* findLab(const node& course, std::vector<node>& labs) {
    // Iterate through the labs vector
    for (auto& labNode : labs) {
        // Match CRS and ensure lab flag is true by comparing course name + " Lab"
        if (course.getName().append(" Lab") == labNode.getName()) {
            return &labNode; // Return reference (as a pointer) to the matching lab node
        }
    }
    return nullptr; // Return nullptr if no match found
}

std::vector<semesterVector> layeredBinning(directedGraphCourses& g, uint8_t creditsPerSemester, std::vector<node>& genElectives_, std::vector<node>& labs, std::vector<std::vector<node>> &layeredTopo_) {

    std::vector<std::vector<node>> layeredTopo = layeredTopo_;

    std::vector<node> genElectives = genElectives_;
    //debug prints
    //std::cout << "Layered Topological Sort Output:" << std::endl;
    //for (size_t i = 0; i < layeredTopo.size(); ++i) {
    //    std::cout << "Layer " << i + 1 << ":" << std::endl;
    //    for (const node& n : layeredTopo[i]) {
    //        // Print CRS and Name of each node in the layer
    //        std::cout << n.getCRS() << " " << n.getName() << "    ";
    //    }
    //    std::cout << std::endl;
    //}

    //std::cout << std::endl << std::endl;

    uint16_t pushCounter = 0; // Counter for tracking course pushes back into the layer
    const uint16_t maxPushes = 1000; // Maximum allowed pushes

    std::vector<semesterVector> binnedSchedule;

    semesterVector currSemester(creditsPerSemester);

    uint8_t totalCredits = 0;
    // Iterate over layers
    for (size_t i = 0; i < layeredTopo.size(); i++) {

        // Iterate over nodes in the current layer
        for (size_t j = 0; j < layeredTopo[i].size(); j++) {
            
            //std::cout << "Depth 3. adding to currSem" << std::endl;

            // If non-elective credits are met, fill with genElectives
            if (currSemester.credits >= currSemester.nonElectiveCredits) { //if important credits full or semester too difficult already, fine tune later
                while (!genElectives.empty() && currSemester.credits < creditsPerSemester) {

                    //std::cout << "Depth 4. filling with electives" << std::endl;

                    currSemester.courses.push_back(genElectives.back());
                    currSemester.credits += genElectives.back().sections[0].credits;  // Assuming one section for simplicity
                    totalCredits += genElectives.back().sections[0].credits; 
                    currSemester.difficulty += genElectives.back().sections[0].difficulty;  // Assuming one section for simplicity
                    genElectives.pop_back();
                }
            }

            // Check if semester is full
            if (currSemester.credits >= creditsPerSemester) {
                binnedSchedule.push_back(currSemester);
                currSemester = semesterVector(creditsPerSemester);
            }

            

            
            if(layeredTopo[i][j].credsNeeded > totalCredits){
                //std::cout << "pushing further layeredTopo[" << i << "][" << j << "]." << layeredTopo[i][j].getName() << std::endl;
                
                if (pushCounter >= maxPushes) { // prevent infinite loop
                    std::cerr << "Failed to handle classes with credits Requirement: Too many pushes" << std::endl;
                    return binnedSchedule; // Return the current schedule
                }
                
                if(i < layeredTopo.size() - 1){
                    //add current to next layer
                    layeredTopo[i+1].push_back(layeredTopo[i][j]);
                    pushCounter++;
                    continue;

                } else if (j < layeredTopo[i].size() - 1){
                    layeredTopo[i].push_back(layeredTopo[i][j]);
                    pushCounter++;
                    continue;
                }else{
                    //std::cerr << "Failed to handle class with credits Requirement";
                }
            }

            // Add the current node from layeredTopo to the semester

            if( currSemester.credits + layeredTopo[i][j].sections[0].credits <= creditsPerSemester ){
                currSemester.courses.push_back(layeredTopo[i][j]);
                totalCredits += layeredTopo[i][j].sections[0].credits;

                if (!layeredTopo[i][j].sections.empty()) {
                    // Add the credits from the first section
                    currSemester.credits += layeredTopo[i][j].sections[0].credits;
                    currSemester.difficulty += layeredTopo[i][j].sections[0].difficulty;
                } else {
                    std::cerr << "No sections available for node in layeredTopo[" << i << "][" << j << "]." << std::endl;
                }

                if(layeredTopo[i][j].hasLab){
                    node* labFound = findLab(layeredTopo[i][j], labs);
                    if(labFound){
                        currSemester.courses.push_back(*labFound);
                        currSemester.credits += labFound->sections[0].credits;
                    }else{
                        std::cout<< "NO lab found for" << layeredTopo[i][j].getName();
                    }
                }
            }

            else {
                binnedSchedule.push_back(currSemester);
                currSemester = semesterVector(creditsPerSemester);
                currSemester.courses.push_back(layeredTopo[i][j]);
            }




            

        }

        //If we get to the end of layer but currSemester not full, fill with electives

        if(currSemester.credits < creditsPerSemester && creditsPerSemester-1 != currSemester.credits){
            while (!genElectives.empty() && currSemester.credits < creditsPerSemester) {

                //std::cout << "Depth 4. filling with electives" << std::endl;

                currSemester.courses.push_back(genElectives.back());
                currSemester.credits += genElectives.back().sections[0].credits;  // Assuming one section for simplicity
                totalCredits += genElectives.back().sections[0].credits; 
                currSemester.difficulty += genElectives.back().sections[0].difficulty;  // Assuming one section for simplicity
                genElectives.pop_back();
            }
        }

    }

    // Add final semester left to schedule
        

    binnedSchedule.push_back(currSemester);
    

    return binnedSchedule;
}

std::vector<semesterVector> layeredBinningFirst(directedGraphCourses& g, uint8_t creditsPerSemester, std::vector<node>& genElectives, std::vector<node>& labs, std::set<Vertex> coursesTaken) {
    std::vector<std::vector<node>> layeredTopo = layeredTopoSort(g, coursesTaken);

    return layeredBinning(g, creditsPerSemester, genElectives, labs, layeredTopo);
}

bool nextPermutationIndices(std::vector<std::vector<size_t>>& indices, const std::vector<std::vector<node>>& vecOfVecs) {
    for (size_t i = 0; i < vecOfVecs.size(); ++i) {
        if (std::next_permutation(indices[i].begin(), indices[i].end())) {
            return true;
        } else {
            std::sort(indices[i].begin(), indices[i].end());
        }
    }
    return false;
}

bool nextPermutationIndicesBackwards(std::vector<std::vector<size_t>>& indices, const std::vector<std::vector<node>>& vecOfVecs) {
    for (size_t i = vecOfVecs.size(); i-- > 0;) {
        if (std::next_permutation(indices[i].begin(), indices[i].end())) {
            return true;
        } else {
            std::sort(indices[i].begin(), indices[i].end());
        }
    }
    return false;
}

size_t factorial(size_t n) {
    size_t result = 1;
    for (size_t i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

template <typename T>
std::vector<T> getPermutation(const std::vector<T>& vec, size_t index) {
    std::vector<T> elements = vec;
    std::vector<T> result;
    size_t n = elements.size();
    
    // Generate permutation based on index
    while (n > 0) {
        size_t fact = factorial(n - 1);
        size_t i = index / fact;
        result.push_back(elements[i]);
        elements.erase(elements.begin() + i);  // Remove the used element
        index %= fact;  // Reduce index for the next iteration
        --n;
    }
    
    return result;
}

template <typename T>
std::vector<std::vector<T>> generatePermutation(const std::vector<std::vector<T>>& vecOfVecs, size_t index) {
    std::vector<std::vector<T>> result;
    size_t totalPermutations = 1;
    
    // First, calculate the total number of permutations possible
    for (const auto& vec : vecOfVecs) {
        totalPermutations *= factorial(vec.size());
    }
    
    // Ensure the index is within bounds
    if (index >= totalPermutations) {
        throw std::out_of_range("Index out of range for permutations");
    }
    
    // Now, calculate the permutations for each inner vector
    size_t partialPermutations = totalPermutations;
    for (const auto& vec : vecOfVecs) {
        size_t numPermsForVec = factorial(vec.size());
        partialPermutations /= numPermsForVec;
        size_t vecIndex = index / partialPermutations;  // Get which permutation for this vector
        result.push_back(getPermutation(vec, vecIndex));  // Get the specific permutation
        index %= partialPermutations;  // Reduce index for the next vectors
    }
    
    return result;
}


std::vector<semesterVector> layeredBinningPermute(directedGraphCourses& g, uint8_t creditsPerSemester, std::vector<node>& genElectives, std::vector<node>& labs, std::set<Vertex> coursesTaken, uint iterationLimit = 100000){

    std::vector<float> complexities;
    std::vector<semesterVector> schedule;

    std::vector<std::vector<node>> topoLayers = layeredTopoSort(g, coursesTaken);

    //std::cout << "permuting Layered toposort out:" << std::endl;
    //for (size_t i = 0; i < topoLayers.size(); ++i) {
    //    std::cout << "Layer " << i + 1 << ":" << std::endl;
    //    for (const node& n : topoLayers[i]) {
    //        // Print CRS and Name of each node in the layer
    //        std::cout << n.getCRS() << " " << n.getName() << "    ";
    //    }
    //    std::cout << std::endl;
    //}
    

    std::vector<std::vector<size_t>> indices;

    for (const auto& vec : topoLayers) {
        std::vector<size_t> idx(vec.size());
        for (size_t i = 0; i < vec.size(); ++i) {
            idx[i] = i;
        }
        indices.push_back(idx);
    }

    std::vector<std::vector<size_t>> indicesMin;
    int minIndex = -1;         // To store the index of the minimum non-zero value
    float minValue = FLT_MAX; // Start with a very large number
    // Iterate over permutations without changing the outer vector positions
    do {
        // Generate the current permutation based on indices
        std::vector<std::vector<node>> permutedTopo = topoLayers;
        for (size_t i = 0; i < permutedTopo.size(); ++i) {
            for (size_t j = 0; j < permutedTopo[i].size(); ++j) {
                permutedTopo[i][j] = topoLayers[i][indices[i][j]];
            }
        }


        // Call your getComplexity() or any other method on the current permutation
        schedule = layeredBinning(g, creditsPerSemester, genElectives, labs, permutedTopo);
        complexities.push_back(getComplexity( &schedule));
        iterationLimit--;

        if(complexities.back() < minValue){
            minValue = complexities.back();
            indicesMin = indices;
        }

        if(iterationLimit < 1){
            break;
        }

    } while (nextPermutationIndices(indices, topoLayers));

    if(minValue == FLT_MAX){
        std::cerr << "failed to find easiest schedule";
        exit(EXIT_FAILURE);
    }

    

    std::vector<std::vector<node>> permutedTopo = topoLayers;
    for (size_t i = 0; i < permutedTopo.size(); ++i) {
        for (size_t j = 0; j < permutedTopo[i].size(); ++j) {
            permutedTopo[i][j] = topoLayers[i][indicesMin[i][j]];
        }
    }

    //std::vector<std::vector<node>> permutedTopo = generatePermutation(topoLayers, minIndex);

    //

    //std::cout << "COMPLEXITIES:" << std::endl;
    //int checkinComplexi = 1000;
    //for(auto number : complexities){
    //    std::cout << number << "   ";
    //    checkinComplexi--;
    //    if(checkinComplexi < 1){
    //        break;
    //    }
    //}
    //std::cout << std::endl;

    //std::cout << std::endl << "Indices picked" << std::endl;
    //printVectorOfVectors(indicesMin);

    //std::cout << "BEST PERMUTATION CHOSEN:" << std::endl;
    //for (size_t i = 0; i < permutedTopo.size(); ++i) {
    //    std::cout << "Layer " << i + 1 << ":" << std::endl;
    //    for (const node& n : permutedTopo[i]) {
    //        // Print CRS and Name of each node in the layer
    //        std::cout << n.getCRS() << " " << n.getName() << "    ";
    //    }
    //    std::cout << std::endl;
    //}

    return layeredBinning(g, creditsPerSemester, genElectives, labs, permutedTopo);
}