// COMPILE THIS PROGRAM TO PRODUCE EXECUTABLE NAMED scheduleCreator
//so current future plan is to: make layered topological take in a set of vertices correctly, by deleting its edges.
// then finish binning to do permutation by steps plan
//finish ironing out input-output for page.

#include "config.h"
#include "node.h"
#include "semesterVector.h"
#include "toposorts.h"
#include "layeredBinning.h"
#include "bruteForce.h"

#include "hardcodeGraph.cpp"
//#include "hardcodeGraphSmall.cpp"

#include "numberOfTopo.h"


int main(){

    std::vector<std::string> nodes = {"5", "0", "8", "7", "10", "11"};

    hardcodeGraph();

    std::set<Vertex> vertexSet;

    //TODO: get credits taken somewhere out of the nodes ehre

    // Iterate over each string in nodeNames
    for (const std::string& nodeIndexStr : nodes) {
        try {
            // Convert string index to integer
            size_t nodeIndex = std::stoul(nodeIndexStr);

            // Check if the index is valid
            if (nodeIndex >= boost::num_vertices(G)) {
                std::cout << "Invalid index: " << nodeIndexStr << " (out of range)" << std::endl;
                continue;  // Skip invalid indices
            }

            // Retrieve the vertex from the graph
            Vertex vertex = boost::vertex(nodeIndex, G);
            vertexSet.insert(vertex);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error converting string to index: " << nodeIndexStr << std::endl;
            continue;  // Skip invalid strings
        } catch (const std::out_of_range& e) {
            std::cerr << "Index is out of range: " << nodeIndexStr << std::endl;
            continue;  // Skip out-of-range indices
        }
    }


    uint8_t creditsPerSemester = 15;

    std::vector<semesterVector> binnedSchedule = stepLayeredBinningPermute(G, creditsPerSemester, true, 9, 1, genElectives, nodeLabs, vertexSet, 1000000, 3);
    
    std::cout << "Semester plan for " << std::to_string(creditsPerSemester) << " credits per semester (" << std::endl;

    printSchedule(&binnedSchedule);
    
}