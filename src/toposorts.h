#pragma once

#include "config.h"

//Layered Sorting

std::vector<Vertex> getNoInEdgeVertexes(directedGraphCourses& g) {
    std::vector<Vertex> result;
    

    VertexIterator i, end;
    for (boost::tie(i,end) = vertices(g); i != end; ++i) {
        InEdgeIterator in_i, in_end;

        boost::tie(in_i, in_end) = in_edges(*i, g);
        if (in_i == in_end) { // No incoming edges
            result.push_back(*i);
        }
    }
    return result;
}

/// @brief THIS METHOD DESTROYS THE EDGES OF THE GRAPHS. check Binning.md for further explanation.
/// @param g boost directed graph defined in config through typdef
/// @return layers of nodes
std::vector<std::vector<node>> layeredTopoSort(directedGraphCourses& g, std::set<Vertex> coursesTaken) {
    std::vector<std::vector<node>> layers;
    std::set<Vertex> processedVertices = coursesTaken; // Set to track processed vertices

    while (processedVertices.size() < num_vertices(g)) {
        // Get all vertices with no incoming edges and not yet processed
        std::vector<Vertex> layer = getNoInEdgeVertexes(g);
        std::vector<node> nodeLayer;

        // Filter out already processed vertices
        layer.erase(
            std::remove_if(layer.begin(), layer.end(), [&processedVertices](Vertex v) {
                return processedVertices.find(v) != processedVertices.end();
            }),
            layer.end()
        );

        // If no new vertices can be processed but not all vertices have been processed, there is a cycle
        if (layer.empty()) {
            if (processedVertices.size() < num_vertices(g)) {
                std::cerr << "Graph contains a cycle, cannot proceed with topological sorting." << std::endl;
                return {}; // Return empty result to indicate failure
            }
            break;
        }

        // Store the node objects from each vertex
        for (Vertex v : layer) {
            node& nodeData = boost::get(boost::vertex_name, g, v);
            nodeLayer.push_back(nodeData);
            processedVertices.insert(v); // Mark the vertex as processed

            boost::clear_out_edges(v, g);
        }

        // Add the node layer to the list of layers
        layers.push_back(nodeLayer);
    }

    return layers;
}


// ALL topo Sorts


std::unordered_set<Vertex> getCourseVerticesSet(directedGraphCourses& g, const std::vector<node>& Courses) {
    std::unordered_set<Vertex> courseVertices;
    VertexIterator vi, vi_end;

    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        node& courseNode = boost::get(boost::vertex_name, g, *vi);

        if (std::find(Courses.begin(), Courses.end(), courseNode) != Courses.end()) {
            courseVertices.insert(*vi);  // Add the vertex corresponding to the node
        }
    }
    return courseVertices;
}

// Function to compute the in-degree of each vertex
std::vector<int> computeInDegrees(const directedGraphCourses& g) {
    std::vector<int> in_degree(boost::num_vertices(g), 0);
    VertexIterator vi, vi_end;

    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        InEdgeIterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::in_edges(*vi, g); ei != ei_end; ++ei) {
            ++in_degree[*vi];
        }
    }
    return in_degree;
}

//* *********
// CALLING THE METHODS BELOW HAS NP COMPLEXITY, ESTIMATED O(V!),
// THIS WILL PROBABLY CAUSE THE PROGRAM TO NOT WORK IN SOME WAY
// EITHER RUNNING OUT OF MEMORY OR TAKING INCREDIBLY LONG EXECUTION TIME
//* *********

// Recursive function to explore all topological sorts
void allTopologicalSortsUtil(directedGraphCourses& g, std::vector<int>& in_degree, std::vector<Vertex>& result, 
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
            allTopologicalSortsUtil(g, in_degree, result, visited, all_sorts, creditsPerSemester);

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
        //for (Vertex v : result) {
        //    //std::cout << v << " ";
        //    node& nodeData = boost::get(boost::vertex_name, g, v);
        //    current_sort.push_back(nodeData);  // Store the actual node in the current sort
        //}
        all_sorts.push_back(current_sort);  // Add the current sort to the collection of all sorts
        //std::cout << std::endl;
    }
}

std::vector<std::vector<node>> allTopologicalSortsV2(directedGraphCourses& g) {
    std::vector<std::vector<node>> result;
    std::vector<node> current_sort;
    std::vector<bool> visited(boost::num_vertices(g), false);
    std::vector<int> in_degree(boost::num_vertices(g), 0);

    // Calculate in-degree for each vertex
    VertexIterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        InEdgeIterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::in_edges(*vi, g); ei != ei_end; ++ei) {
            ++in_degree[*vi];
        }
    }

    // Helper function for recursive DFS
    std::function<void()> dfs = [&]() {
        bool flag = false;
        for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
            if (in_degree[*vi] == 0 && !visited[*vi]) {
                OutEdgeIterator ei, ei_end;
                for (boost::tie(ei, ei_end) = boost::out_edges(*vi, g); ei != ei_end; ++ei) {
                    --in_degree[boost::target(*ei, g)];
                }

                current_sort.push_back(boost::get(boost::vertex_name, g, *vi));
                visited[*vi] = true;

                dfs();

                // Backtrack
                visited[*vi] = false;
                current_sort.pop_back();
                for (boost::tie(ei, ei_end) = boost::out_edges(*vi, g); ei != ei_end; ++ei) {
                    ++in_degree[boost::target(*ei, g)];
                }

                flag = true;
            }
        }

        if (!flag) {
            result.push_back(current_sort);
        }
    };

    dfs();
    return result;
}

// Function to initialize the topological sort generation and return the result
std::vector<std::vector<node>> allTopologicalSorts(directedGraphCourses& g, uint8_t creditsPerSemester) {
    std::vector<int> in_degree = computeInDegrees(g);
    std::vector<Vertex> result;
    std::vector<bool> visited(boost::num_vertices(g), false);
    std::vector<std::vector<node>> all_sorts;

    // Call the recursive utility function
    allTopologicalSortsUtil(g, in_degree, result, visited, all_sorts, creditsPerSemester);

    return all_sorts;  // Return the collection of all topological sorts
}

