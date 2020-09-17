#include "../src/rbgraph.hpp"
#include "../src/functions.hpp"


int main(int argc, const char* argv[]) {
  RBGraph g;
  RBVertex s1, s2, s3, s4, s5,
           c1, c2, c3, c4, c5;

  s1 = add_vertex("s1", Type::species, g);
  s2 = add_vertex("s2", Type::species, g);
  s3 = add_vertex("s3", Type::species, g);
  s4 = add_vertex("s4", Type::species, g);
  c1 = add_vertex("c1", Type::character, g);
  c2 = add_vertex("c2", Type::character, g);
  c3 = add_vertex("c3", Type::character, g);
  c4 = add_vertex("c4", Type::character, g);
  c5 = add_vertex("c5", Type::character, g);

  add_edge(c1, s1, g);
  add_edge(c1, s2, g);
  add_edge(c1, s3, g);
  add_edge(c1, s4, g);
  add_edge(c2, s1, g);
  add_edge(c2, s2, g);
  add_edge(c3, s1, Color::red, g);
  add_edge(c3, s2, Color::red, g);
  add_edge(c3, s3, Color::red, g);
  add_edge(c3, s4, Color::red, g);
  add_edge(c4, s3, g);
  add_edge(c4, s4, g);
  add_edge(c5, s4, Color::red, g);

  realize_character(c1, g);
  assert(!exists(c1, g));

  realize_character(c3, g);
  assert(!exists(c3, g));

  realize_character(c5, g);
  assert(exists(c5, g));
  assert(exists(c5, s4, g));

  realize_character(c4, g);
  assert(!exists(c4, s3, g));
  assert(!exists(c4, s4, g));
  assert(!exists(c4, g));
  assert(!exists(s3, g));

  realize_character(c5, g);
  assert(!exists(c5, g));
  assert(!exists(s4, g));

  realize_character(c2, g);
  assert(!exists(c2, g));
  assert(!exists(s2, g));
  assert(!exists(s1, g));

  assert(g.m_vertices.empty());
  assert(g.m_edges.empty());

  std::cout << "realize_character: tests passed" << std::endl;

  return 0;
}
