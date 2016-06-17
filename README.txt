Prints the suffix link tree of the input file into stdout.

Compile with make at the project root
Note: Needs the cmake build tool installed to build the sdsl-lite library
Building tested on OS X 10.10 and Ubuntu 14

Usage: ./slt_to_dot -f inputfile [--fasta] [--debug]
    Prints the suffix link tree of the text in the input file to stdout
    Options:
    --fasta: Interprets the input file as a fasta-format file
             concatenating all sequences found in the file placing
             dollar symbols between all found sequences
    --debug: Label all nodes with the corresponding substrings
