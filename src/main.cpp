// Big conundrum appears with the ORs in the course requirements
// as the directed edge does not know if the other one is picked it doesnt need to be picked

// assumes all courses can somehow be taken with no timeslot incoherences

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

    hardcodeGraph();

    uint8_t creditsPerSemester = 15;

    std::string gv_filename = "graphOutput.gv";
    
    // Export graph to then convert to png. check README.md
    std::ofstream outf(gv_filename);
    boost::write_graphviz(outf, G, vertex_writer(G));
    outf.close();

    std::string command = "dot -Tsvg " + gv_filename + " -o graphConverted.svg";

    system(command.c_str());

    std::vector<semesterVector> binnedSchedule;

    //binnedSchedule = layeredBinningFirst(G, creditsPerSemester, genElectives, nodeLabs);
    
    //std::cout << "Semester plan for " << std::to_string(creditsPerSemester) << " credits per semester (" << std::endl;

    //printSchedule(&binnedSchedule);
    
    //std::cout << std::endl << std::endl;

    binnedSchedule = layeredBinningPermute(G, creditsPerSemester, genElectives, nodeLabs, 100000);
    
    std::cout << "Semester plan for " << std::to_string(creditsPerSemester) << " credits per semester (" << std::endl;

    printSchedule(&binnedSchedule);
    
}