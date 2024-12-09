// COMPILE THIS PROGRAM TO PRODUCE EXECUTABLE NAMED scheduleCreator
//so current future plan is to: make layered topological take in a set of vertices correctly, by deleting its edges.
// then finish binning to do permutation by steps plan
//finish ironing out input-output for page.

#include "config.h"
#include "node.h"
#include "semesterVector.h"
#include "holePuncher.cpp"

#include "hardcodeGraph.cpp"
//#include "hardcodeGraphSmall.cpp"

#include "numberOfTopo.h"


int main(int argc, char* argv[]){

    if (argc < 2) { // Check if at least one argument (node count) is passed
        std::cerr << "No nodes selected.\n";
        return 1; // Indicate error
    }

    int nodeCount = std::stoi(argv[1]); // Get the number of nodes
    std::vector<std::string> nodes;

    // Process command-line arguments
    //std::cout << "NODES GOT ";
    for (int i = 2; i < nodeCount+2; ++i) { // Start from 2 to skip the program name and count
        nodes.push_back(argv[i]);
        //std::cout << " " << argv[i];
    }
    std::cout << std::endl;

    bool isTakingSummer = std::stoi(argv[argc - 2]) != 0; // If "1", it's true; else, false
    int summerIndex = std::stoi(argv[argc - 1]); 
    summerIndex--;



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


    std::vector<semesterVecVertex> fittedSchedule = scheduleFit(G, &vertexSet, 0, isTakingSummer, summerIndex);
    std::vector<semesterVecVertex> fittedSchedule2;
    printScheduleVertexes(fittedSchedule);


    for(int i = 1; i < 4; i++){
        fittedSchedule = scheduleFit(G, &vertexSet, i, isTakingSummer, summerIndex);
        fittedSchedule2 = scheduleMov(G, &vertexSet, 0, isTakingSummer, summerIndex);
        
        if(fittedSchedule.size() <= fittedSchedule2.size()){
            //std::cout << " Fit";
        }else{
            fittedSchedule = fittedSchedule2;
            //std::cout << " Mov";
        }
        
        if(isFullTime(G, &fittedSchedule)){
            std::cout<<"\n\n\n\nAlternative: \n\n";
            printScheduleVertexes(fittedSchedule);
        }
    }
    
}