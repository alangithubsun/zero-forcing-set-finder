/**
 * Created by Alan Sun (alansun@umich.edu) to demonstrate interest 
 * in Eric Ramos's example potential research problem involving 
 * Zero Forcing sets (NYC Discrete Math REU 2025). Also hopefully
 * displays competent coding capabilities :D
 * 
 * February 11, 2025
 */
#include "Graph.hpp"

using namespace std;


int main() {
    Graph g;
    cout << "All subsets sorted by id:\n";
    g.print_subsets();
    cout << '\n';

    cout << "All zero forcing subsets:\n";
    g.find_zero_forcing();
    g.print_zero_forcing_subsets();
    cout << '\n';

    cout << "All minimal zero forcing subsets:\n";
    g.find_minimal_zero_forcing();
    g.print_minimal_zero_forcing_subsets();
    cout << '\n';

    cout << "# of zero forcing subsets: " << g.get_num_zero_forcing() << '\n';
    cout << "# of minmal zero forcing subset: " 
        << g.get_num_minimal_zero_forcing() << '\n';
    cout << "Zero forcing number: " << g.get_zero_forcing_number() << '\n';
}



