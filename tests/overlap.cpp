#include "../src/rbgraph.hpp"


int main(int argc, const char* argv[]) {
  RBGraph g;
  RBVertex s1, s2, s3, s4,
           c1, c2;

  s1 = add_vertex("s1", Type::species, g);
  s2 = add_vertex("s2", Type::species, g);
  s3 = add_vertex("s3", Type::species, g);
  s4 = add_vertex("s4", Type::species, g);
  c1 = add_vertex("c1", Type::character, g);
  c2 = add_vertex("c2", Type::character, g);

  add_edge(s1, c1, g);
  add_edge(s2, c2, g);

  std::map<RBVertex, std::list<RBVertex>> adj_spec;
  build_adjacent_species_map(adj_spec, g);
  assert(!overlap(c1, c2, adj_spec));
  add_edge(c2, s1, g);
  build_adjacent_species_map(adj_spec, g);
  assert(!overlap(c1, c2, adj_spec));
  add_edge(c1, s3, g);
  build_adjacent_species_map(adj_spec, g);
  assert(overlap(c2, c1, adj_spec));

  std::cout << "overlap: tests passed" << std::endl;
}
