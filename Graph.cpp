#ifndef _GRAPH_CPP_
#define _GRAPH_CPP_
/**
 * Created by Alan Sun (alansun@umich.edu) to demonstrate interest 
 * in Eric Ramos's example potential research problem involving 
 * Zero Forcing sets (NYC Discrete Math REU 2025). Also hopefully
 * displays competent coding capabilities :D
 * 
 * February 11, 2025
 */

#include "Graph.hpp"

bool Graph::Subset::contains(size_t v) const {
    return V.find(v) != V.end();
}

struct Graph::Subset_Id_Less {
    bool operator()(const Graph::Subset& l, const Graph::Subset& r) const {
        return l.id < r.id;
    }
};

struct Graph::Subset_Size_Less {
    bool operator()(const Graph::Subset& l, const Graph::Subset& r) const {
        if (l.V.size() == r.V.size()) {
            return l.id < r.id;
        } else {
            return l.V.size() < r.V.size();
        }
    }
};

ostream& operator<<(ostream& os, const Graph::Subset& S) {
    os << S.id << ":\t";
    set<int> temp(S.V.begin(), S.V.end());
    for (int v : temp)
        os << v << ' ';
    return os;
}

Graph::Graph() : zero_forcing_number(DOES_NOT_EXIST) {
    cin >> n;
    adj_list.resize(n);

    int num_edges;
    cin >> num_edges;

    //build adjacency list
    for (int i = 0; i < num_edges; ++i) {
        int u, v;
        cin >> u >> v;
        adj_list.at(u).insert(v);
        adj_list.at(v).insert(u);			
    }
    
    generate_gray_code_subsets(n);

    //destroying my beautiful work from the gray code function :/
    //now all_subsets.at(id) = subset with id
    sort(all_subsets.begin(), all_subsets.end(), Graph::Subset_Id_Less());
}

void Graph::find_zero_forcing() {
    //is_zero_forcing() adds S to zero_forcing_sets (if appropriate)
    for (const Subset& S : all_subsets)
        if (is_zero_forcing(S))
            zero_forcing_sets.insert(S.id);

    //the subset with all elements is not added
    zero_forcing_sets.insert(all_subsets.back().id);
}

void Graph::find_minimal_zero_forcing() {
    for (size_t set_id : zero_forcing_sets) {
        //if no subsets are zero forcing, then minimal
        if (!has_zero_forcing_subset(set_id))
            minimal_zero_forcing_sets.insert(set_id);
    }

    for (size_t set_id : minimal_zero_forcing_sets) {
        zero_forcing_number = 
            min(zero_forcing_number, all_subsets.at(set_id).V.size());
    }
}

void Graph::print_subsets() const {
    for (const Subset& s : all_subsets)
        cout << s << endl;
}

void Graph::print_zero_forcing_subsets() const {
    print_subsets_given_id(zero_forcing_sets);
}

void Graph::print_minimal_zero_forcing_subsets() const {
    print_subsets_given_id(minimal_zero_forcing_sets);
}


size_t Graph::get_num_zero_forcing() const {
    return zero_forcing_sets.size();
}


size_t Graph::get_num_minimal_zero_forcing() const {
    return minimal_zero_forcing_sets.size();
}


size_t Graph::get_zero_forcing_number() const {
    return zero_forcing_number;
}


void Graph::generate_gray_code_subsets(size_t n) {

    //base value of an empty set
    all_subsets.push_back(Subset{{}, 0});

    //bottom-up gray code generation using recursive definition
    size_t id_update = 1;

    //v is the new vertex to be added
    for (size_t v = 0; v < n; ++v) {

        size_t current_size = all_subsets.size();

        //copy each subset in reverse order and add v
        for (int i = int(current_size) - 1; i >= 0; --i) {

            unordered_set<size_t> U = all_subsets.at(i).V;
            U.insert(v);

            //id is base10 val of binary representation of each subset
            size_t U_id = all_subsets.at(i).id + id_update; //id_update = 2^v
            all_subsets.push_back(Subset{U, U_id});
        }

        id_update *= 2;
    }
}

void Graph::print_subsets_given_id(const unordered_set<size_t>& ids) const {
    for (size_t i : ids) {
        cout << all_subsets.at(i) << endl;
    }
}

bool Graph::is_zero_forcing(const Subset& S) {
    static unordered_set<size_t> non_zero_forcing_sets;

    //base case #1: s == V
    if (S.V.size() == n)
        return true;

    //base case #2: s == empty set
    if (S.V.empty())
        return false;

    //return true if we already know S is zero forcing
    if (zero_forcing_sets.find(S.id) != zero_forcing_sets.end())
        return true;
    
    //return false if we already know S is not zero forcing
    if (non_zero_forcing_sets.find(S.id) != non_zero_forcing_sets.end())
        return false;
    
    //finds what S would be after one round of the game
    const Subset& next_set = all_subsets.at(infect_neighbors(S));

    //if S doesn't change, then it is not zero forcing
    if (S.id == next_set.id) {
        non_zero_forcing_sets.insert(S.id);
        return false;
    }

    //we mark S as zero forcing/non zero forcing accordingly
    if (is_zero_forcing(next_set)) {
        zero_forcing_sets.insert(next_set.id);
        return true;
    } else {
        non_zero_forcing_sets.insert(next_set.id);
        return false;
    }
}

size_t Graph::infect_neighbors(const Subset& S) const {

    //makes it easier to prevent adding duplicates
    unordered_set<size_t> newly_infected;

    //for each infected vertex
    for (size_t v : S.V) {

        const unordered_set<size_t>& v_neighbors = adj_list.at(v);
        size_t num_infected_neighbors = 0;
        size_t noninfected_neighbor = DOES_NOT_EXIST;
     

        //counting # of infected neighbors
        for (size_t u : v_neighbors) {
            if (S.contains(u)) {
                ++num_infected_neighbors;
            } else {
                noninfected_neighbor = u;
            }
        }

        //if all but one neighbor is infected
        if (num_infected_neighbors == v_neighbors.size() - 1 &&
            noninfected_neighbor != DOES_NOT_EXIST) {

            //i tried [next_set_id += size_t(pow(2, noninfected_neighbor));]
            //but if noninfected_neighbor is the same for multiple points, it counts it twice

            newly_infected.insert(noninfected_neighbor);
        }
    }

    //computer next set's id
    size_t next_set_id = S.id;
    for (size_t u : newly_infected) {
        //since id = sum 2^v for all v in S, adding 2^u gets id of S union {u}
        next_set_id += size_t(pow(2, u));
    }

    //in case I royally messed up
    if (next_set_id >= size_t(pow(2, n))) {
        cout << "what!!!\n";
    }

    return next_set_id;
}

bool Graph::has_zero_forcing_subset(size_t id) const {
    //checks if all subsets with one less element are zero forcing
    //ASSUMPTION: if a set is not zero forcing, none of its subsets are either
    for (size_t v : all_subsets.at(id).V) {
        //since id = sum of 2^v for all v in V
        size_t subset_without_v = id - size_t(pow(2, v));
        if (zero_forcing_sets.find(subset_without_v) != zero_forcing_sets.end())
            return true;
    }
    return false;
}


#endif