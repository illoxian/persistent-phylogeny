/** 
 * 
 * @file nfuns.cpp
 * 
 * @author Mattia Sgro'
 * 
 */

#include "nfuns.hpp"
#include "functions.hpp"
#include "rbgraph.hpp"
#include <boost/graph/connected_components.hpp>
#include <boost/graph/depth_first_search.hpp>




















// RBGraph compute_gskeleton(const RBGraph &g) {
//   RBGraph g_skeleton;
//   RBGraph g_minimal_form;
// //
// //  get_active_chars
// //  get_inactive_chars
// // computes 
//   minimal_form_graph(g, g_minimal_form);
//   maximal_reducible_graph(g_minimal_form, g_skeleton, true);
//   return g_skeleton;
// }


// std::list<SignedCharacter> source_2_solvable(RBGraph &g)
// {
//   RBGraph gmax, gmin;

//   // compute gm g-skeleton of g
//   int source_size = get_sources(g).size();

//   if (source_size == 1)
//   {
//     std::list<SignedCharacter> extension;
//     RBVertex ext;
//     ext = get_extension(*get_sources(g).begin(), gmax, gmin);

//     if (extension.size() == 1)
//       return extension;
//   }
//   if (source_size == 2)
//   {
//     std::list<SignedCharacter> source1, source2;
//     //assign respective extensions


//   }
//   std::list<SignedCharacter> null_ret;
//   return  null_ret; // returning an empty set if either of the 2if statement is taken
// }

// // void test_l_source(std::list<SignedCharacter> source) {
// //   maximal_characters()
// // }


// std::list<SignedCharacter> ppr_general(RBGraph &g, std::list<SignedCharacter> a) {
//   std::list<SignedCharacter> realized_chars = realize_red_univ_and_univ_chars(g).first; // A == realized chars, as in algorithm 2
//                                                                                           // A set of characters in r == realized_chars of g
//   remove_duplicate_species(g);
//   // r temporary std::list
//   std::list<SignedCharacter> tmp;
//   // sources
//   while (!is_empty(g))
//   {
//     // Computed G-Skeleton

//     RBGraph g_skeleton = compute_gskeleton(g);
//     // is_2_solvable( *get_sources(g_skeleton).begin(), g_skeleton);

//     source_2_solvable(g_skeleton);

    
//     // if Gm is 2-solvable
//     // then r:= Source-2solvable(G,A)

//     if (true) //!r.empty()
//     {
//       // realize r==tmp in G
//       realized_chars.splice(realized_chars.end(), tmp);
//       realized_chars.splice(realized_chars.end(), realize_red_univ_and_univ_chars(g).first);
//       remove_duplicate_species(g);
//       // ? update A with r?
//       realized_chars = tmp;
//       // (update G, A)

//       // iterates over connected components
//       RBGraphVector conn_comp = connected_components(g);
//       auto cc = conn_comp.begin();
//       auto cc_end = conn_comp.end();
//       for (; cc != cc_end; ++cc)
//       { //itera sulle componenti connesse
//         RBGraph tmp_graph;
//         copy_graph(*cc->get(), tmp_graph);
//         tmp = ppr_general(*cc->get());
//         for (RBVertex v : tmp_graph.m_vertices)
//           remove_vertex(tmp_graph[v].name, g);
//         realized_chars.splice(realized_chars.end(), tmp);
//       }
//     }
//     // return realized_chars;
//     return tmp;
//   }
//   return tmp;
// }

