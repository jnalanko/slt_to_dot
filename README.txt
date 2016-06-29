Prints the suffix link tree of the input file in dot format into stdout.

Compile with make at the project root
Note: Needs the cmake build tool installed to build the sdsl-lite library
Building tested on OS X 10.10 and Ubuntu 14

Usage: ./slt_to_dot -f inputfile [--fasta] [--debug]
    Prints the suffix link tree of the text in the input file to stdout
    Options:
    --fasta: Interprets the input file as a fasta-format file
             concatenating all sequences found in the file placing
             dollar symbols between all found sequences. Does not
             explore the subtrees of edges with dollars, i.e. if the
             edge from the parent of a node is labelled with a dollar,
             do not explore the children of the node.
    --debug: Label all nodes with the corresponding substrings

Small example data file example.txt included in the project root.
To run example (after building) run the command ./slt_to_dot -f example.txt

Note: maximum input size: 2^31 - 1 characters.

Repository also contains some additional tools which are not documented.
