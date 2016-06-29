#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <streambuf>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <utility>

using namespace std;

vector<string> split(string s){
    // Split s by whitespace
    istringstream iss(s);
    vector<string> tokens;
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter(tokens));
    return tokens;
}

void compute_statistics(int node, int depth, vector<int>& subtree_sizes, 
                        vector<int>& depths, vector<vector<int> >& children){
    depths[node] = depth;
    subtree_sizes[node] = 1; // The node itself is counted in its subtree

    // Add the sizes of the subtrees to the count
    for(auto child : children[node]){
        compute_statistics(child, depth+1, subtree_sizes, depths, children);
        subtree_sizes[node] += subtree_sizes[child];            
    }
}

int main(int argc, char** argv){
    vector<pair<int,int> > edges;
    string line;
    int nVertices = 0; // Total number of vertices in the tree
    // Parse .dot input
    while(getline(cin,line)){
        vector<string> tokens = split(line);
        if(tokens.size() == 4){
            // This line describes an edge
            int from = stoi(tokens[0]);
            int to = stoi(tokens[2]);
            nVertices = max(nVertices, from+1);
            nVertices = max(nVertices, to+1);
            edges.push_back({from,to});
        }
    }

    vector<vector<int> > children(nVertices);
    for(auto E : edges){
        children[E.first].push_back(E.second);
    }

    vector<int> subtree_sizes(nVertices);
    vector<int> depths(nVertices);
    compute_statistics(0, 0, subtree_sizes, depths, children);
    for(int i = 0; i < nVertices; i++){
        cout << depths[i] << " " << subtree_sizes[i] << "\n";
    }
}
