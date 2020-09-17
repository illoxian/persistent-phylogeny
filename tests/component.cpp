#include "../src/rbgraph.hpp"

int main(int argc, char* argv[]) {

  RBGraph g;
  RBVertex s, v, u;
  s = add_vertex("s1", Type::species, g);
  v = add_vertex("s2", Type::species, g);
  u = add_vertex("s3", Type::species, g);
  add_vertex("c1", Type::character, g);
  add_vertex("c2", Type::character, g);
  add_vertex("c3", Type::character, g);
  add_vertex("c4", Type::character, g);
  add_vertex("c5", Type::character, g);
  add_vertex("c6", Type::character, g);

  add_edge("s1", "c1", Color::black, g);
  add_edge("s1", "c2", Color::red, g);
  add_edge("s1", "c3", Color::black, g);
  add_edge("s2", "c4", Color::black, g);
  add_edge("s2", "c5", Color::red, g);
  add_edge("s3", "c5", Color::red, g);
  add_edge("s3", "c6", Color::red, g);

  // TESTING species_adj_active_characters()
  auto active_char_set = species_adj_active_characters(s, g);

  assert(active_char_set.size() == 1);
  assert(*active_char_set.begin() == "c2");

  active_char_set = species_adj_active_characters(v, g);

  assert(active_char_set.size() == 1);
  assert(*active_char_set.begin() == "c5");

  active_char_set = species_adj_active_characters(u, g);

  assert(active_char_set.size() == 2);
  assert(*active_char_set.begin() == "c5");
  assert(*++active_char_set.begin() == "c6");

  // TESTING comp_active_characters()
  auto active_chars_comp_set = comp_active_characters(s, g);
  
  assert(active_chars_comp_set.size() == 1);
  assert(*active_chars_comp_set.begin() == "c2");

  active_chars_comp_set = comp_active_characters(v, g);

  assert(active_chars_comp_set.size() == 2);
  assert(*active_chars_comp_set.begin() == "c5");
  assert(*++active_chars_comp_set.begin() == "c6");

  std::cout << "component: tests passed" << std::endl;

  return 0;
}