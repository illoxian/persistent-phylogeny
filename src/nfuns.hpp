/** 
 * 
 * @file nfuns.hpp
 * 
 * @author Mattia Sgro'
 * 
 */

#ifndef NEW_FUNCTIONS_HPP
#define NEW_FUNCTIONS_HPP

#include "functions.hpp"
#include "rbgraph.hpp"


/**
 * @brief Computes the g-skeleton of a graph. Graph-> minimalFormaGraph-> maximalReducibleGraph -> 
 *
**/

void compute_gskeleton(const RBGraph &g, RBGraph &g_skeleton);

/**
 * @brief algorithm 4 of the paper, defines the source_2_solvable procedure
 * 
 * @param[in] g Red-Black graph
 * 
 **/

// void source_2_solvable(RBGraph &g);


/**
  @brief It iterates the graph if it's valid. General procedure descripted in Algorithm 3.
  // General algorithm for 2-solvable graphs

  @param[in] g Red-black graph
  @param[in] a Set of Characters

  @return void
**/

std::list<SignedCharacter> ppr_general(RBGraph &g, std::list<SignedCharacter> a);













#endif