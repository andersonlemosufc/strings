#ifndef SUFIX_TREE_H
#define SUFIX_TREE_H
#include <unordered_map>
#include <string>
#include <iostream>
class sufix_tree
{
   typedef struct {
        int begin,end;
   }info;

    typedef struct node{
        node* sufix_link;
        unsigned int beg,end;
        std::unordered_map<char,node*> children;
        node(){}
        node(unsigned int beg, unsigned int end):beg(beg),end(end){}
        int edge_size(){return end-beg+1;}
    }Node;
    Node* root;
    std::string base_str;

    int phase(int i,int nleaves);

public:
    sufix_tree(std::string str);
    void print_node(Node* node);
};

#endif // SUFIX_TREE_H
