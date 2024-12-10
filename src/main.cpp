//TODO: FIX GENERAL ELECTIVE HANDLING IN BINNING PERMUTE

// Big conundrum appears with the ORs in the course requirements
// as the directed edge does not know if the other one is picked it doesnt need to be picked

// assumes all courses can somehow be taken with no timeslot incoherences

#include "config.h"
#include "node.h"
#include "holePuncher.cpp"
#include "hardcodeGraph.cpp"
//#include "hardcodeGraphSmall.cpp"

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

    std::ofstream htmlFile("schedule.html");

    if (!htmlFile) {
        std::cerr << "Error: Could not open file for writing.\n";
        return 1;
    }

    // Write the HTML header
    htmlFile << "<!DOCTYPE html>\n"
             << "<html lang=\"en\">\n"
             << "<head>\n"
             << "    <meta charset=\"UTF-8\">\n"
             << "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
             << "    <title>Schedule</title>\n"
             << "    <style>\n"
             << "        .checkbox { cursor: pointer; display: block; margin: 5px 0; }\n"
             << "        .grid-container { display: grid; grid-template-columns: 1fr 1fr; gap: 20px; }\n"
             << "        .column { display: flex; flex-direction: column; }\n"
             << "    </style>\n"
             << "</head>\n"
             << "<body>\n"
             << "<h1>Schedule</h1>\n";

    // Start the grid container
    htmlFile << "<div class=\"grid-container\">\n";

    size_t printSemNum = 1;
    size_t midpoint = (baseSchedule.size()+1) / 2;

    for (size_t i = 0; i < baseSchedule.size(); ++i) {
        const auto& vector = baseSchedule[i];

        // Open a new column if we reach the midpoint
        if (i == midpoint) {
            htmlFile << "</div>\n<div class=\"column\">\n";
        } else if (i == 0) {
            htmlFile << "<div class=\"column\">\n";
        }

        if (printSemNum == 5) {
            htmlFile << "<h2>Summer</h2>\n";
            ++printSemNum;
        } else {
            htmlFile << "<h2>Semester " << printSemNum++ << "</h2>\n";
        }

        for (const auto& v : vector) {
            node& nodeData = boost::get(boost::vertex_name, G, v);

            // Write the HTML span element for each vertex
            htmlFile << "<span class=\"checkbox\" data-value=\"" 
                     << v 
                     << "\">" 
                     << std::to_string(nodeData.getCRS()) 
                     << " " 
                     << nodeData.getName() 
                     << "</span>\n";
        }
    }

    // Close the final column and grid container
    htmlFile << "</div>\n</div>\n";

    // Write the HTML footer
    htmlFile << "</body>\n</html>\n";

    htmlFile.close();
    std::cout << "Base Schedule HTML has been written to schedule.html.\n";


    //binnedSchedule = layeredBinningFirst(G, creditsPerSemester, genElectives, nodeLabs);
    
    //std::cout << "Semester plan for " << std::to_string(creditsPerSemester) << " credits per semester (" << std::endl;

    //printSchedule(&binnedSchedule);
    
    //std::cout << std::endl << std::endl;
    
    std::set<Vertex> emptySet {};

    bool takingSummer = true;
    uint8_t creditsPerSummer = 9; 
    uint8_t semestersBeforeSummer = 4;

    //binnedSchedule = layeredBinningPermute(G, creditsPerSemester, genElectives, nodeLabs, emptySet, 100000);
    std::vector<semesterVecVertex> fittedSchedule = scheduleFit(G, &emptySet);

    std::cout<<"OPTION 1: \n";
    printScheduleVertexes(fittedSchedule);

    std::cout<<"OPTION 2: \n";
    fittedSchedule = scheduleFit(G, &emptySet, 1);

    std::cout<<"OPTION 3: \n";
    fittedSchedule = scheduleFit(G, &emptySet, 2);

    std::cout<<"OPTION 4: \n";
    fittedSchedule = scheduleFit(G, &emptySet, 3);
}