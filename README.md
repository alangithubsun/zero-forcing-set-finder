Any graph to be tested should be stored in its own test file in the following format:
        [# of vertices]
        [# of edges]
        [edge]
        [edge]
         ...
        [edge]
where vertices V = {0, 1, ..., n - 1} and each edge is in the form of two integers separated by a space.

The graph is read in through input redirection from the file, so lauch the program like this:
    ./zero < [graph.txt]

The program will then find and print all subsets of V, all zero forcing subsets of V, 
and all minimal zero forcing subsets of V. Finally, the number of zero forcing subsets,
number of minimal zero forcing subsets, and the zero forcing number of the graph are printed.