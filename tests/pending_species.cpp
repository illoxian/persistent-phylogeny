#include "../src/rbgraph.hpp"


int main(int argc, const char* argv[]) {
  RBGraph g;
  RBVertex s1 = add_vertex("s1", Type::species, g);
  add_vertex("c1", Type::character, g);
  add_vertex("c2", Type::character, g);

  assert(!is_pending_species(s1, g));

  add_edge("s1", "c1", Color::black, g);
  assert(is_pending_species(s1, g));

  remove_edge("s1", "c1", g);
  assert(!is_pending_species(s1, g));

  add_edge("s1", "c2", Color::red, g);
  assert(!is_pending_species(s1, g));
  
  add_edge("s1", "c1", Color::black, g);
  assert(!is_pending_species(s1, g));

  std::cout << "pending_species: tests passed" << std::endl;

  return 0;
}
