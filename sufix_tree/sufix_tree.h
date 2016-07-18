#ifndef SUFIX_TREE_H
#define SUFIX_TREE_H
#include <unordered_map>
#include <string>
#include <iostream>
#include <queue>
#include <stack>
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
        int under_leaves(){
            if(this->children.empty())
                return 0;
            Node* path_node;
            std::unordered_map<char,Node*>::iterator it;
            int count = 0;
            std::queue<Node*> q;
            q.push(this);
            while(!q.empty()){
                path_node = q.front();
                q.pop();
                if(path_node->children.empty()){
                    count++;
                    continue;
                }
                for(it = path_node->children.begin();it!=path_node->children.end();it++){
                    q.push(it->second);
                }

            }
            return count;
        }
    }Node;
    Node* root;
    std::string base_str;

    int phase(int i,int nleaves);
    void print_node(Node* node);

public:
    sufix_tree(std::string str);
    bool is_substring(std::string p);
    int occurrences(std::string p);
    std::string longest_repeat();
    int longest_subsequence(std::string);
};

#endif // SUFIX_TREE_H
