#include "sufix_tree.h"

int sufix_tree::phase(int i, int nleaves){
    int ignored = nleaves;
    std::string current_sufix = base_str.substr(nleaves,i-nleaves);
    std::cout<<current_sufix<<std::endl;
    Node* node = root, *path_node, *new_node, *backup_node = nullptr;
    std::unordered_map<char,Node*>::iterator got;

    for (int j = 0; j < current_sufix.size(); ++j) {
        got = node->children.find(current_sufix[j]);
        //não existe filho que comece com esse caractere - aplicar regra 2
        if(got == node->children.end()){
            std::cout<<"c2_1 "<<i<<std::endl;
            //cria folha com a posição final após todas as fases (j+i) pra tornar o índice global
            new_node = new Node(j+ignored,base_str.size()-1);
            node->children[current_sufix[j]] = new_node;
            new_node = nullptr;
            nleaves++;
        }else{
            int k = 1;
            int offset = 0;
            while(true){
                path_node = got->second;
                int edge_size = path_node->edge_size();
                while(k-offset < edge_size && (j+k)<current_sufix.size() && current_sufix[j+k] == base_str[path_node->beg + (k-offset)]){
                    k++;
                }
                //caso 3
                if(j+k == current_sufix.size()){
                    std::cout<<"c3 "<<i<<std::endl;
                    return nleaves;
                }

                // tá errado - falta o caso que ainda falta parte do sufixo pra processar - e falta fase 3
                if(k-offset == edge_size){
                    got = path_node->children.find(current_sufix[j+k]);
                    //nesse caso outra folha é criada com índice global
                    if(got == path_node->children.end()){
                        std::cout<<"c2_2 "<<i<<std::endl;
                        path_node->children[current_sufix[j+k+1]] = new Node(path_node->beg+k+1,base_str.size()-1);
                        nleaves++;
                        break;
                    }
                    else{
                        //sufix link path
                        offset+=k;
                        node = path_node;
                    }


                }else{
                    new_node = new Node(path_node->beg,path_node->beg+k-1);
                    new_node->children[base_str[path_node->beg+k]] = path_node;
                    std::cout<<"c2_3 "<<current_sufix[j+k] <<" "<< base_str[path_node->beg+k]<<" "<<k<<std::endl;
                    path_node->beg+=k;
                    //nova folha com o sufixo que sobra
                    new_node->children[current_sufix[j+k]] = new Node(j+ignored+k,base_str.size()-1);
                    node->children[current_sufix[j]] = new_node;
                    nleaves++;
                    //nó do tipo xA e A é vazio
                    if(new_node->edge_size() == 1)
                        new_node->sufix_link = root;

                    if(backup_node!=nullptr)
                        backup_node->sufix_link = new_node;
                    backup_node = new_node;
                    break;
                }
            }
            if(node!=root)
                node = node->sufix_link;
        }
    }
    return nleaves;
}

sufix_tree::sufix_tree(std::string str):base_str(str){
    root = new Node();
    int nleaves = 0;
    for (int var = 1; var <= (int)str.size(); ++var) {
        nleaves = phase(var,nleaves);
    }
    print_node(root->children['x']->sufix_link);
}

void sufix_tree::print_node(sufix_tree::Node *node)
{
    std::cout<<base_str.substr(node->beg,node->edge_size())<<std::endl;
}
