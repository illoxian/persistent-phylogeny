#include "rbgraph.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>
#include <fstream>

//=============================================================================
// Boost functions (overloading)

void clear(RBGraph& g) {
  g.clear();
  vertex_map(g).clear();
}

void remove_edge(const RBVertex& s, const RBVertex& t, RBGraph& g) {
  if (!exists(s, g) || !exists(t, g))
    throw std::runtime_error("[ERROR] In remove_edge(): source vertex or target vertex does not exist");
  if (!exists(s, t, g))
    throw std::runtime_error("[ERROR] In remove_edge(): edge does not exist");

  boost::remove_edge(s, t, g);
}

void remove_edge(const std::string& s, const std::string& t, RBGraph& g) {
  remove_edge(get_vertex(s, g), get_vertex(t, g), g);
}

void remove_vertex(const RBVertex& v, RBGraph& g) {
  if (!exists(v, g))
    throw std::runtime_error("[ERROR] In remove_vertex(): vertex does not exist");

  // delete v from the map
  vertex_map(g).erase(g[v].name);

  boost::clear_vertex(v, g);
  boost::remove_vertex(v, g);

  if (is_species(v, g))
    num_species(g)--;
  else
    num_characters(g)--;
}


void remove_vertex(const std::string& name, RBGraph& g) {
  remove_vertex(get_vertex(name, g), g);
}


RBVertex add_vertex(const std::string& name, const Type type, RBGraph& g) {
 if (vertex_map(g).find(name) != vertex_map(g).end())
  throw std::runtime_error("[ERROR] In add_vertex(): vertex already exists");

  const RBVertex v = boost::add_vertex(g);

  // insert v in the map
  vertex_map(g)[name] = v;

  g[v].name = name;
  g[v].type = type;

  if (is_species(v, g))
    num_species(g)++;
  else
    num_characters(g)++;

  return v;
}

std::pair<RBEdge, bool> add_edge(const RBVertex& u, const RBVertex& v, const Color color, RBGraph& g) {
  if (!exists(u, g) || !exists(v, g))
    throw std::runtime_error("[ERROR] In add_edge(): source vertex or target vertex does not exist");
  if (exists(u, v, g)) 
    throw std::runtime_error("[ERROR] In add_edge(): edge already exists");
  RBEdge e;
  bool exists;
  std::tie(e, exists) = boost::add_edge(u, v, g);
  g[e].color = color;

  return std::make_pair(e, exists);   
}

std::pair<RBEdge, bool> add_edge(const std::string& source, const std::string& target, Color color, RBGraph& g) {
  return add_edge(get_vertex(source, g), get_vertex(target, g), color, g);
}

RBEdge get_edge(const RBVertex &source, const RBVertex &target, const RBGraph &g) {
  if (!exists(source, g) || !exists(target, g)) 
    throw std::runtime_error("[ERROR] In get_edge(): source vertex or edge vertex does not exist");
  if (!exists(source, target, g)) 
    throw std::runtime_error("[ERROR] In get_edge(): edge does not exist");

  RBEdge e;
  std::tie(e, std::ignore) = boost::edge(source, target, g);
  return e;  
}

//=============================================================================
// General functions

const RBVertex& get_vertex(const std::string& name, const RBGraph& g) {
  if(!exists(name, g))
    throw std::runtime_error("[ERROR] In get_vertex(): vertex does not exist");

  return vertex_map(g).at(name);   
}

bool exists(const RBVertex &source, const RBVertex &target, const RBGraph &g) {
  if (!exists(source, g) || !exists(target, g))
    return false;
  
  RBOutEdgeIter e, e_end;
  std::tie(e, e_end) = out_edges(source, g);
  for (; e != e_end; ++e) {
    if (g[e->m_target].name == g[target].name &&
      g[e->m_target].type == g[target].type)
      return true;
  }
  return false;
}

bool exists(const std::string &source, const std::string &target, const RBGraph &g) {
  if (!exists(source, g) || !exists(target, g))
    return false;
  else
    return exists(get_vertex(source, g), get_vertex(target, g), g);
}

bool exists(const RBVertex &v, const RBGraph &g) {
  auto vertices = g.m_vertices;
  return std::find(vertices.begin(), vertices.end(), v) != vertices.end();
}

bool exists(const std::string &name, const RBGraph &g) {
  if (vertex_map(g).find(name) != vertex_map(g).end())
    return true;
  return false;
}

void build_vertex_map(RBGraph& g) {
  vertex_map(g).clear();

  RBVertexIter v, v_end;
  std::tie(v, v_end) = vertices(g);
  for (; v != v_end; ++v) {
    vertex_map(g)[g[*v].name] = *v;
  }
}

void copy_graph(const RBGraph& g, RBGraph& g_copy) {
  RBVertexIMap index_map;
  RBVertexIAssocMap index_assocmap(index_map);

  // fill the vertex index map index_assocmap
  RBVertexIter u, u_end;
  std::tie(u, u_end) = vertices(g);
  for (size_t index = 0; u != u_end; ++u, ++index) {
    boost::put(index_assocmap, *u, index);
  }

  // copy g to g_copy
  copy_graph(g, g_copy, boost::vertex_index_map(index_assocmap));

  // update g_copy's number of species and characters
  num_species(g_copy) = num_species(g);
  num_characters(g_copy) = num_characters(g);

  // rebuild g_copy's map
  build_vertex_map(g_copy);
}

void copy_graph(const RBGraph& g, RBGraph& g_copy, RBVertexMap& v_map) {
  RBVertexIMap index_map;
  RBVertexAssocMap v_assocmap(v_map);
  RBVertexIAssocMap index_assocmap(index_map);

  // fill the vertex index map index_assocmap
  RBVertexIter u, u_end;
  std::tie(u, u_end) = vertices(g);
  for (size_t index = 0; u != u_end; ++u, ++index) {
    boost::put(index_assocmap, *u, index);
  }

  // copy g to g_copy, fill the vertex map v_assocmap (and v_map)
  copy_graph(g, g_copy,
             boost::vertex_index_map(index_assocmap).orig_to_copy(v_assocmap));

  // update g_copy's number of species and characters
  num_species(g_copy) = num_species(g);
  num_characters(g_copy) = num_characters(g);

  // rebuild g_copy's map
  build_vertex_map(g_copy);
}

std::ostream& operator<<(std::ostream& os, const RBGraph& g) {
  std::list<std::string> lines;
  std::list<std::string> species;
  std::list<std::string> characters;

  RBVertexIter v, v_end;
  std::tie(v, v_end) = vertices(g);
  for (size_t i = 0; v != v_end; ++v, ++i) {
    std::list<std::string> edges;

    RBOutEdgeIter e, e_end;
    std::tie(e, e_end) = out_edges(*v, g);
    for (size_t j = 0; e != e_end; ++e, ++j) {
      std::string edge;
      edge += " -";
      edge += (is_red(*e, g) ? "r" : "-");
      edge += "- ";
      edge += g[target(*e, g)].name;
      edge += ";";

      edges.push_back(edge);
    }

    auto compare_edges = [](const std::string& a, const std::string& b) {
      size_t a_index, b_index;
      std::stringstream ss;

      ss.str(a.substr(6, a.size() - 7));
      ss >> a_index;

      ss.clear();

      ss.str(b.substr(6, b.size() - 7));
      ss >> b_index;

      return a_index < b_index;
    };

    edges.sort(compare_edges);

    std::string edges_str;
    for (const auto& edge : edges) {
      edges_str.append(edge);
    }

    auto line(g[*v].name + ":" + edges_str);

    if (std::next(v) != v_end) line += "\n";

    if (is_species(*v, g))
      species.push_back(line);
    else
      characters.push_back(line);
  }

  auto compare_lines = [](const std::string& a, const std::string& b) {
    size_t a_index, b_index;
    std::stringstream ss;

    ss.str(a.substr(1, a.find(":")));
    ss >> a_index;

    ss.clear();

    ss.str(b.substr(1, b.find(":")));
    ss >> b_index;

    return a_index < b_index;
  };

  species.sort(compare_lines);
  characters.sort(compare_lines);

  lines.splice(lines.end(), species);
  lines.splice(lines.end(), characters);

  std::string lines_str;
  for (const auto& line : lines) {
    lines_str += line;
  }

  os << lines_str;

  return os;
}

// File I/O

void read_graph(const std::string& filename, RBGraph& g) {
  std::vector<RBVertex> species, characters;
  bool first_line = true;
  std::string line;
  std::ifstream file(filename);
  std::vector<std::string> a_chars;

  if (!file) {
    // input file doesn't exist
    throw std::runtime_error(
        "[ERROR] Failed to read graph from file: no such file or directory");
  }

  size_t index = 0;
  while (std::getline(file, line)) {
    // for each line in file
    std::istringstream iss(line);

    if (first_line) {
      size_t cont = 0;
      size_t read;
      size_t num_s, num_c;

      while(iss >> read) {
        if(cont == 0){
          num_s = read;
          cont++;
        }
        else if(cont == 1) {
          num_c = read;
          cont++;
        }
        else {
          if(read >= num_c)
            throw std::runtime_error("[ERROR] Failed to read graph from file: Inexistent character");
          std::string s = "c" + std::to_string(read);
          a_chars.push_back(s);
        }
      }

      species.resize(num_s);
      characters.resize(num_c);

      if (species.size() == 0 || characters.size() == 0) {
        // input file parsing error
        throw std::runtime_error(
            "[ERROR] Failed to read graph from file: badly formatted line 0");
      }

      // insert species in the graph
      for (size_t j = 0; j < species.size(); ++j) {
        const auto v_name = "s" + std::to_string(j);

        species[j] = add_species(v_name, g);
      }

      // insert characters in the graph
      for (size_t j = 0; j < characters.size(); ++j) {
        const auto v_name = "c" + std::to_string(j);

        characters[j] = add_character(v_name, g);
      }

      first_line = false;
    } 
    else {
      char value;

      // read binary matrix
      while (iss >> value) {
        switch (value) {
          case '1':
            // add edge between species[s_index] and characters[c_index]
            {
              const auto s_index = index / characters.size(),
                         c_index = index % characters.size();

              if (s_index >= species.size() || c_index >= characters.size()) {
                // input file parsing error
                throw std::runtime_error(
                    "[ERROR] Failed to read graph from file: oversized matrix");
              }

              add_edge(species[s_index], characters[c_index], g);
            }
            break;

          case '0':
            // ignore
            break;

          default:
            // input file parsing error
            throw std::runtime_error(
                "[ERROR] Failed to read graph from file: unexpected value in matrix");
        }

        index++;
      }
    }
  }

  

  if (index != species.size() * characters.size()) {
    // input file parsing error
    throw std::runtime_error(
        "[ERROR] Failed to read graph from file: undersized matrix");
  }

  if (species.size() == 0 || characters.size() == 0) {
    // input file parsing error
    throw std::runtime_error("[ERROR] Failed to read graph from file: empty file");
  }

  for(const auto& elem : a_chars)
    change_char_type(vertex_map(g).at(elem), g);
}

//=============================================================================
// Algorithm functions

bool is_active(const RBVertex& v, const RBGraph& g) {

  bool is_char = is_character(v, g);
  // an active character has only incident red edges, while
  // an active species has no incident red edges

  RBOutEdgeIter e, e_end;
  std::tie(e, e_end) = out_edges(v, g);
  for (; e != e_end; ++e)
    if (is_char && !is_red(*e, g))
      return false;
    else if (!is_char && is_red(*e, g))
      return false;

  return true;
}

bool is_quasi_active(const RBVertex& s, const RBGraph& g) {
  // TODO
  return false;
}

bool is_pending_species(const RBVertex& s, const RBGraph& g) {
  if (!is_species(s, g))
    return false;
  
  RBOutEdgeIter e, e_end;
  std::tie(e, e_end) = out_edges(s, g);
  std::list<RBEdge> edge_list(e, e_end);
  
  if (edge_list.size() != 1)
    return false;

  if (is_black(*edge_list.begin(), g))
    return true;
  else
    return false;
}

void remove_singletons(RBGraph& g) {
  RBVertexIter v, v_end, next;
  std::tie(v, v_end) = vertices(g);
  for (next = v; v != v_end; v = next) {
    next++;
    remove_vertex_if(*v, if_singleton(), g);
  }
}

bool is_red_universal(const RBVertex& v, const RBGraph& g) {
  if (!is_character(v, g)) 
    return false;

  RBVertexIMap index_map, comp_map;
  RBVertexIAssocMap index_assocmap(index_map), comp_assocmap(comp_map);
  
  // fill vertex index map
  RBVertexIter u, u_end;
  std::tie(u, u_end) = vertices(g);
  for (size_t index = 0; u != u_end; ++u, ++index) {
    boost::put(index_assocmap, *u, index);
  }

  // build the components map
  boost::connected_components(g, comp_assocmap,
                              boost::vertex_index_map(index_assocmap));

  return is_red_universal(v, g, comp_map);
}

bool is_red_universal(const RBVertex& v, const RBGraph& g, const RBVertexIMap& c_map) {
  if (!is_character(v, g)) 
    return false;

  size_t tot_species = 0;
  RBVertexIter u, u_end;
  std::tie(u, u_end) = vertices(g);
  for (; u != u_end; ++u) {
    // if it is not a species or it is not located in the component of v
    if (!is_species(*u, g) || c_map.at(v) != c_map.at(*u)) {
      continue;
    }
    tot_species++;
  }

  size_t count_species = 0;
  RBOutEdgeIter e, e_end;
  std::tie(e, e_end) = out_edges(v, g);
  for (; e != e_end; ++e) {
    if (!is_red(*e, g)) 
      return false;
    count_species++;
  }

  if (count_species != tot_species) 
    return false;

  return true;
}

bool is_universal(const RBVertex v, const RBGraph& g) {
  if (!is_character(v, g)) 
    return false;

  RBVertexIMap index_map, comp_map;
  RBVertexIAssocMap index_assocmap(index_map), comp_assocmap(comp_map);

  // fill vertex index map
  RBVertexIter u, u_end;
  std::tie(u, u_end) = vertices(g);
  for (size_t index = 0; u != u_end; ++u, ++index) {
    boost::put(index_assocmap, *u, index);
  }

  // build the components map
  boost::connected_components(g, comp_assocmap,
                              boost::vertex_index_map(index_assocmap));

  return is_universal(v, g, comp_map);
}

bool is_universal(const RBVertex v, const RBGraph& g,
                  const RBVertexIMap& c_map) {
  if (!is_character(v, g)) 
    return false;

  size_t tot_species = 0;
  RBVertexIter u, u_end;
  std::tie(u, u_end) = vertices(g);
  for (; u != u_end; ++u) {
    if (!is_species(*u, g) || c_map.at(v) != c_map.at(*u)) 
      continue;
    tot_species++;
  }

  size_t count_species = 0;
  RBOutEdgeIter e, e_end;
  std::tie(e, e_end) = out_edges(v, g);
  for (; e != e_end; ++e) {
    if (!is_black(*e, g)) 
      return false;
    count_species++;
  }

  if (count_species != tot_species) 
    return false;

  return true;
}

RBGraphVector connected_components(const RBGraph& g) {
  RBVertexIMap index_map, comp_map;
  RBVertexIAssocMap index_assocmap(index_map), comp_assocmap(comp_map);

  // fill the vertex index map index_assocmap
  RBVertexIter v, v_end;
  std::tie(v, v_end) = vertices(g);
  for (size_t index = 0; v != v_end; ++v, ++index) {
    boost::put(index_assocmap, *v, index);
  }

  // get number of components and the components map
  size_t comp_count = boost::connected_components(
      g, comp_assocmap, boost::vertex_index_map(index_assocmap));

  // how comp_map is structured (after running boost::connected_components):
  // comp_map[i] => < vertex_in_g, component_index >
  return connected_components(g, comp_map, comp_count);
}

RBGraphVector connected_components(const RBGraph& g, const RBVertexIMap& c_map,
                                   const size_t c_count) {
  RBGraphVector components;
  RBVertexMap vertices;
  
  // how vertices is going to be structured:
  // vertices[vertex_in_g] => vertex_in_component

  // resize subgraph components
  components.resize(c_count);

  // initialize subgraph components
  for (size_t i = 0; i < c_count; ++i) {
    components[i] = std::make_unique<RBGraph>();
  }
  
  if (c_count <= 1) {
    // graph is connected
    return components;
  }
  // else graph is disconnected

  // add vertices to their respective subgraph
  for (const auto& vcomp : c_map) {
    // for each vertex
    const auto v = vcomp.first;
    const auto comp = vcomp.second;
    auto* const component = components[comp].get();

    // add the vertex to *component and copy its descriptor in vertices[v]
    vertices[v] = add_vertex(g[v].name, g[v].type, *component);
  }

  // add edges to their respective vertices and subgraph
  for (const auto& vcomp : c_map) {
    // for each vertex
    const auto v = vcomp.first;

    // prevent duplicate edges from characters to species
    if (!is_species(v, g)) 
      continue;

    const auto new_v = vertices[v];
    const auto comp = vcomp.second;
    auto* const component = components[comp].get();

    RBOutEdgeIter e, e_end;
    std::tie(e, e_end) = out_edges(v, g);
    for (; e != e_end; ++e) {
      // for each out edge
      const auto new_vt = vertices[target(*e, g)];

      // prevent duplicate edges on non-bipartite graphs
      if (exists(new_v, new_vt, *component)) 
        continue;

      add_edge(new_v, new_vt, g[*e].color, *component);
    }
  }

  if (logging::enabled) {
    
    if (c_count == 1) {
      std::cout << "G connected" << std::endl;
    } else {
      std::cout << "Connected components: " << c_count << std::endl;

      for (const auto& component : components) {
        std::cout << *component.get() << std::endl << std::endl;
      }
    }
  }

  return components;
}

std::map<RBVertex, std::list<RBVertex>> get_neighbours_map(const RBGraph& g) {
  std::map<RBVertex, std::list<RBVertex>> neighbours_map;
  // neighbours_map will be structured as follows:
  // neighbours_map[v] = list of verteces adjacent to vertex v

  // fill the map
  RBVertexIter v, v_end;
  std::tie(v, v_end) = vertices(g);
  for (; v != v_end; ++v) {
    RBOutEdgeIter e, e_end;
    std::tie(e, e_end) = out_edges(*v, g);
    for (; e != e_end; ++e) {
      RBVertex vt = target(*e, g);
      neighbours_map[*v].push_back(vt);
    }
  }
  return neighbours_map;
}

std::map<RBVertex, std::list<RBVertex>> get_adjacent_species_map(const RBGraph& g) {
  std::map<RBVertex, std::list<RBVertex>> adj_spec = get_neighbours_map(g);

  // remove from the map all the keys that refer to species
  RBVertexIter v, v_end;
  std::tie(v, v_end) = vertices(g);
  for (; v != v_end; ++v) {
    if (!is_character(*v, g)) 
      adj_spec.erase(*v);
  }
  return adj_spec;
}

std::map<RBVertex, std::list<RBVertex>> get_adjacent_characters_map(const RBGraph& g) {
    std::map<RBVertex, std::list<RBVertex>> adj_chars = get_neighbours_map(g);

  // remove from the map all the keys that refer to characters
  RBVertexIter v, v_end;
  std::tie(v, v_end) = vertices(g);
  for (; v != v_end; ++v) {
    if (!is_species(*v, g)) 
      adj_chars.erase(*v);
  }
  return adj_chars;
}

bool includes(const RBVertex& c1, const RBVertex& c2, std::map<RBVertex, std::list<RBVertex>>& adj_spec) {
  bool included = true;
  RBVertexIter c2_it = adj_spec[c2].begin(), c2_it_end = adj_spec[c2].end();
  while (c2_it != c2_it_end) {
    if (!contains(adj_spec[c1], *c2_it)) {
      included = false;
      break;
    }
    ++c2_it;
  }
  return included;
}

bool overlap(const RBVertex& c1, const RBVertex& c2, std::map<RBVertex, std::list<RBVertex>>& adj_spec) {
  if (includes(c1, c2, adj_spec) || includes(c2, c1, adj_spec))
    return false;
  RBVertexIter c2_it = adj_spec[c2].begin(), c2_it_end = adj_spec[c2].end();
  while (c2_it != c2_it_end) {
    if (contains(adj_spec[c1], *c2_it))
      return true;
    ++c2_it;
  }
  return false;
}

std::list<RBVertex> get_inactive_chars(const RBGraph& g) {
  std::list<RBVertex> list_result;

  RBVertexIter v, v_end;
  std::tie(v, v_end) = vertices(g);
  while (v != v_end) {
    if (is_character(*v, g) && is_inactive(*v, g))
      list_result.push_back(*v);
    ++v;
  }

  return list_result;
}

const std::list<RBVertex> maximal_characters(const RBGraph& g) {
  std::list<RBVertex> cm;
  auto adj_spec = get_adjacent_species_map(g);
  // adj_spec is a map, where adj_spec[*v] contains the list of species adjacent to v

  std::list<RBVertex> inactive_chars = get_inactive_chars(g);

  // for each inactive character c in g, if S(c) ⊄ S(c') for any
  // character c', then v is a maximal character and it
  // is inserted in cm
  RBVertexIter v = inactive_chars.begin(), v_end = inactive_chars.end();
  bool maximal_character;
  for (; v != v_end; ++v) {
    maximal_character = true;

    RBVertexIter u = inactive_chars.begin(), u_end = inactive_chars.end();
    for (; u != u_end; ++u) {
      if (*v == *u)
        continue;
      
      if (includes(*u, *v, adj_spec)) {
        maximal_character = false;
        break;
      }
    }

    if (maximal_character)
      cm.push_back(*v);
  }
  return cm;
}

RBGraph maximal_reducible_graph(const RBGraph& g, const bool active) {
  // copy g to gm
  RBGraph gm;
  copy_graph(g, gm);

  // compute the maximal characters of gm
  const auto cm = maximal_characters(gm);

  if (logging::enabled) {
    // verbosity enabled
    std::cout << "Maximal characters Cm = { ";

    for (const auto& kk : cm) {
      std::cout << gm[kk].name << " ";
    }

    std::cout << "} - Count: " << cm.size() << std::endl;
  }

  // remove non-maximal characters of gm
  RBVertexIter v, v_end, next;
  std::tie(v, v_end) = vertices(gm);
  for (next = v; v != v_end; v = next) {
    next++;

    if (!is_character(*v, gm))
      // don't remove non-character vertices
      continue;

    if (active && is_active(*v, gm))
      // don't remove active or non-character vertices
      continue;

    remove_vertex_if(*v, if_not_maximal(cm), gm);
  }

  remove_singletons(gm);

  return gm;
}

bool has_red_sigmagraph(const RBGraph& g) {
  size_t count_actives = 0;

  RBVertexIter v, v_end;
  std::tie(v, v_end) = vertices(g);
  for (; v != v_end; ++v) {
    if (is_active(*v, g)) count_actives++;

    if (count_actives == 2) break;
  }

  // if count_actives doesn't reach 2, g can't contain a red sigma-graph
  if (count_actives < 2) return false;

  std::tie(v, v_end) = vertices(g);
  for (; v != v_end; ++v) {
    if (!is_active(*v, g)) continue;
    // for each active character

    for (auto u = std::next(v); u != v_end; ++u) {
      if (!is_active(*u, g)) continue;
      // for each active character coming after *v

      // check if characters *v and *u are part of a red sigma-graph
      if (has_red_sigmapath(*v, *u, g)) return true;
    }
  }

  return false;
}

bool has_red_sigmapath(const RBVertex c0, const RBVertex c1, const RBGraph& g) {
  // vertex that connects c0 and c1 (always with red edges)
  RBVertex junction = 0;

  bool half_sigma = false;

  RBOutEdgeIter e, e_end;
  std::tie(e, e_end) = out_edges(c0, g);
  for (; e != e_end; ++e) {
    if (!is_red(*e, g)) continue;
    // for each species connected to c0 via a red edge

    RBVertex s = target(*e, g);
    
    // check if s can be a junction vertex
    if (junction == 0 && exists(c1, s, g) && is_red(get_edge(c1, s, g), g)) {
      junction = s;

      continue;
    }

    // check if s and c1 are connected
    if (exists(c1, s, g))
      // skip s
      continue;

    half_sigma = true;

    if (junction != 0) break;
  }

  if (!half_sigma || junction == 0) return false;

  std::tie(e, e_end) = out_edges(c1, g);
  for (; e != e_end; ++e) {
    RBVertex s = target(*e, g);

    if (!is_red(*e, g) || s == junction) 
    continue;
    // for each species connected to c1 via a red edge (which is not junction)

    // check if s and c0 are connected
    if (exists(c0, s, g))
      // skip s
      continue;

    return true;
  }

  return false;
}

void change_char_type(const RBVertex& v, RBGraph& g) {
  // get the black edges in v
  RBOutEdgeIter e, e_end;
  std::tie(e, e_end) = out_edges(v, g);
  std::list<RBVertex> species_black_edges;
  for (; e != e_end; ++e) 
    species_black_edges.push_back((*e).m_target);

  // get the species not connected to v
  RBVertexIter u, u_end;
  std::list<RBVertex> species_red_edges;
  std::tie(u, u_end) = vertices(g);
  while (u != u_end) {
    if (is_species(*u, g) && !contains(species_black_edges, *u))
      species_red_edges.push_back(*u);
    ++u;
  }

  boost::clear_vertex(v, g);

  u = species_red_edges.begin();
  u_end = species_red_edges.end();
  while (u != u_end) {
    add_edge(v, *u, Color::red, g);
    ++u;
  }
}

std::set<std::string> get_active_characters(const RBGraph& g) {
  std::set<std::string> ac;
  RBVertexIter v, v_end;
  
  std::tie(v, v_end) = vertices(g);
  while(v != v_end) {
    if(is_active(*v, g))
      ac.insert(g[*v].name);
    v++;
  }
  return ac;
}

std::list<std::string> get_comp_vertex(const RBVertex& u, const RBGraph& g) {

  std::list<std::string> result;
  RBGraphVector connected_comp_vec = connected_components(g);

  if (connected_comp_vec.size() == 1) {
    // only a connected component, then return all the species of g
    for (RBVertex v : g.m_vertices)
        if (g[u].type != g[v].type)
          result.push_back(g[v].name);
  } else {
    // more than a component => then find the component in which c is located and
    // return the species included in such component
    auto comp = connected_comp_vec.begin();
    auto comp_end = connected_comp_vec.end();
    for (; comp != comp_end; ++comp)
      if (exists(g[u].name, *comp->get())) { 
        for (RBVertex v : comp->get()->m_vertices)
          if (g[u].type != (*comp->get())[v].type)
            result.push_back((*comp->get())[v].name);
        break;
      }
  }
  return result;
}

std::set<std::string> get_species_adj_active_characters(const RBVertex s, const RBGraph& g) {
  std::set<std::string> adj_active_chars;
  std::list<RBVertex> adj_chars = get_adjacent_characters_map(g)[s];

  for (RBVertex c : adj_chars)
    if (is_active(c, g))
      adj_active_chars.insert(g[c].name);
  return adj_active_chars;
}

std::set<std::string> get_comp_active_characters(const RBVertex s, const RBGraph& g) {
  std::set<std::string> result;
  std::list<std::string> char_str_list = get_comp_vertex(s, g);

  for (std::string c_str : char_str_list)
    if (is_active(get_vertex(c_str, g), g))
      result.insert(c_str);

  return result;
}