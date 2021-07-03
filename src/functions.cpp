/** 
 * 
 * @file functions.cpp
 * 
 * @author Simone Paolo Mottadelli
 * 
 */

#include "functions.hpp"
#include "rbgraph.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/depth_first_search.hpp>

std::pair<std::list<SignedCharacter>, bool>
realize_character(const SignedCharacter &sc, RBGraph &g) {
    std::list<SignedCharacter> output;

    // current character vertex
    RBVertex cv = 0;

    // get the vertex in g whose name is sc.character
    try {
        cv = get_vertex(sc.character, g);
    } catch (const std::out_of_range &e) {
        // g has no vertex named sc.character
        return std::make_pair(output, false);
    }

    RBVertexIMap i_map, c_map;
    RBVertexIAssocMap i_assocmap(i_map), c_assocmap(c_map);

    // fill vertex index map
    RBVertexIter v, v_end;
    std::tie(v, v_end) = vertices(g);
    for (size_t index = 0; v != v_end; ++v, ++index) {
        boost::put(i_assocmap, *v, index);
    }

    // build the components map
    boost::connected_components(g, c_assocmap,
                                boost::vertex_index_map(i_assocmap));

    if (sc.state == State::gain && is_inactive(cv, g)) {
        // c+ and c is inactive
        if (logging::enabled) {
            // verbosity enabled
            //std::cout << "Realizing " << sc;
        }
        // realize the character c+:
        // - add a red edge between c and each species in D(c) \ N(c)
        // - delete all black edges incident on c
        std::tie(v, v_end) = vertices(g);
        for (; v != v_end; ++v) {
            if (!is_species(*v, g) || c_map.at(*v) != c_map.at(cv)) {
                continue;
            }
            // for each species in the same connected component of cv

            RBEdge e;
            bool exists;
            std::tie(e, exists) = edge(*v, cv, g);

            if (exists) {
                // there is an edge (black) between *v and cv
                remove_edge(e, g);
            } else {
                // there isn't an edge between *v and cv
                add_edge(*v, cv, Color::red, g);
            }
        }

        if (logging::enabled) {
            // verbosity enabled
            //std::cout << std::endl;
        }
    } else if (sc.state == State::lose && is_active(cv, g)) {
        // c- and c is active
        if (logging::enabled) {
            // verbosity enabled
            //std::cout << "Realizing " << sc << std::endl;
        }

        // realize the character c- if it is connected through red edges to all the species of the component in which c resides. In this case:
        // - delete all edges incident on c
        bool connected = true;
        std::tie(v, v_end) = vertices(g);
        for (; v != v_end; ++v) {
            if (!is_species(*v, g) || c_map.at(*v) != c_map.at(cv)) {
                continue;
            }

            // if cv is not connected to the species v, which belongs to its same component, then cv is not connected to all the species of its component.
            if (!exists(*v, cv, g)) {
                connected = false;
                break;
            }
        }

        if (connected) {
            clear_vertex(cv, g);
        } else {
            if (logging::enabled) {
                // verbosity enabled
                //std::cout << "Could not realize " << sc << std::endl;
            }
            return std::make_pair(output, false);
        }
    } else {
        if (logging::enabled) {
            // verbosity enabled
            //std::cout << "Could not realize " << sc << std::endl;
        }

        // this should never happen during the algorithm, but it is handled just in
        // case something breaks (or user input happens)
        return std::make_pair(output, false);
    }

    output.push_back(sc);

    // delete all isolated vertices
    remove_singletons(g);

    return std::make_pair(output, true);
}

std::pair<std::list<SignedCharacter>, bool> realize_species(const RBVertex v,
                                                            RBGraph &g) {
    std::list<SignedCharacter> lsc;

    if (!is_species(v, g)) {
        return std::make_pair(lsc, false);
    }

    // build the list of inactive characters adjacent to v (species)

    std::list<RBVertex> adjacent_chars = get_adj_vertices(v, g);

    for (RBVertex c : adjacent_chars) {
        if (is_inactive(c, g)) {
            lsc.push_back({g[c].name, State::gain});
        }
    }

    return realize(lsc, g);
}

std::pair<std::list<SignedCharacter>, bool>
realize(const std::list<SignedCharacter> &lsc, RBGraph &g) {
    std::list<SignedCharacter> output;

    // realize the list of signed characters lsc; the algorithm stops when a
    // non-feasible realization is encountered, setting the boolean flag to false
    // TODO: maybe change this behaviour
    for (const SignedCharacter i : lsc) {
        if (std::find(output.cbegin(), output.cend(), i) != output.cend()) {
            // the signed character i has already been realized in a previous sc
            continue;
        }

        std::list<SignedCharacter> sc;
        bool feasible;

        std::tie(sc, feasible) = realize_character(i, g);

        if (!feasible) {
            return std::make_pair(sc, false);
        }

        output.splice(output.cend(), sc);
    }

    return std::make_pair(output, true);
}

bool is_complete(std::list<SignedCharacter> sc, const RBGraph &gm) {
    RBVertexIter v, v_end;
    auto scb = sc.begin();
    auto sce = sc.end();
    std::tie(v, v_end) = vertices(gm);
    while (v != v_end) {
        if (is_inactive(*v, gm)) {
            while (scb != sce) {
                if ((get_vertex(scb->character, gm) == *v)) {
                    return false;
                }
                scb++;
            }
        }
        v++;
    }
    return true;
}

void sort_by_degree(std::list<RBVertex> &list_to_sort, const RBGraph &g) {
    // constructing a list of pairs <RBVertex, int>, where the first element is a vertex and the second element is the degree of the vertex
    std::list<std::pair<RBVertex, int>> list_of_pairs;

    for (RBVertex v : list_to_sort) {
        list_of_pairs.push_back(std::make_pair(v, out_degree(v, g)));
    }

    auto comparator = [](const std::pair<RBVertex, int> &a,
                         const std::pair<RBVertex, int> &b) {
        return a.second > b.second;
    };

    list_of_pairs.sort(comparator);

    list_to_sort.clear();

    for (std::pair<RBVertex, int> pair : list_of_pairs) {
        list_to_sort.push_back(pair.first);
    }
}

std::list<RBVertex> get_all_minimal_p_active_species(const RBGraph &g,
                                                     bool all) {
    std::list<RBVertex> out;
    std::list<RBVertex> active_species = get_active_species(g);
    sort_by_degree(active_species, g);

    bool found;
    int num_inctv_chars_v, num_inctv_chars_u;
    for (RBVertex v : active_species) {
        found = false;
        // for every active species v in the sorted list of active species
        for (int i = 1; i < num_characters(g); ++i) {
            // index "i" is used after to check if vertex "u" has "i" more inactive characters than "v"

            for (RBVertex u : get_neighbors(v, g)) {
                if (u == v || is_character(u, g)) {
                    continue;
                }
                // for every species u (neighbor of v)

                if (includes_species(u, v, g)) {
                    // if u includes all the inactive species of v, then check if u has "i" species more than v
                    num_inctv_chars_v =
                        get_adj_inactive_characters(v, g).size();
                    num_inctv_chars_u =
                        get_adj_inactive_characters(u, g).size();
                    if (num_inctv_chars_u == num_inctv_chars_v + i) {

                        // check if the realization of v and then of u can generate any red-sigmagraphs in g
                        RBGraph g_copy;
                        copy_graph(g, g_copy);

                        realize_species(get_vertex(g[v].name, g_copy), g_copy);
                        realize_species(get_vertex(g[u].name, g_copy), g_copy);
                        if (!has_red_sigmagraph(g_copy)) {
                            out.push_back(v);
                            found = true;
                            break;
                        }
                    }
                }
            }
            if (found) {
                break;
            }
        }
        if (found && !all) {
            break;
        }
    }
    return out;
}

RBVertex get_minimal_p_active_species(const RBGraph &g) {
    return *get_all_minimal_p_active_species(g, false).begin();
}

RBVertex get_quasi_active_species(const RBGraph &g) {
    for (RBVertex v : g.m_vertices) {
        if (!is_species(v, g)) {
            continue;
        }
        RBOutEdgeIter e, e_end;
        std::tie(e, e_end) = out_edges(v, g);
        int black_edges = 0;
        int red_edges = 0;
        for (; e != e_end; ++e) {
            if (is_black(*e, g)) {
                ++black_edges;
            } else {
                ++red_edges;
            }
        }
        // if we are here then s is a species with some red incoming edges. So, we have to check whether its realization generates a red-sigmagraph
        if (black_edges > 0 && red_edges > 0) {
            RBGraph g_copy;
            copy_graph(g, g_copy);
            realize_species(get_vertex(g[v].name, g_copy), g_copy);
            if (!has_red_sigmagraph(g_copy)) {
                return v;
            }
        }
    }
    return nullptr;
}

std::list<SignedCharacter> ppp_maximal_reducible_graphs(RBGraph &g) {

    std::list<SignedCharacter> realized_chars =
        realize_red_univ_and_univ_chars(g).first;
    remove_duplicate_species(g);
    std::list<SignedCharacter> tmp;

    while (!is_empty(g)) {
        std::cout << "PRINT G " << std::endl << g << std::endl;
        if (get_pending_species(g).size() == 1) {
            tmp = realize_species(*get_pending_species(g).begin(), g).first;
        } else if (get_minimal_p_active_species(g) != 0) {
            tmp = realize_species(get_minimal_p_active_species(g), g).first;
        } else if (is_degenerate(g)) {
            for (RBVertex c : get_inactive_chars(g)) {
                tmp.splice(
                    tmp.end(),
                    realize_character({g[c].name, State::gain}, g).first);
            }
        } else if (get_active_species(g).size() == 1) {
            tmp = realize_species(*get_active_species(g).begin(), g).first;
        } else if (get_quasi_active_species(g) != 0
                   && all_species_with_red_edges(g)) {
            tmp = realize_species(get_quasi_active_species(g), g).first;
        } else {
            if (has_red_sigmagraph(g)) {
                std::cout << "[INFO] Red sigma graph generated" << std::endl;
            }
            throw std::runtime_error(
                "[ERROR] In ppp_maximal_reducible_graphs(): could not build "
                "the PPP");
        }

        realized_chars.splice(realized_chars.end(), tmp);
        realized_chars.splice(realized_chars.end(),
                              realize_red_univ_and_univ_chars(g).first);
        remove_duplicate_species(g);

        if (!is_empty(g)) {
            RBGraphVector conn_compnts = connected_components(g);
            auto cc = conn_compnts.begin();
            auto cc_end = conn_compnts.end();
            for (; cc != cc_end; ++cc) {
                RBGraph tmp_graph;
                copy_graph(*cc->get(), tmp_graph);
                tmp = ppp_maximal_reducible_graphs(*cc->get());
                if (logging::enabled) {
                    std::cout << "[INFO] iterating in conn_compt " << std::endl;
                }
                for (RBVertex v : tmp_graph.m_vertices) {
                    remove_vertex(tmp_graph[v].name, g);
                }
                realized_chars.splice(realized_chars.end(), tmp);
            }
        }
    }
    return realized_chars;
}

std::pair<std::list<SignedCharacter>, bool>
realize_red_univ_and_univ_chars(RBGraph &g) {
    std::list<SignedCharacter> output;

    RBVertexIter v, v_end, next;
    std::tie(v, v_end) = vertices(g);

    std::list<SignedCharacter> lsc;
    for (next = v; v != v_end; v = next) {
        next++;
        if (is_red_universal(*v, g)) {
            // if v is red_universal
            // realize v-
            if (logging::enabled) {
                // verbosity enabled
                //std::cout << "G red-universal character " << g[*v].name << std::endl;
            }
            std::tie(lsc, std::ignore) =
                realize_character({g[*v].name, State::lose}, g);

            output.splice(output.cend(), lsc);

            // we have to loop again from the beginning, because if some isolate node has been deleted, the for loop is compromised
            std::tie(v, v_end) = vertices(g);
            next = v;
        } else if (is_universal(*v, g)) {
            // if v is universal
            // realize v+
            if (logging::enabled) {
                // verbosity enabled
                //std::cout << "G universal character " << g[*v].name << std::endl;
            }

            std::list<SignedCharacter> lsc;

            std::tie(lsc, std::ignore) =
                realize_character({g[*v].name, State::gain}, g);

            output.splice(output.cend(), lsc);

            // we have to loop again from the beginning, because if some isolate node has been deleted, the for loop is compromised
            std::tie(v, v_end) = vertices(g);
            next = v;
        }
    }

    if (!output.empty()) {
        return std::make_pair(output, true);
    } else {
        return std::make_pair(output, false);
    }
}

RBVertex get_extension(const RBVertex &s, const RBGraph &gmax,
                       const RBGraph &gmin) {
    if (logging::enabled) {
        std::cout << "[INFO] get_extension " << std::endl;
    }

    // ******** CASE 1 ********
    // check whether s already exists in gmin without any additional minimal characters. If it exists, then we have already found the extension of s.

    // find the characters of s
    std::set<std::string> s_chars;
    for (RBVertex v : get_adj_vertices(s, gmax)) {
        s_chars.insert(gmax[v].name);
    }

    for (RBVertex v : gmin.m_vertices) {
        if (is_species(v, gmin)) {
            std::set<std::string> v_chars;
            for (RBVertex u : get_adj_vertices(v, gmin)) {
                v_chars.insert(gmin[u].name);
            }

            if (v_chars == s_chars) {
                return v;
            }
        }
    }

    // ******** CASE 2 ********
    // find all the species such that (1) they include s and (2) they do not have additional maximal characters. These species will be possible candidates

    std::list<RBVertex> max_chars_of_gmin = maximal_characters(gmin);
    std::list<RBVertex> candidates;// it contains the possible candidates
    for (RBVertex v : gmin.m_vertices) {
        if (is_species(v, gmin)) {

            // find the characters of v
            std::set<std::string> v_chars;
            for (RBVertex u : get_adj_vertices(v, gmin)) {
                v_chars.insert(gmin[u].name);
            }

            // v must include s
            if (std::includes(v_chars.begin(), v_chars.end(), s_chars.begin(),
                              s_chars.end())) {
                std::cout << "includes " << gmin[v].name << std::endl;

                // v must not have more max chars than s
                bool more_max_chars = false;
                for (RBVertex max_char : max_chars_of_gmin) {
                    if (!exists(gmax[s].name, gmax[max_char].name, gmax)
                        && exists(gmin[v].name, gmin[max_char].name, gmin)) {
                        more_max_chars = true;
                        break;
                    }
                }

                if (!more_max_chars) {
                    std::cout << "has not more max chars than s "
                              << gmin[v].name << std::endl;
                    candidates.push_back(v);
                }
            }
        }
    }

    // if there is only a candidate, then return it
    if (candidates.size() == 1) {
        return *candidates.begin();
    }

    // if we have more candidates, we have to choose the candidate X that satisfies
    // the following property: (1) the realization of the characters of X and of
    // all the minimal characters that overlap with the minimal characters of s
    // must not induce a red-sigma graph and (2) X must be minimal.

    // we first sort the candidate list so that (2) is satisfied
    sort_by_degree(candidates, gmin);
    candidates.reverse();
    for (RBVertex candidate : candidates) {

        // this list will contain the minimal characters that overlap with the ones
        // of s. We are going to build it by first inserting all the vertices of gmin
        // and then reducing it progressively.
        std::set<RBVertex> overlapping_min_chars(gmin.m_vertices.begin(),
                                                 gmin.m_vertices.end());
        for (RBVertex u : gmin.m_vertices) {
            if (is_species(u, gmin)) {
                overlapping_min_chars.erase(u);
            }
        }

        std::list<RBVertex> candidate_chars = get_adj_vertices(candidate, gmin);
        for (RBVertex u : candidate_chars) {
            overlapping_min_chars.erase(u);
        }

        for (RBVertex u : max_chars_of_gmin) {
            overlapping_min_chars.erase(u);
        }

        for (RBVertex z : overlapping_min_chars) {
            for (RBVertex u : candidate_chars) {
                if (!overlaps_character(z, u, gmin)) {
                    overlapping_min_chars.erase(z);
                }
            }
        }

        // check for the red-sigma graph
        RBGraph g_copy;
        copy_graph(gmin, g_copy);
        realize_species(get_vertex(gmin[candidate].name, g_copy), g_copy);
        for (RBVertex u : overlapping_min_chars) {
            if (exists(gmin[u].name, g_copy)) {
                realize_species(get_vertex(gmin[u].name, g_copy), g_copy);
            }
        }

        if (!has_red_sigmagraph(g_copy)) {
            std::cout << "Candidate does not induce red sigmagraph"
                      << std::endl;
            return candidate;
        } else {
            std::cout << "RED sigmagraph" << std::endl;
        }
    }

    return 0;// return null
}

std::list<RBVertex> get_sources(const RBGraph &gm) {
    std::list<RBVertex> sources = get_all_minimal_p_active_species(gm, true);
    if (sources.empty()) {
        std::list<RBVertex> pending_species = get_pending_species(gm);
        if (pending_species.size() == 1) {
            sources.push_back(*pending_species.begin());
        }
    }
    return sources;
}

bool is_2_solvable(std::list<RBVertex> &sources, const RBGraph &gm) {
    if (sources.size() == 1) {
        if (logging::enabled) {
            std::cout << " [INFO] mono_solvable " << std::endl;
        }
        return true;
    } else if (sources.size() != 2) {
        return false;
    } else {
        RBVertex source1 = *sources.begin();
        RBVertex source2 = *++sources.begin();
        if (get_adj_vertices(source1, gm).size() < 2
            || get_adj_vertices(source2, gm).size() < 2) {
            return false;
        }
    }
    return true;
}

bool is_3_canonical(std::list<RBVertex> &sources, const RBGraph &gm) {
    if (sources.size() != 3) {
        return false;
    }
    RBVertex source1 = *sources.begin();
    RBVertex source2 = *++sources.begin();
    RBVertex source3 = *++++sources.begin();

    if (logging::enabled) {
        std::cout << " [TODO]: is_3_solvable semantics " << std::endl;
    }
    return true;
}

bool is_m_solvable(std::list<RBVertex> &sources, const RBGraph &gm) {
    if (sources.size() <= 3) {
        return false;
    }
    //

    return true;
}

std::list<RBVertex> closure(const RBVertex &v,
                            const RBGraph &g)//C(S) char massimali
{
    //CL(s) is defined as follows: a ∈ CL(s) if and only if a is minimal in G and moreover it is
    //included in all maximal characters of s.

    // get the minimal characters as follow:
    // cmin = vertices(g) - cmax - species(g)

    std::list<RBVertex> cmax = maximal_characters(g);
    std::list<RBVertex> cmin(g.m_vertices.begin(), g.m_vertices.end());
    RBVertexIter b = cmin.begin(), e = cmin.end(), next;
    for (next = b; b != e; b = next) {
        ++next;
        if (is_species(*b, g)) {
            cmin.remove(*b);
        }
        for (RBVertex u : cmax) {
            if (g[*b].name == g[u].name) {
                cmin.remove(*b);
                break;
            }
        }
    }

    // keep just the max chars of v
    b = cmax.begin(), e = cmax.end();
    for (next = b; b != e; b = next) {
        ++next;
        if (!exists(*b, v, g)) {
            cmax.remove(*b);
        }
    }

    // keep just the min chars that are included in all max chars of v
    b = cmin.begin(), e = cmin.end();
    for (next = b; b != e; b = next) {
        ++next;
        for (RBVertex u : cmax) {
            if (!includes_characters(u, *b, g)) {
                cmin.remove(*b);
                break;
            }
        }
    }

    return cmin;
}

////////////////////////////////////////////////
//bool type_one(RBVertex &s, RBVertex &salt) {
//    // s source and salt source alternative source
//}

// new
bool type_one(const RBGraph &g, const RBVertex &main_source,
              const RBVertex &other_source, const std::list<RBVertex> &closure,
              const std::list<RBVertex> &interjection) {
    // TODO: check type1 algorithm and resolution
    std::list<RBVertex> ms_chars = get_comp_vertex(main_source, g);
    std::list<RBVertex> os_chars = get_comp_vertex(other_source, g);
    // ms_chars - max(chars) - os_chars
    std::list<RBVertex> max_chars = maximal_characters(g);

    std::list<RBVertex> a_chars(g.m_vertices.begin(), g.m_vertices.end());
    RBVertexIter b = a_chars.begin(), e = a_chars.end(), next;

    for (next = b; b != e; b = next) {
        bool found = false;
        ++next;
        if (is_species(*b, g)) {
            a_chars.remove(*b);
            found = true;
            break;
        } else {
            for (RBVertex v : os_chars) {
                if (g[*b].name == g[v].name & !found) {
                    a_chars.remove(*b);
                    found = true;
                    break;
                }
            }
            for (RBVertex v : max_chars) {
                if (g[*b].name == g[v].name & !found) {
                    a_chars.remove(*b);
                    found = true;
                    break;
                }
            }
        }

        // for each a - begin

        // leaf-species-not-in-inverted-path
        std::list<RBVertex> leaf_species(g.m_vertices.begin(),
                                         g.m_vertices.end());
        RBVertexIter b_L = leaf_species.begin(), e_L = leaf_species.end(),
                     next_L;
        for (next_L = b_L; b_L != e_L; b_L = next_L) {
            bool found_L = false;
            ++next_L;
            if (is_character(*b_L, g)) {
                leaf_species.remove(*b_L);
                found_L = true;
                break;
            } else {

                std::list<RBVertex> char_set = get_comp_vertex(*b_L, g);
                bool pass = false;
                for (RBVertex v : interjection) {
                    if (contains(char_set, v)) {
                        // not pass;
                    } else {
                        // add sm to leaf
                        pass = true;
                        break;
                    }
                }
                if (!pass) {
                    leaf_species.remove(*b_L);
                    found_L = true;
                    break;
                }
            }
        }

        // Set-B
        std::list<RBVertex> b_chars(g.m_vertices.begin(), g.m_vertices.end());
        RBVertexIter b_B = b_chars.begin(), e_B = b_chars.end(), next_B;
        for (next_B = b_B; b_B != e_B; b_B = next_B) {
            bool found_B = false;
            ++next_B;
            if (is_species(*b_B, g)) {
                b_chars.remove(*b_B);
                found_B = true;
                break;
            } else {
                for (RBVertex v : max_chars) {
                    if (g[*b_B].name == g[v].name & !found_B) {
                        b_chars.remove(*b_B);
                        found_B = true;
                        break;
                    }
                }

                for (RBVertex v : closure) {
                    if (found_B)
                        break;
                    if (g[*b_B].name == g[v].name & !found_B) {
                        b_chars.remove(*b_B);
                        found_B = true;
                        break;
                    }
                }

                for (RBVertex v : os_chars) {
                    if (found_B)
                        break;
                    if (g[*b_B].name == g[v].name & !found_B) {
                        b_chars.remove(*b_B);
                        found_B = true;
                        break;
                    }
                }

                if (!overlaps_character(*b_B, *b, g)) {
                    b_chars.remove(*b_B);
                    found_B = true;
                    break;
                }
            }
            // for-each B left
            if (!found_B) {
                bool pass = true;
                for (RBVertex v : leaf_species) {
                    if (contains(get_comp_vertex(v, g), *b_B)) {
                        // pass FALSE
                        pass = false;
                        break;
                    }
                }
                if (pass) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool is_linetree(const RBGraph &g) {
    std::list<RBVertex> vertices(g.m_vertices.begin(), g.m_vertices.end());
    RBVertexIter b = vertices.begin(), e = vertices.end(), next;
    ulong tmp;
    bool found = false;
    for (next = b; b != e; b = next) {
        ++next;
        // TODO: replace with degree of incident EDGES
        if (out_degree(*b, g) > 2) {
            std::cout << "GUARD_IS_A_SPLIT_NODE" << std::endl;
            std::cout << "NODE: " << g[*b].name << std::endl;
            found = true;
        }
    }

    return !found;
}

bool test_l_source(const RBVertex &s1, const RBVertex &s2,
                   const RBGraph &g_skeleton, const RBGraph &g) {

    // given s1 species, return a list of connected chars
    // remove from s1_max_chars, minimal chars of g
    std::list<RBVertex> tmp;
    // prendo le componenti di s1 in g
    std::list<RBVertex> s1_max_chars = get_comp_vertex(s1, g);

    // prendo i max char di g
    std::list<RBVertex> cmax = maximal_characters(g);

    // computo i min char di g, rimuovendo le specie e rimuovendo i char maximali dall'iterator di vertici di g
    std::list<RBVertex> cmin(g.m_vertices.begin(), g.m_vertices.end());
    RBVertexIter b = cmin.begin(), e = cmin.end(), next;

    for (next = b; b != e; b = next) {
        ++next;
        if (is_species(*b,
                       g)) {// se e' specie, rimuovila
            cmin.remove(*b);
        }
        for (RBVertex u : cmax) {
            if (g[*b].name == g[u].name) {
                cmin.remove(*b);
                break;
            }
        }
    }

    // rimuovo dalle componenti di s1, i char minimali
    b = s1_max_chars.begin(), e = s1_max_chars.end(), next;
    for (next = b; b != e; b = next) {
        ++next;
        for (RBVertex u : cmin) {
            if (g[*b].name == g[u].name) {
                s1_max_chars.remove(*b);
                break;
            }
        }
    }// got s1_max_chars

    // riga 3 algoritmo --> per ogni a in cmin
    bool included = true;
    for (RBVertex a : cmin) {
        b = cmin.begin(), e = cmin.end();
        for (next = b; b != e; b = next) {
            ++next;
            for (RBVertex u : s1_max_chars) {
                if (!includes_characters(u, *b, g)) {
                    included = false;
                    break;
                } else {// se ogni u *carattere di C_s ovvero di s1_max_chars* include il char a
                    // allora computo la closure
                    tmp = closure(s1, g);
                    //tmp.push_back(a);// serve??? CL(s) = {a} U CL(s)
                }
            }
        }
    }

    // computo s-graph : sottografo di G indotto da C_s U CL(s)

    // s1_max_chars = c_s
    // tmp = closure di s
    //    std::list<RBVertex> closure_ext = tmp; TODO check this value

    std::cout << "GUARD_TREE GRAPH\n" << g << std::endl;


    RBGraph s_graph;

    //adding tmp
    for (RBVertex v : tmp) {
        if (!exists(g[v].name, s_graph)) {
            add_character(g[v].name, s_graph);

            RBOutEdgeIter e, e_end;
            std::tie(e, e_end) = out_edges(v, g);
            for (; e != e_end; ++e) {
                if (!exists(g[e->m_target].name, s_graph)) {
                    add_species(g[e->m_target].name, s_graph);
                }
                add_edge(g[v].name, g[e->m_target].name, Color::black, s_graph);
            }
        }
    }

    //adding s1_max_chars
    for (RBVertex v : s1_max_chars) {
        if (!exists(g[v].name, s_graph)) {
            add_character(g[v].name, s_graph);

            RBOutEdgeIter e, e_end;
            std::tie(e, e_end) = out_edges(v, g);
            for (; e != e_end; ++e) {
                if (!exists(g[e->m_target].name, s_graph)) {
                    add_species(g[e->m_target].name, s_graph);
                }
                add_edge(g[v].name, g[e->m_target].name, Color::black, s_graph);
            }
        }
    }
    std::cout << "GUARD_TREE S_GRAPH\n" << s_graph << std::endl;

    //interjection
    std::list<RBVertex> s1_chars = get_adj_inactive_characters(s1, s_graph);
    std::list<RBVertex> s2_chars = get_adj_inactive_characters(s2, s_graph);
    std::list<RBVertex> interjection;

    for (RBVertex i : s1_chars) {
        for (RBVertex j : s2_chars) {
            if (s_graph[j].name == s_graph[i].name) {
                interjection.push_back(i);
                break;
            }
        }
    }

    std::cout << "GUARD_INTERJECTION\ninterjection" << std::endl;

    for (RBVertex p : interjection) {
        std::cout << "(" << s_graph[p].name << ") ";
    }
    std::cout << std::endl;


    std::cout << "\nMAIN Source" << std::endl;
    for (RBVertex s : s1_chars) {
        std::cout << "(" << s_graph[s].name << ") ";
    }
    std::cout << "\nOTHER Source" << std::endl;
    for (RBVertex s : s2_chars) {
        std::cout << "(" << s_graph[s].name << ") ";
    }
    std::cout << std::endl;


    // computing G_s
    RBGraph sub_s_graph;

    //se tutta l'intersezione fa parte di una specie, salvo il set di specie
    // e prendo il sottografo con quelle

    // if comp

    for (RBVertex s : s_graph.m_vertices) {
        if (is_species(s, s_graph)) {
            std::list<RBVertex> specie_chars = get_adj_inactive_characters(s, s_graph);


            std::cout << "GUARD_ADJ_vertices" << std::endl;
            std::cout << "\nadjjj" << std::endl;
            for (RBVertex s : specie_chars) {
                std::cout << "(" << s_graph[s].name << ") ";
            }
            std::cout << std::endl;

            bool inSpecie = true;
            for (RBVertex v : interjection) {
                if (!containsV2(specie_chars, v, s_graph)) {
                    std::cout << "GUARD_SUB_CANT_ADD\n" << s_graph[v].name << std::endl;
                    inSpecie = false;
                    break;
                }
            }

            if (inSpecie) { // keep specie in the sub_s_graph
                if (!exists(s_graph[s].name, sub_s_graph)) {
                    add_species(s_graph[s].name, sub_s_graph);
                    std::cout << "GUARD_SUB_S_GRAPH_OK" << std::endl;
                    RBOutEdgeIter e, e_end;
                    std::tie(e, e_end) = out_edges(s, s_graph);
                    for (; e != e_end; ++e) {
                        if (!exists(s_graph[e->m_target].name, sub_s_graph)) {
                            add_character(s_graph[e->m_target].name, sub_s_graph);
                        }
                        add_edge(s_graph[s].name, s_graph[e->m_target].name,
                                 Color::black, sub_s_graph);
                    }


                }

            }
        }
//        for (RBVertex v : interjection) {
//            bool notinspecie = false;
//
//            if (!exists(s_graph[v].name, sub_s_graph)) {
//                add_character(s_graph[v].name, sub_s_graph);
//
//                RBOutEdgeIter e, e_end;
//                std::tie(e, e_end) = out_edges(v, s_graph);
//                for (; e != e_end; ++e) {
//                    if (!exists(s_graph[e->m_target].name, sub_s_graph)) {
//                        add_species(s_graph[e->m_target].name, sub_s_graph);
//                    }
//                    add_edge(s_graph[v].name, s_graph[e->m_target].name,
//                             Color::black, sub_s_graph);
//                }
//            }
//        }
    }
    std::cout << "GUARD_TREE SUB_S_GRAPH\n" << sub_s_graph << std::endl;

    if (logging::enabled)
        std::cout << "[INFO] Reached Linetree " << std::endl;

    if (is_linetree(sub_s_graph)) {
        std::cout << "GUARD_LINETREE_OK" << std::endl;
        if (type_one(g, s1, s2, tmp, interjection)) {
            std::cout << "GUARD_TYPE1_NOT" << std::endl;
            return false;
        } else {
            std::cout << "GUARD_TYPE1" << std::endl;
            return true;
        }
    } else {
        std::cout << "GUARD_LINETREE_NOT" << std::endl;

        return false;
    }
}

RBVertex source_2_solvable(const std::list<RBVertex> &sources,
                           const RBGraph &g_skeleton, const RBGraph &g_min,
                           const RBGraph &g) {
    RBVertex tmp = nullptr;

    if (sources.size() == 1) {
        RBVertex source = *sources.begin();
        RBVertex extension = get_extension(source, g_skeleton, g_min);
        std::cout << "GUARD_B" << std::endl;
        //extension
        //tmp is the s-extension to source_2_solvable
        if (extension != nullptr && is_species(extension, g_skeleton)) {
            std::cout << "GUARD_B1" << std::endl;
            return extension;
        }
        std::cout << "GUARD_B2" << std::endl;

    } else if (sources.size() == 2) {
        if (logging::enabled)
            std::cout << "[INFO] 2-sources " << std::endl;

        RBVertex source1 = *sources.begin();
        RBVertex source2 = *++sources.begin();
        RBVertex extension1 = get_extension(source1, g_skeleton, g_min);
        RBVertex extension2 = get_extension(source2, g_skeleton, g_min);

        if (extension1 != nullptr && extension2 != nullptr
            && is_species(extension1, g_skeleton)
            && is_species(extension2, g_skeleton)) {

            std::cout << "GUARD_C" << std::endl;

            if (test_l_source(extension1, extension2, g_skeleton, g)) {
                std::cout << "GUARD_EXT1" << std::endl;
                tmp = extension1;
                //                tmp.splice(tmp.end(),
                //                           realize_species(extension1, g_skeleton).first);
            }
            std::cout << "GUARD_C1" << std::endl;

            if (test_l_source(extension2, extension1, g_skeleton, g)) {

                std::cout << "GUARD_EXT2" << std::endl;
                tmp = extension2;
                //                tmp.splice(tmp.end(),
                //                           realize_species(extension2, g_skeleton).first);
            }
            if (tmp != nullptr) {
                std::cout << "GUARD_EXT_1_2_NULL" << std::endl;
                return tmp;
            }

            std::cout << "GUARD_C2" << std::endl;
        }
    }
    throw std::runtime_error("[ERROR] In ppr_general: could not "
                             "compute persistent phylogeny");
}

#if 0
std::list<SignedCharacter> source_3_canonical(std::list<RBVertex> &sources,
                                              const RBGraph &g_skeleton) {
    if (logging::enabled) {
        std::cout << "[TODO] implement source_3_canonical " << std::endl;
    }
}

//#if 0
std::list<SignedCharacter> source_m_solvable(std::list<RBVertex> &sources,
                                             const RBGraph &g_skeleton) {
    if (logging::enabled) {
        std::cout << "[TODO] implement source_m_solvable  " << std::endl;
    }
}
#endif

std::list<SignedCharacter>
ppr_general(RBGraph &g) /*, std::list<SignedCharacter> a) */
{
    // g-skeleton computation g->g_min->max_reducible_graph

    //    if (logging::enabled) std::cout << "[PRINT] Graph print " << g << std::endl;

    // ** found that gmin->g-max-red it's the same as computing a g-skelton (making a graph from all Inactive max char plus all active chars

    if (logging::enabled) {
        std::cout << "[INFO] Realizing chars" << std::endl;
    }
    std::list<SignedCharacter> realized_chars =
        realize_red_univ_and_univ_chars(g).first;
    remove_duplicate_species(g);
    std::list<SignedCharacter> tmp;

    if (logging::enabled) {
        std::cout << "[INFO] Running PPPH iteration " << std::endl;
    }

    while (!is_empty(g)) {

        RBGraph gm;
        RBGraph g_max;
        RBGraph g_min;
        std::cout << "GUARD_PRINT_TREE\n" << g <<  std::endl;


        if (logging::enabled) {
            std::cout << "[INFO] Computing minimal form" << std::endl;
        }
        minimal_form_graph(g, g_min);

        if (logging::enabled) {
            std::cout << "[INFO] Computing g-skeleton" << std::endl;
        }
        g_skeleton(g, gm);
        std::cout << "GUARD_PRINT_SKELETON\n" << gm <<  std::endl;

        if (logging::enabled) {
            std::cout << "[INFO] Computing max_red_graph" << std::endl;
        }
        maximal_reducible_graph(g_min, g_max, true);

        // need this in advance for realizing characters, do this before the main loop
        if (logging::enabled) {
            std::cout << "[INFO] Getting Sources" << std::endl;
        }
        std::list<RBVertex> gm_sources = get_sources(gm);

        if (is_2_solvable(gm_sources, gm)) {

            tmp =
                realize_species(source_2_solvable(gm_sources, gm, g_min, g), g)
                    .first;

            if (logging::enabled) {
                std::cout << "[INFO] 2-solvable " << std::endl;
            }
            std::cout << "GUARD_A 2_SOLVABLE" << std::endl;


            //} else if (!is_2_solvable(sources_skeleton, gm)) {
            // return result; // skip controls,
        } else if (is_degenerate(gm)) {
            if (logging::enabled) {
                std::cout << "[INFO] is-degenerate " << std::endl;
            }
            //            throw std::runtime_error("[ERROR] In ppr_general: could not "
            //                                     "compute persistent phylogeny");
            break;
        } else {
            if (logging::enabled) {
                std::cout << "[INFO] no-2-solv " << std::endl;
            }
            break;
            //            throw std::runtime_error("[ERROR] In ppr_general: could not "
            //                                     "compute persistent phylogeny");
            // red sigmagraph??
        }

        if (!tmp.empty()) {
            if (logging::enabled)
                std::cout << "[INFO] realization not empty " << std::endl;

            // realize result in G
            //update G and A
            // A set of characters of r (relized chars)

            realized_chars.splice(realized_chars.end(), tmp);
            realized_chars.splice(realized_chars.end(),
                                  realize_red_univ_and_univ_chars(g).first);
            remove_duplicate_species(g);
            // iterates over connected components
            if (!is_empty(g)) {
                if (logging::enabled)
                    std::cout << "[INFO] Iterating conn_cmpt " << std::endl;
                std::cout << "GUARD_ITERATING_COMPT" << std::endl;
                RBGraphVector conn_compnts = connected_components(g);
                auto cc = conn_compnts.begin();
                auto cc_end = conn_compnts.end();
                for (; cc != cc_end; ++cc) {
                    RBGraph tmp_graph;
                    copy_graph(*cc->get(), tmp_graph);
                    tmp = ppr_general(*cc->get());
                    for (RBVertex v : tmp_graph.m_vertices) {
                        remove_vertex(tmp_graph[v].name, g);
                    }
                    realized_chars.splice(realized_chars.end(), tmp);

                    if (logging::enabled)
                        std::cout << "[INFO] Realized chars ";
                    for (SignedCharacter sc : realized_chars)
                        std::cout << "(" << sc << ") ";
                    std::cout << std::endl;
                }
            }
        }
    }
    return realized_chars;
}
