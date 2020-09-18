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

  std::cout << "test_simple_add_vertex(): passed" << std::endl;
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

void test_remove_edge() {
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

  remove_edge(v1, v3, g);
  assert(!exists(v1, v3, g));
  assert(g.m_edges.size() == 1);
  remove_edge(e1, g);
  assert(g.m_edges.size() == 0);
  assert(!exists(e1.m_source, e1.m_target, g));

  std::cout << "test_remove_edge(): passed" << std::endl;
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

void test_exists() {
  RBGraph g;

  RBVertex v1 = add_vertex("v1", Type::species, g);
  RBVertex v2 = add_vertex("v2", Type::character, g);
  RBVertex v3 = add_vertex("v3", Type::character, g);
  assert(exists("v2", g));
  assert(!exists("v4", g));

  RBEdge e1, e2;
  std::tie(e1, std::ignore) = add_edge(v1, v2, g);
  std::tie(e2, std::ignore) = add_edge(v1, v3, Color::red, g);
  assert(exists("v1", "v3", g));
  assert(!exists("v4", "v5", g));

  std::cout << "test_exists(): passed" << std::endl;
}

void test_copy_graph() {
  RBGraph g;

  RBVertex v1 = add_vertex("v1", Type::species, g);
  RBVertex v2 = add_vertex("v2", Type::character, g);
  RBVertex v3 = add_vertex("v3", Type::character, g);

  RBEdge e1, e2;
  std::tie(e1, std::ignore) = add_edge(v1, v2, g);
  std::tie(e2, std::ignore) = add_edge(v1, v3, Color::red, g);

  RBGraph g2;
  copy_graph(g, g2);
  RBVertex v1_g2 = get_vertex("v1", g2);
  RBVertex v2_g2 = get_vertex("v2", g2);
  RBVertex v3_g2 = get_vertex("v3", g2);

  assert(g[v1].name == g2[v1_g2].name);
  assert(g[v1].type == g2[v1_g2].type);
  assert(g[v2].name == g2[v2_g2].name);
  assert(g[v2].type == g2[v2_g2].type);
  assert(g[v3].name == g2[v3_g2].name);
  assert(g[v3].type == g2[v3_g2].type);
  assert(g[e1].color == g2[get_edge(v1_g2, v2_g2, g2)].color);
  assert(g[e1.m_source].name == g2[get_edge(v1_g2, v2_g2, g2).m_source].name);
  assert(g[e1.m_target].name == g2[get_edge(v1_g2, v2_g2, g2).m_target].name);
  assert(g[e2].color == g2[get_edge(v1_g2, v3_g2, g2)].color);
  assert(g[e2.m_source].name == g2[get_edge(v1_g2, v3_g2, g2).m_source].name);
  assert(g[e2.m_target].name == g2[get_edge(v1_g2, v3_g2, g2).m_target].name);
  assert(g.m_vertices.size() == g2.m_vertices.size());
  assert(g.m_edges.size() == g2.m_edges.size());
  assert(num_species(g) == num_species(g2));
  assert(num_characters(g) == num_characters(g2));

  g2[v1_g2].name = "pippo";
  assert(g2[v1_g2].name == "pippo" && g[v1].name == "v1");

  g2[v2_g2].type = Type::character;
  g[v2].type = Type::species;
  assert(g2[v2_g2].type != g[v2].type);

  add_vertex("s*", Type::character, g2);
  assert(exists("s*", g2));
  assert(!exists("s*", g));

  assert(g2[get_edge(v1_g2, v3_g2, g2).m_source].name == "pippo");
  assert(g[get_edge(v1, v3, g).m_source].name == "v1");

  std::cout << "test_copy_graph(): passed" << std::endl;
}

void test_read_graph() {
  RBGraph g;
  read_graph("test_5x2.txt", g);
  
  assert(num_characters(g) == 2);
  assert(num_species(g) == 5);
  assert(exists("c0", g));
  assert(exists("c1", g));
  assert(exists("s0", g));
  assert(exists("s1", g));
  assert(exists("s2", g));
  assert(exists("s3", g));
  assert(exists("s4", g));
  assert(!exists("s5", g));
  assert(!exists("c2", g));

  assert(exists("s0", "c1", g));
  assert(!exists("s0", "c0", g));
  assert(exists("s1", "c0", g));
  assert(exists("s1", "c1", g));
  assert(exists("s2", "c0", g));
  assert(exists("s3", "c0", g));
  assert(exists("s3", "c1", g));
  assert(exists("s4", "c0", g));
  assert(exists("s4", "c1", g));
  assert(exists("s4", "c1", g));
  assert(!exists("s2", "c1", g));

  std::cout << "test_read_graph(): passed" << std::endl;
}

void test_has_red_sigmagraph() {
  RBGraph g;
  add_vertex("s1", Type::species, g);
  add_vertex("s2", Type::species, g);
  add_vertex("s3", Type::species, g);
  add_vertex("c1", Type::character, g);
  add_vertex("c2", Type::character, g);

  assert(!has_red_sigmagraph(g));

  add_edge("s1", "c1", Color::red, g);
  assert(!has_red_sigmagraph(g));
  add_edge("s2", "c1", Color::red, g);
  assert(!has_red_sigmagraph(g));
  add_edge("s2", "c2", Color::red, g);
  assert(!has_red_sigmagraph(g));
  add_edge("s3", "c2", Color::red, g);
  assert(has_red_sigmagraph(g));
  add_vertex("c3", Type::character, g);
  assert(has_red_sigmagraph(g));
  add_edge("s3", "c3", Color::black, g);
  assert(has_red_sigmagraph(g));

  clear(g);
  add_species("s1", g);
  add_species("s2", g);
  add_species("s3", g);
  add_species("s4", g);
  add_character("c1", g);
  add_character("c2", g);

  add_edge("s1", "c1", Color::red, g);
  add_edge("s2", "c1", Color::red, g);
  add_edge("s3", "c2", Color::red, g);
  add_edge("s4", "c2", Color::red, g);
  assert(!has_red_sigmagraph(g));

  add_edge("c2", "s2", Color::red, g);
  assert(has_red_sigmagraph(g));

  std::cout << "test_has_red_sigmagraph(): passed" << std::endl;
}

int test_get_neighbors() {
  RBGraph g;
  RBVertex s1 = add_species("s1", g);
  RBVertex s2 = add_species("s2", g);
  RBVertex s3 = add_species("s3", g);
  RBVertex s4 = add_species("s4", g);
  add_character("c1", g);
  add_character("c2", g);
  add_character("c3", g);

  assert(get_neighbors(s1, g).empty());
  add_edge("s1", "c1", Color::black, g);
  assert(get_neighbors(s1, g).empty());
  add_edge("s2", "c1", Color::black, g);
  assert(get_neighbors(s1, g).size() == 1);
  assert(*get_neighbors(s1, g).begin() == s2);
  add_edge("s3", "c2", Color::black, g);
  assert(get_neighbors(s1, g).size() == 1);
  assert(*get_neighbors(s1, g).begin() == s2);
  assert(get_neighbors(s3, g).empty());
  add_edge("s4", "c2", Color::black, g);
  add_edge("s4", "c3", Color::black, g);
  add_edge("s3", "c3", Color::black, g);
  assert(get_neighbors(s4, g).size() == 1);
  assert(*get_neighbors(s4, g).begin() == s3);
  add_edge("s3", "c1", Color::black, g);
  assert(get_neighbors(s2, g).size() == 2);
  assert(*get_neighbors(s2, g).begin() == s3 && *++(get_neighbors(s2, g).begin()) == s1);
  assert(get_neighbors(s3, g).size() == 3);

  RBVertex s5 = add_species("s5", g);
  RBVertex s6 = add_species("s6", g);
  add_character("c4", g);

  add_edge("s5", "c4", Color::red, g);
  assert(get_neighbors(s5, g).empty());
  add_edge("s6", "c4", Color::red, g);
  assert(get_neighbors(s5, g).empty());
  add_edge("s5", "c1", Color::black, g);
  assert(get_neighbors(s5, g).size() == 3);
  assert(std::find(get_neighbors(s5, g).begin(), get_neighbors(s5, g).end(), s1) != get_neighbors(s5, g).end());
  assert(std::find(get_neighbors(s5, g).begin(), get_neighbors(s5, g).end(), s2) != get_neighbors(s5, g).end());
  assert(std::find(get_neighbors(s5, g).begin(), get_neighbors(s5, g).end(), s3) != get_neighbors(s5, g).end());

  std::cout << "test_get_neighbors(): passed" << std::endl;
}

int main(int argc, char *argv[]) {
  test_simple_add_vertex();
  test_add_vertex_with_duplicates();
  test_get_vertex();
  test_add_edge();
  test_graph_size();
  test_get_edge();
  test_remove_edge();
  test_remove_vertex();
  test_remove_non_existent_vertex();
  test_exists();
  test_copy_graph();
  test_read_graph();
  test_has_red_sigmagraph();
  test_get_neighbors();
}