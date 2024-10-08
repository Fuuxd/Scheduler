#pragma once

#include "config.h"
#include "node.h"
#include "semesterVector.h"

// Function to check which vertices have zero in-degree (sources)
std::vector<int> find_sources(const directedGraphCourses& g, const std::vector<int>& in_degree) {
    std::vector<int> sources;
    for (int v = 0; v < boost::num_vertices(g); ++v) {
        if (in_degree[v] == 0) {
            sources.push_back(v);
        }
    }
    return sources;
}

// Function to recursively count the number of topological sorts
int number_of_topological_sortings(directedGraphCourses g) {
    // Get the in-degrees of all vertices
    std::vector<int> in_degree(boost::num_vertices(g), 0);
    boost::graph_traits<directedGraphCourses>::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        boost::graph_traits<directedGraphCourses>::out_edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::out_edges(*vi, g); ei != ei_end; ++ei) {
            in_degree[boost::target(*ei, g)]++;
        }
    }

    // Find all vertices with zero in-degree (sources)
    std::vector<int> sources = find_sources(g, in_degree);

    // Base case: if no vertices are left, we found a valid topological sort
    if (boost::num_vertices(g) == 0) return 1;

    // Recursive case: for each source, remove it and continue the recursion
    int count = 0;
    for (int source : sources) {
        // Remove the source from the graph and decrease in-degrees of its neighbors
        directedGraphCourses g_copy = g;
        std::vector<int> in_degree_copy = in_degree;

        boost::clear_vertex(source, g_copy);
        boost::remove_vertex(source, g_copy);

        for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
            boost::graph_traits<directedGraphCourses>::out_edge_iterator ei, ei_end;
            for (boost::tie(ei, ei_end) = boost::out_edges(*vi, g); ei != ei_end; ++ei) {
                int target = boost::target(*ei, g);
                if (target == source) {
                    in_degree_copy[boost::target(*ei, g)]--;
                }
            }
        }

        count += number_of_topological_sortings(g_copy);
    }

    return count;
}