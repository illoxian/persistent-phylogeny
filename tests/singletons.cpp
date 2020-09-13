#include "../src/rbgraph.hpp"


int main(int argc, const char* argv[]) {
  RBGraph g;
  RBVertex v1, v2, v3, v4;

       add_vertex("v0", Type::species, g);
  v1 = add_vertex("v1", Type::character, g);
  v2 = add_vertex("v2", Type::species, g);
  v3 = add_vertex("v3", Type::species, g);
  v4 = add_vertex("v4", Type::species, g);
       add_vertex("v5", Type::character, g);

  add_edge(v1, v2, g);
  add_edge(v3, v4, g);

  remove_singletons(g);

  assert(num_vertices(g) == 4);

  std::cout << "singletons: tests passed" << std::endl;

  return 0;
}
