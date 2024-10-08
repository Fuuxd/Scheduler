#pragma once


#include <fstream>
#include <vector>
#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include <utility>

//boost related
#include <boost/config.hpp>
#include <boost/graph/properties.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/topological_sort.hpp>

class node; //Forward declaration

//typedefs boost related
typedef std::pair<node,node> Edge;
typedef boost::property<boost::vertex_name_t, node> VertexProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, VertexProperty, boost::property<boost::vertex_index_t, std::size_t>> directedGraphCourses;
typedef boost::graph_traits<directedGraphCourses>::vertex_descriptor Vertex;
typedef boost::graph_traits<directedGraphCourses>::vertex_iterator VertexIterator;
typedef boost::graph_traits<directedGraphCourses>::out_edge_iterator OutEdgeIterator;
typedef boost::graph_traits<directedGraphCourses>::in_edge_iterator InEdgeIterator;

