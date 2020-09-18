#include "../src/rbgraph.hpp"
#include "../src/functions.hpp"


int main(int argc, const char* argv[]) {
  RBGraph g;
  RBVertex s1, s2, s3, s4, s5, c1, c2, c3, c4;

  s1 = add_vertex("s1", Type::species, g);
  s2 = add_vertex("s2", Type::species, g);
  s3 = add_vertex("s3", Type::species, g);
  s4 = add_vertex("s4", Type::species, g);
  s5 = add_vertex("s5", Type::species, g);
  c1 = add_vertex("c1", Type::character, g);
  c2 = add_vertex("c2", Type::character, g);
  c3 = add_vertex("c3", Type::character, g);
  c4 = add_vertex("c4", Type::character, g);

  add_edge(s1, c1, g);
  add_edge(s2, c1, g);
  add_edge(s2, c2, g);
  add_edge(s3, c1, g);
  add_edge(s3, c2, g);
  add_edge(s3, c3, g);
  add_edge(s3, c4, g);
  add_edge(s5, c2, g);
  add_edge(s5, c3, g);
  add_edge(s5, c4, g);

  std::list<RBVertex> active_species = get_active_species(g);
  order_by_degree(active_species, g);

  assert(active_species == std::list<RBVertex>({s3, s5, s2, s1, s4}));

  std::cout << "p-active: tests passed" << std::endl;

  return 0;
}
