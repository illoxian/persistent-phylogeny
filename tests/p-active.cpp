#include "../src/rbgraph.hpp"
#include "../src/functions.hpp"


int main(int argc, const char* argv[]) {

  // CASE: no active characters - 3 species with increasing different degree - isolated nodes
  RBGraph g;
  RBVertex s1, s2, s3, s4, s5, s6, c1, c2, c3, c4, c5;

  s1 = add_vertex("s1", Type::species, g);
  s2 = add_vertex("s2", Type::species, g);
  s3 = add_vertex("s3", Type::species, g);
  s4 = add_vertex("s4", Type::species, g);
  s5 = add_vertex("s5", Type::species, g);
  c1 = add_vertex("c1", Type::character, g);
  c2 = add_vertex("c2", Type::character, g);
  c3 = add_vertex("c3", Type::character, g);
  c4 = add_vertex("c4", Type::character, g);
  c5 = add_vertex("c5", Type::character, g);

  add_edge(s1, c1, g);
  add_edge(s1, c2, g);
  add_edge(s1, c3, g);
  add_edge(s1, c4, g);
  add_edge(s1, c5, g);
  add_edge(s2, c1, g);
  add_edge(s2, c2, g);
  add_edge(s2, c3, g);
  add_edge(s3, c1, g);
  add_edge(s3, c2, g);
  add_edge(s3, c3, g);
  add_edge(s3, c4, g);

  assert(get_minimal_p_active_species(g) == s3);

  // CASE: all active characters - isolated nodes
  clear(g);

  s1 = add_vertex("s1", Type::species, g);
  s2 = add_vertex("s2", Type::species, g);
  s3 = add_vertex("s3", Type::species, g);
  s4 = add_vertex("s4", Type::species, g);
  c1 = add_vertex("c1", Type::character, g);
  c2 = add_vertex("c2", Type::character, g);
  c3 = add_vertex("c3", Type::character, g);

  add_edge(s1, c1, Color::red, g);
  add_edge(s1, c2, Color::red, g);
  add_edge(s1, c3, Color::red, g);
  add_edge(s2, c1, Color::red, g);
  add_edge(s2, c2, Color::red, g);
  add_edge(s3, c2, Color::red, g);

  assert(!exists(get_minimal_p_active_species(g), g));

  // CASE: all inactive characters - species with same degree
  clear(g);
  s1 = add_vertex("s1", Type::species, g);
  s2 = add_vertex("s2", Type::species, g);
  s3 = add_vertex("s3", Type::species, g);
  c1 = add_vertex("c1", Type::character, g);
  c2 = add_vertex("c2", Type::character, g);

  add_edge(s1, c1, g);
  add_edge(s1, c2, g);
  add_edge(s2, c1, g);
  add_edge(s2, c2, g);
  add_edge(s3, c1, g);
  add_edge(s3, c2, g);

  assert(!exists(get_minimal_p_active_species(g), g));

  // CASE: red sigmagraph
  clear(g);

  add_species("s1", g);
  add_species("s2", g);
  add_species("s3", g);
  add_species("s4", g);
  add_species("s5", g);
  add_character("c1", g);
  add_character("c2", g);
  add_character("c3", g);
  add_character("c4", g);

  add_edge("c1", "s1", Color::red, g);
  add_edge("c1", "s2", Color::red, g);
  add_edge("c2", "s2", Color::red, g);
  add_edge("c2", "s3", Color::red, g);
  add_edge("c3", "s4", Color::black, g);
  add_edge("c3", "s5", Color::black, g);
  add_edge("c4", "s5", Color::black, g);

  assert(!exists(get_minimal_p_active_species(g), g));

  // CASE: closest neighbor has the maximum number of inactive characters
  clear(g);

  add_species("s1", g);
  add_species("s2", g);
  add_species("s3", g);
  add_character("c1", g);
  add_character("c2", g);
  add_character("c3", g);
  add_character("c4", g);

  add_edge("c1", "s1", Color::black, g);
  add_edge("c2", "s1", Color::black, g);
  add_edge("c3", "s1", Color::black, g);
  add_edge("c4", "s1", Color::black, g);
  add_edge("c3", "s2", Color::black, g);

  assert(get_minimal_p_active_species(g) == get_vertex("s2", g));

  // CASE: disjunctive species set
  clear(g);
  add_species("s1", g);
  add_species("s2", g);
  add_character("c1", g);
  add_character("c2", g);

  add_edge("s1", "c1", Color::black, g);
  add_edge("s2", "c2", Color::black, g);
  assert(!exists(get_minimal_p_active_species(g), g));

  // CASE: overlapping species
  clear(g);
  add_species("s1", g);
  add_species("s2", g);
  add_character("c1", g);
  add_character("c2", g);
  add_character("c3", g);

  add_edge("s1", "c1", Color::black, g);
  add_edge("s1", "c2", Color::black, g);
  add_edge("s2", "c2", Color::black, g);
  add_edge("s2", "c3", Color::black, g);

  assert(!exists(get_minimal_p_active_species(g), g));

  // CASE: all nodes isolated
  clear(g);
  add_species("s1", g);
  add_species("s2", g);
  add_character("c1", g);
  add_character("c2", g);

  assert(!exists(get_minimal_p_active_species(g), g));

  // closest neighbor is inactive
  clear(g);
  add_species("s1", g);
  add_species("s2", g);
  add_character("c1", g);
  add_character("c2", g);
  add_character("c3", g);

  add_edge("s1", "c1", Color::black, g);
  add_edge("s2", "c1", Color::black, g);
  add_edge("s2", "c2", Color::black, g);
  add_edge("s2", "c3", Color::red, g);

  assert(get_minimal_p_active_species(g) == get_vertex("s1", g));

  std::cout << "p-active: tests passed" << std::endl;

  return 0;
}
