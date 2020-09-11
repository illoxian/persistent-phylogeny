/** 
 * @file basic_rbgraph_functions.cpp
 * @author Simone Paolo Mottadelli
 * 
 * @brief This file contains the tests for the basic functions
 * of the red black graph
 * 
 */

#include "rbgraph.hpp"
#include <iostream>

void test_simple_add_vertex() 
{
  // add_vertex() test
  RBGraph g;
  RBVertex v1, v2;

  v1 = add_vertex("v1", Type::species, g);
  v2 = add_vertex("v2", Type::character, g);

  assert(g[boost::graph_bundle].num_species == 1);
  assert(g[boost::graph_bundle].num_characters == 1);
  assert(g[boost::graph_bundle].vertex_map.size() == 2);
  assert(g[boost::graph_bundle].vertex_map.at("v1") == v1);
  assert(g[g[boost::graph_bundle].vertex_map.at("v1")].name == "v1");
  assert(g[g[boost::graph_bundle].vertex_map.at("v1")].type == Type::species);
  assert(g[g[boost::graph_bundle].vertex_map.at("v2")].type == Type::character);

  // add_species() and add_character() tests
  RBVertex v3, v4;
  v3 = add_species("v3", g);
  v4 = add_character("v4", g);

  assert(g[g[boost::graph_bundle].vertex_map.at("v3")].type == Type::species);
  assert(g[g[boost::graph_bundle].vertex_map.at("v4")].type == Type::character);

  std::cout << "test_add_vertex(): passed" << std::endl;
}


void test_add_vertex_with_duplicates() 
{
  // add_vertex() test
  RBGraph g;
  RBVertex v1;

  try {
    v1 = add_vertex("v1", Type::species, g);
    v1 = add_vertex("v1", Type::species, g);
    assert(false);
  } catch(...) {
    std::cout << "test_add_vertex_with_duplicates(): passed" << std::endl;
  }
}


int main(int argc, char *argv[])
{
  test_simple_add_vertex();
  test_add_vertex_with_duplicates();
  test_add_edge();
}