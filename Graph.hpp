#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_
/**
 * Created by Alan Sun (alansun@umich.edu) to demonstrate interest 
 * in Eric Ramos's example potential research problem involving 
 * Zero Forcing sets (NYC Discrete Math REU 2025). Also hopefully
 * displays competent coding capabilities :D
 * 
 * February 11, 2025
 */

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

#include <numeric>
#include <set>
#include <unordered_set>
#include <cmath>
#include <iomanip>

using namespace std;

class Graph {
public:
    struct Subset {
        unordered_set<size_t> V;
        size_t id; //base10 val of subset's unique binary representation

        /**
         * Requires: none
         * Modifies: none
         * Effects:  Return true if v in V.
         */
        bool contains(size_t v) const;
    };

    //comparators to sort Subsets by their id and size, respectively
    struct Subset_Id_Less;
    struct Subset_Size_Less;
    
private:
    const static size_t DOES_NOT_EXIST = string::npos;

public:
    /**
     * Requires: none
     * Modifies: n, adj_list, all_subsets
     * Effects:  Reads input from cin formatted as
     *              [# of vertices]
     *              [# of edges]
     *              [edge]
     *              [edge]
     *               ...
     *              [edge]
     *           where vertices V = {0, 1, ..., n - 1} and each edge
     *           is in the form of two integers separated by a space.
     *           Builds an adjacency list and generates all subsets of V.
     */
	Graph();

    /**
     * Requires: none
     * Modifies: zero_forcing_sets
     * Effects:  Finds all zero forcing sets
     */
    void find_zero_forcing();
    
    /**
     * Requires: find_zero_forcing() has already been executed
     * Modifies: minimal_zero_forcing_sets
     * Effects:  Finds all minimal zero forcing sets and the zero forcing number
     */
    void find_minimal_zero_forcing();

    /**
     * Requires: none
     * Modifies: cout
     * Effects:  Prints all subsets ordered by id
     */
	void print_subsets() const;

    /**
     * Requires: none
     * Modifies: cout
     * Effects:  Prints all zero forcing subsets in no particular order
     */
    void print_zero_forcing_subsets() const;

    /**
     * Requires: none
     * Modifies: cout
     * Effects:  Prints all minimal zero forcing subsets in no particular order
     */
    void print_minimal_zero_forcing_subsets() const;

    /**
     * Requires: find_zero_forcing() has already been run
     * Modifies: none
     * Effects:  Returns the number of zero forcing subsets
     */
    size_t get_num_zero_forcing() const;

    /**
     * Requires: find_minimal_zero_forcing() has already been run
     * Modifies: none
     * Effects:  Returns the number of minimal zero forcing subsets
     */
    size_t get_num_minimal_zero_forcing() const;

    /**
     * Requires: find_minimal_zero_forcing() has already been run
     * Modifies: none
     * Effects:  Returns the zero forcing number of the graph
     */
    size_t get_zero_forcing_number() const;

    /**
     * Requires: none
     * Modifies: os
     * Effects:  Prints the contents of S in the format
     *           [id]: [TAB] [vertices in increasing order]
     */
    friend ostream& operator<<(ostream& os, const Subset& S);

private:
    /**
     * Requires: none
     * Modifies: all_subsets
     * Effects:  Generates all subsets of V by using Gray Codes such that
     *           all adjacent subsets in all_subsets differ by exactly 
     *           one element. Each subset S is assigned an id, which is the
     *           base10 value of the binary Gray Code string it corresponds
     *           to; that is, the id is the sum of 2^v for all v in S.
     * 
     * REMARK: The goal of this was to be able to check if each subset is
     *         zero forcing without storing all subsets and to be able to 
     *         switch subsets with minimal insertions/removals. However,
     *         I have no clue how to do that, but this is also cool!
     */
    void generate_gray_code_subsets(size_t n);

    /**
     * Requires: none
     * Modifies: cout
     * Effects:  Prints all subsets to cout
     */
    void print_subsets_given_id(const unordered_set<size_t>& ids) const;

    /**
     * Requires: none
     * Modifies: zero_forcing_sets
     * Effects:  Returns true if S is a zero forcing set. If S has not been
     *           marked as zero forcing or non zero forcing, then we play
     *           one more round of the game to get S'. Recursively calls to
     *           see if S' is zero forcing. If so, then S is zero forcing 
     *           and likewise if not. S' is then marked as zero forcing or 
     *           not zero forcing, but S is not.
     * 
     * ASSUMPTION: Let S subset V and S' be the set of infected vertices after
     *             one round of the game is played with S. Then if S' is zero-
     *             forcing, S is also zero forcing.
     */
    bool is_zero_forcing(const Subset& S);

    /**
     * Requires: none
     * Modifies: none
     * Effects:  Plays one round of the "game". Let S' = S. For each v in S, 
     *           we check if it has exaclty one non-infected neighbor u. If 
     *           so, then we add u to S'. We then return the id of S'.
     * 
     *           This implementation does not create an S' but instead tracks
     *           the would be id of S' and updates it as elements are added.
     */
    size_t infect_neighbors(const Subset& S) const;

    /**
     * Requires: 0 <= id && id < n
     * Modifies: none
     * Effects:  Checks if subset with corresponding id has any 
     *           proper subsets that are also zero forcing.
     * 
     * ASSUMPTION: If a set S is not zero forcing, then any subset
     *             of S is also not zero forcing.
     */
    bool has_zero_forcing_subset(size_t id) const;

	size_t n; //V = {0, 1, ..., n - 1}
	vector<unordered_set<size_t>> adj_list;
    vector<Subset> all_subsets; //sorted by id for now, but want to change

    unordered_set<size_t> zero_forcing_sets;
    unordered_set<size_t> minimal_zero_forcing_sets;
    size_t zero_forcing_number;
};

#endif