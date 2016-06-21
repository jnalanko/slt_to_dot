#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include "BD_BWT_index.hh"
#include "Iterators.hh"
#include <streambuf>
#include <utility>
#include <string>

using namespace std;

void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}

void trim(std::string &s) {
    ltrim(s); rtrim(s);
}

string parseConcatenate(std::istream &input, char separator){

    string data = "";
    string read = "";
    while(true){
        
        string line; getline(input,line); trim(line);
        if(line.size() == 0 && !input.eof()) continue;
        if(input.eof() || line[0] == '>'){ // Add the current read to the data if it's good
            if(read.size() != 0){                
                data += separator + read;
            }
         
            read = "";
            
            if(input.eof()) return data;
            
        } else{ // Add the characters on the line to the current read
            for(char c : line){
                c = toupper(c);
                read.push_back(c);
            }
        }
    }
}

void print_instructions(){
    cerr << "  Usage: ./slt_to_dot -f inputfile [--fasta] [--debug]" << endl;
    cerr << "  Prints the suffix link tree of the text in the input file to stdout" << endl;
    cerr << "  Options:" << endl;
    cerr << "  --fasta: Interprets the input file as a fasta-format file," << endl;
    cerr << "           concatenating all sequences found in the file placing" << endl;
    cerr << "           dollar symbols between all found sequences" << endl;
    cerr << "  --debug: Label all nodes with the corresponding substrings" << endl;
    return;
}


int main(int argc, char** argv){
    bool debug_mode = false;
    bool fasta = false;
    string filename;
    if(argc == 1){
        print_instructions();
        return 1;
    }
    for(int i = 1; i < argc; i++){
        if(string(argv[i]) == "--debug") debug_mode = true;
        else if(string(argv[i]) == "--fasta") fasta = true;
        else if(string(argv[i]) == "-f"){
            if(i == argc - 1) {
                cerr << "Error: give filename after -f" << endl;
                return 1;
            } else filename = argv[i+1];
            i++;
        }
        else{
            cout << "Error parsing command line parameters" << endl;
            print_instructions();
            return 1;
        }
        
    }
    
    if(filename == ""){
        cerr << "Error: missing input file" << endl;
        print_instructions();
        return 1;
    }
    
    
    ifstream instream(filename);
    if(!instream.good()){
        cerr << "Error: failed to open file " << filename << endl;
        return 1;
    }
    string s;
    if(fasta) s = parseConcatenate(instream,'$') + "$";
    else{
        std::string raw((std::istreambuf_iterator<char>(instream)),
                    std::istreambuf_iterator<char>());
        s = raw;
    }
    
    if(s.size() > 2147483647){ // 2147483647 = 2^31 - 1
        cerr << "Error: maximum input size is 2147483647 (=2^31 - 1) characters" << endl;
        return 1;
    }
    BD_BWT_index<> index((uint8_t*)(s.c_str()));
    BD_BWT_index_iterator<sdsl::bit_vector> it(&index, debug_mode);
    if(fasta) it.stop_at_dollars = true;
    cout << "digraph slt {\n";
    while(it.next()){
        // Iterate through the tree. The iterator is printing
        // the edges in .dot format to stdout
    }
    cout << "}" << endl;
    
}
