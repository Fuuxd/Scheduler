#pragma once

#include "config.h"

//SO all CRNs are five digits, its probably for the best to store a vector for each course numbers that gets all the
//possible CRNS, credits and  for a specific course, but the node main ID should be the CRS 
//difficulty is in the CRS and goes from [1, 7]

//CRN required bits: 17

//CRS required bits: 14
//credits required bits: 4
// total minimum bits would be 24
struct section{
    unsigned CRN: 17;
    unsigned credits: 4;
    unsigned difficulty: 3; // [1,8] for possible difficulties
    // could add seats available or whatever but for now assume its open, when building graph only add open sections
};

class node {
public:
    //consider privating member variables future
    unsigned CRS : 14;
    uint8_t credsNeeded = 0;
    bool hasLab = false;
    std::string courseName; 
    std::vector<section> sections;

    // Default constructor
    node() 
        : CRS(0000), hasLab(false), sections({{04040, 3, 0}}) {}

    node(uint16_t CRS_, const std::string &courseName_, const std::vector<section> &sections_) 
        : CRS(CRS_), courseName(courseName_) , hasLab(false), sections(sections_) {}

    node(uint16_t CRS_, const std::string &courseName_, bool lab_, const std::vector<section> &sections_) 
        : CRS(CRS_), courseName(courseName_), hasLab(lab_), sections(sections_) {}

    node(uint16_t CRS_, const std::string &courseName_, bool lab_, const std::vector<section> &sections_, uint8_t credsNeeded_) 
        : CRS(CRS_), courseName(courseName_), hasLab(lab_), sections(sections_), credsNeeded(credsNeeded_){}

    node(uint16_t CRS_, const std::string &courseName_,  const std::vector<section> &sections_, uint8_t credsNeeded_) 
        : CRS(CRS_), courseName(courseName_), hasLab(false), sections(sections_), credsNeeded(credsNeeded_){}

    ~node() = default;

    int getCRS() const { return CRS;}

    std::string getName() const { return courseName;}

    section getSection() const {
        //Manage obtaining open sections. For now just get the first available one.
        if (!sections.empty()) {
            return sections[0]; // Return the first available section
        } else {
            throw std::out_of_range("No sections available");
        }
    }

    bool operator==(const node& other) const {
        return CRS == other.CRS && courseName == other.courseName;
    }

    bool operator<(const node& other) const {
        return this->CRS < other.CRS;  // Compare based on CRS
    }
};

// Custom vertex writer to output courseName for Graphviz
struct vertex_writer {
    directedGraphCourses const& graph;

    vertex_writer(directedGraphCourses const& g) : graph(g) {}

    template <class Vertex>
    void operator()(std::ostream& out, const Vertex& v) const {
        const node& n = boost::get(boost::vertex_name, graph)[v];
        std::size_t index = boost::get(boost::vertex_index, graph)[v];
        out << "[label=\"" << std::to_string(n.getCRS()) << "\", shape=\"square\"]"; //change to std::tostring n.getCRS() if desired
    }
};