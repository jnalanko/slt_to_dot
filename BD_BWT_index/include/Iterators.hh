#ifndef ITERATORS_HH
#define ITERATORS_HH

#include "BD_BWT_index.hh"
#include <algorithm>

/**
 * Class BD_BWT_index_iterator
 * 
 * Iterates the suffix link tree of the given index.
 * 
 */
template<class t_bitvector>
class BD_BWT_index_iterator{
    
public:
    
    class Stack_frame{
    public:
        Interval_pair intervals;  // forward interval, reverse interval
        int64_t depth; // depth in the suffix link tree
        uint8_t extension; // the label on the arc between this node and its parent
        int64_t node_id; // To print tree in .dot format
        Stack_frame(Interval_pair intervals, int64_t depth, char extension, int64_t node_id) : intervals(intervals), depth(depth), node_id(node_id), extension(extension) {}
        Stack_frame(){}
    };
    
    const BD_BWT_index<t_bitvector>* index;
    bool debug_mode;
    bool stop_at_dollars;
    int next_id;
    
    // Iteration state
    std::deque<Stack_frame> iteration_stack;
    Stack_frame current; // A stack frame containing information about the current node
    std::string label; // The string on the path from the root to the current node
    
    // Reused space between iterations
    std::vector<int64_t> local_c_array;
    
    BD_BWT_index_iterator(const BD_BWT_index<t_bitvector>* index, bool debug_mode = false) : index(index), debug_mode(debug_mode), stop_at_dollars(false), next_id(1), local_c_array(256) {
        Interval empty_string(0,index->size()-1);
        iteration_stack.push_back(Stack_frame(Interval_pair(empty_string,empty_string), 0, 0, 0));
        current = iteration_stack.back();
        label = "";
    }
    
    /**
     * @brief Go to the next suffix link tree node
     * 
     * Updates the member variables iteration_stack, current and label.
     * 
     * @return False if all nodes have been iterated, else true
     */
    bool next(); // Go to the next node. Return false if none found
    
    /**
     * @brief Go to the next depth k suffix link tree node
     * 
     * Updates the member variables iteration_stack, current and label.
     * 
     * @return False if all nodes have been iterated, else true
     */
    bool next(int64_t k);
    
private:
    void push_right_maximal_children(Stack_frame f);
    void update_label(Stack_frame f);
};


//  Interval_pair left_extend(Interval_pair intervals, char c, const std::vector<int64_t>& local_c_array) const;
template<class t_bitvector>
void BD_BWT_index_iterator<t_bitvector>::push_right_maximal_children(Stack_frame f){
    index->compute_local_c_array_forward(f.intervals.forward, local_c_array);
    for(uint8_t c : index->get_alphabet()){
        if(c == BD_BWT_index<t_bitvector>::END) continue;
        Interval_pair child = index->left_extend(f.intervals,c,local_c_array);
        if(child.forward.size() == 0) continue; // Extension not possible
        if(index->is_right_maximal(child)){
            // Add child to stack
            int64_t child_id = next_id;
            next_id++;
            if(debug_mode){
                std::string label_rev(label.rbegin(), label.rend());
                std::cout << "\"" << label_rev << "\" -> \"" << (char)c + label_rev << "\" [label=\"" << c << "\"];\n";
            }
            else
                std::cout << f.node_id << " -> " << child_id << " [label=\"" << c << "\"];\n";
            if(!stop_at_dollars || c != '$')
                iteration_stack.push_back(Stack_frame(child,f.depth+1,c,child_id));
        }
    }    
}

template<class t_bitvector>
void BD_BWT_index_iterator<t_bitvector>::update_label(Stack_frame f){
    while(label.size() > 0 && label.size() >= f.depth) // Unwind stack
        label.pop_back();
    if(f.extension != 0)
        label.push_back(current.extension);    
}

template<class t_bitvector>
bool BD_BWT_index_iterator<t_bitvector>::next(int64_t k){
    
    while(true){
        if(iteration_stack.empty()) return false;
        
        current = iteration_stack.back(); iteration_stack.pop_back();
        update_label(current);
        
        if(current.depth == k) return true; // Stop recursing to children and give control back

        push_right_maximal_children(current);

    }
}

template<class t_bitvector>
bool BD_BWT_index_iterator<t_bitvector>::next(){
    if(iteration_stack.empty()) return false;
    
    current = iteration_stack.back(); iteration_stack.pop_back();
    update_label(current);
    push_right_maximal_children(current);
        
    return true;
}


#endif
