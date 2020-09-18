#include "../src/rbgraph.hpp"


int main(int argc, const char* argv[]) {
  RBGraph g;
  RBVertex s1, s2, s3, s4,
           c1, c2;

  // test includes characters

  s1 = add_vertex("s1", Type::species, g);
  s2 = add_vertex("s2", Type::species, g);
  s3 = add_vertex("s3", Type::species, g);
  s4 = add_vertex("s4", Type::species, g);
  c1 = add_vertex("c1", Type::character, g);
  c2 = add_vertex("c2", Type::character, g);

  add_edge(s1, c1, g);
  add_edge(s2, c2, g);

  assert(!includes_characters(c1, c2, g));
  add_edge(s3, c2, g);
  add_edge(s3, c1, g);
  assert(!includes_characters(c1, c2, g));
  add_edge(s1, c2, g);
  assert(includes_characters(c2, c1, g));
  assert(!includes_characters(c1, c2, g));
  add_edge(s4, c1, g);
  assert(!includes_characters(c1, c2, g));
  assert(!includes_characters(c2, c1, g));

  // test include species
  clear(g);
  s1 = add_species("s1", g);
  s2 = add_species("s2", g);
  c1 = add_character("c1", g);
  c2 = add_character("c2", g);
  RBVertex c3 = add_character("c3", g);
  
  add_edge(s1, c1, Color::black, g);
  add_edge(s2, c2, Color::black, g);

  assert(!includes_species(s1, s2, g));

  add_edge(s1, c3, Color::red, g);
  add_edge(s2, c3, Color::red, g);

  assert(!includes_species(s1, s2, g));

  add_edge(s1, c2, Color::black, g);

  assert(includes_species(s1, s2, g));
  assert(!includes_species(s2, s1, g));

  add_edge(s2, c1, Color::black, g);
  assert(includes_species(s1, s2, g));
  assert(includes_species(s2, s1, g));


  std::cout << "includes: tests passed" << std::endl;
}
