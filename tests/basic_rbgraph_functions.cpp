/** 
 * @file basic_rbgraph_functions.cpp
 * @author Simone Paolo Mottadelli
 * 
 * @brief This file contains the tests for the basic functions
 * of the red black graph
 * 
 */

#include "../src/rbgraph.hpp"
#include <iostream>

void test_simple_add_vertex() {
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


void test_add_vertex_with_duplicates() {
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


void test_get_vertex() {
  RBGraph g;
  RBVertex v1;

  v1 = add_vertex("v1", Type::character, g);

  assert(g[get_vertex("v1", g)].type == Type::character);
  g[vertex_map(g).at("v1")].type = Type::species;
  assert(g[get_vertex("v1", g)].type == Type::species);

  try {
    get_vertex("v2", g);
    assert(false);
  } catch (...) {
    // OK - passed
  }

  std::cout << "test_get_vertex(): passed" << std::endl;
}


void test_add_edge() {
  RBGraph g;
  RBVertex v1, v2, v3;

  v1 = add_vertex("v1", Type::species, g);
  v2 = add_vertex("v2", Type::character, g);
  v3 = add_vertex("v3", Type::character, g);

  RBEdge e1, e2;
  std::tie(e1, std::ignore) = add_edge(v1, v2, g);
  std::tie(e2, std::ignore) = add_edge(v1, v3, Color::red, g);

  assert(g[e1].color == Color::black);
  assert(g[e2].color == Color::red);
  assert(g.m_edges.size() == 2);

  std::cout << "test_add_edge(): passed" << std::endl;
}


void test_graph_size() {
  RBGraph g;
  RBVertex v1, v2, v3;

  v1 = add_vertex("v1", Type::species, g);
  v2 = add_vertex("v2", Type::character, g);
  v3 = add_vertex("v3", Type::character, g);

  RBEdge e1, e2;
  std::tie(e1, std::ignore) = add_edge(v1, v2, g);
  std::tie(e2, std::ignore) = add_edge(v1, v3, Color::red, g);

  int characters = g[boost::graph_bundle].num_characters;
  int species = g[boost::graph_bundle].num_species;
  assert(g.m_vertices.size() == characters + species);
  
  assert(g.m_edges.size() == 2);

  std::cout << "test_graph_size(): passed" << std::endl;
}


void test_get_edge() {
  RBGraph g;
  RBVertex v1, v2, v3;

  v1 = add_vertex("v1", Type::species, g);
  v2 = add_vertex("v2", Type::character, g);
  v3 = add_vertex("v3", Type::character, g);

  RBEdge e1, e2;
  std::tie(e1, std::ignore) = add_edge(v1, v2, g);
  std::tie(e2, std::ignore) = add_edge(v1, v3, Color::red, g);

  assert(get_edge(v1, v3, g) == e2);
  assert(g[get_edge(v1, v3, g)].color == Color::red);

  try {
    get_edge(v2, v3, g);
    assert(false);
  } catch (...) {
    // OK - passed
  }

  RBVertex v4, v5;
  try {
    get_edge(v4, v5, g);
    assert(false);
  } catch (...) {
    // OK - passed
  }

  std::cout << "test_get_edge(): passed" << std::endl;
}


void test_remove_vertex() {
  RBGraph g;

  RBVertex v1, v2, v3;
  v1 = add_vertex("v1", Type::species, g);
  v2 = add_vertex("v2", Type::character, g);
  v3 = add_vertex("v3", Type::character, g);

  RBEdge e1, e2, e3;
  std::tie(e1, std::ignore) = add_edge(v1, v2, g);
  std::tie(e2, std::ignore) = add_edge(v1, v3, Color::red, g);
  std::tie(e3, std::ignore) = add_edge(v2, v3, Color::black, g);

  // check that the edges incident in the vertex have been deleted
  assert(get_edge(v1, v2, g) == e1);
  remove_vertex(v2, g);
  try {
    get_edge(v1, v2, g);
    assert(false);
  } catch (...) {
    // OK - passed
  }

  // check that the vertex has been deleted successfully
  assert(vertex_map(g).size() == 2);
  try {
    get_vertex("v2", g);
    assert(false);
  } catch (...) {
    // OK - passed
  }

  // check the number of edges of the rbgraph g
  assert(g.m_edges.size() == 1);

  std::cout << "test_remove_vertex(): passed" << std::endl;
}


void test_remove_non_existent_vertex() {
  RBGraph g;

  RBVertex v1;
  v1 = add_vertex("v1", Type::species, g);

  try {
    remove_vertex("v3", g);
    assert(false);
  } catch (...) {
    // OK - passed
  }

  RBVertex v2;
  try {
    remove_vertex(v2, g);
    assert(false);
  } catch (...) {
    // OK - passed
  }

  remove_vertex(v1, g);

  try {
    remove_vertex(v1, g);
  } catch (...) {
    // OK - passed
  }

  std::cout << "test_remove_non_existent_vertex(): passed" << std::endl;
}

void test_equal_graph() {

}


void test_copy_graph() {
  RBGraph g;
  RBVertex v1, v2, v3;

  v1 = add_vertex("v1", Type::species, g);
  v2 = add_vertex("v2", Type::character, g);
  v3 = add_vertex("v3", Type::character, g);

  RBEdge e1, e2;
  std::tie(e1, std::ignore) = add_edge(v1, v2, g);
  std::tie(e2, std::ignore) = add_edge(v1, v3, Color::red, g);

  RBGraph g2;
  copy_graph(g, g2);

  assert(g == g2);

  RBGraph g3;
  RBVertexMap v_map;

  copy_graph(g2, g3, v_map);

  assert(g2 == g3);

  std::cout << "test_copy_graph(): passed" << std::endl;
}

int main(int argc, char *argv[]) {
  test_simple_add_vertex();
  test_add_vertex_with_duplicates();
  test_get_vertex();
  test_add_edge();
  test_graph_size();
  test_get_edge();
  test_remove_vertex();
  test_remove_non_existent_vertex();
  test_equal_graph();
  test_copy_graph();
}