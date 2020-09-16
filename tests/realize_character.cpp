#include "../src/rbgraph.hpp"
#include "../src/functions.hpp"


int main(int argc, const char* argv[]) {
  RBGraph g;
  RBVertex s2, s3, s4, s5, s6,
           c1, c2, c3, c4, c5, c6, c7;

  s2 = add_vertex("s2", Type::species, g);
  s3 = add_vertex("s3", Type::species, g);
  s4 = add_vertex("s4", Type::species, g);
  s5 = add_vertex("s5", Type::species, g);
  s6 = add_vertex("s6", Type::species, g);
  c1 = add_vertex("c1", Type::character, g);
  c2 = add_vertex("c2", Type::character, g);
  c3 = add_vertex("c3", Type::character, g);
  c4 = add_vertex("c4", Type::character, g);
  c5 = add_vertex("c5", Type::character, g);
  c6 = add_vertex("c6", Type::character, g);
  c7 = add_vertex("c7", Type::character, g);

  add_edge(s2, c3, g);
  add_edge(s2, c5, g);
  add_edge(s2, c6, g);
  add_edge(s3, c2, g);
  add_edge(s3, c3, g);
  add_edge(s3, c4, Color::red, g);
  add_edge(s4, c1, g);
  add_edge(s4, c2, g);
  add_edge(s4, c4, Color::red, g);
  add_edge(s5, c1, g);
  add_edge(s5, c2, g);
  add_edge(s5, c3, g);
  add_edge(s5, c4, Color::red, g);
  add_edge(s5, c5, g);
  add_edge(s5, c7, g);
  add_edge(s6, c2, g);
  add_edge(s6, c3, g);
  add_edge(s6, c5, g);

  realize_character(c4, g);
  assert(!exists(c4, g));

  realize_character(c1, g);
  assert(!exists(c1, s4, g));
  assert(!exists(c1, s5, g));
  assert(exists(c1, s2, g));
  assert(g[get_edge(c1, s2, g)].color == Color::red);
  assert(exists(c1, s3, g));
  assert(g[get_edge(c1, s3, g)].color == Color::red);
  assert(exists(c1, s6, g));
  assert(g[get_edge(c1, s3, g)].color == Color::red);

  realize_character(c1, g);
  assert(!exists(c1, g));

  realize_character(c2, g);
  assert(!exists(c2, s3, g));
  assert(!exists(c2, s4, g));
  assert(!exists(c2, s5, g));
  assert(!exists(c2, s6, g));
  assert(exists(c2, s2, g));
  assert(g[get_edge(c2, s2, g)].color == Color::red);

  realize_character(c3, g);
  assert(!exists(c3, g));

  realize_character(c5, g);
  assert(!exists(c5, g));

  realize_character(c2, g);
  assert(!exists(c2, g));

  realize_character(c6, g);
  assert(!exists(c6, g));
  assert(!exists(s2, g));

  realize_character(c7, g);
  assert(!exists(c7, g));
  assert(!exists(s5, g));

  assert(g.m_vertices.empty());
  assert(g.m_edges.empty());

  std::cout << "realize_character: tests passed" << std::endl;

  return 0;
}
