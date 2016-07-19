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
            //cria folha com a posição final após todas as fases (j+i) pra tornar o índice global
            new_node = new Node(j+ignored,base_str.size()-1);
            print_node(new_node);
            std::cout<<" -> c2_1 "<<i<<std::endl;
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

int sufix_tree::phase2(int i, int nleaves){
    int ignored = nleaves;
    std::string sufix = base_str.substr(nleaves,i-nleaves);
    Node *parent = root,*path_node = root,*new_node,*backup_node;
    std::unordered_map<char,Node*>::iterator it;
    int extensions =(int) sufix.size();
    for (int j = 0; j < extensions; ++j) {
        std::string current_sufix = sufix.substr(j,extensions-j);

        it = path_node->children.find(current_sufix[0]);
        int checked = 1;
        while(it!=path_node->children.end()){
            parent = path_node;
            path_node = it->second;
            int k = 1;
            while(checked < current_sufix.size() && path_node->beg+k<=path_node->end && current_sufix[checked] == base_str[path_node->beg+k]){
                checked++;
                k++;
            }
            //caso 3
            if(checked == current_sufix.size()){
                std::cout<<"c3 - "<<current_sufix<<std::endl;
                return nleaves;
            }
            else if(path_node->beg+k > path_node->end )
                it = path_node->children.find(current_sufix[checked++]);
            else{
                //caso 2_2 difícil - divergiu no meio de uma aresta
                char key_backup = base_str[path_node->beg];

                new_node = new Node(path_node->beg,path_node->beg+k-1);
                char aux = base_str[path_node->beg+k];
                new_node->children[base_str[path_node->beg+k]] = path_node;
                path_node->beg+=k;
                //nova folha com os caracteres que sobraram
                new_node->children[current_sufix[checked]] = new Node(ignored+j+checked,base_str.size());
                nleaves++;
                std::cout<<current_sufix[checked]<<" c2_2 novo nó-> ";
                print_node(new_node);
                std::cout<<std::endl<<"pai : "<<current_sufix[checked]<<" ";
                print_node(new_node->children[current_sufix[checked]]);
                std::cout<<std::endl<<"pai : "<<aux<<" ";
                print_node(path_node);
                std::cout<<"  k = "<<ignored+j+checked-1<<"\n";
                //insere o novo nó como interno no lugar do antigo
                parent->children[key_backup] = new_node;

                if(backup_node!=nullptr)
                   backup_node->sufix_link = new_node;

                if(new_node->edge_size() == 1){
                    new_node->sufix_link = root;
                    backup_node = nullptr;
                }else
                    backup_node = new_node;

                break;
            }
        }
        // caso 2 simples - apendas criar uma folha se quebrar a condição loop
        if(it==path_node->children.end()){
            path_node->children[current_sufix[checked-1]] = new Node(ignored+j+checked-1,(int)base_str.size());
            std::cout<<"c2_1 "<<i<<current_sufix<<": ";
            print_node(path_node->children[current_sufix[checked-1]]);
            std::cout<<std::endl;
            nleaves++;
        }
        path_node = root;
    }
    return nleaves;

}

sufix_tree::sufix_tree(std::string str):base_str(str){
    root = new Node();
    int nleaves = 0;
    for (int var = 1; var <= (int)base_str.size(); ++var) {
        nleaves = phase2(var,nleaves);
    }
    print_node(root->children['b']->children['x']);
    std::cout<<std::endl;
}

bool sufix_tree::is_substring(std::string p){
    Node *path_node = root;
    std::unordered_map<char,Node*>::iterator it;
    it = path_node->children.find(p[0]);
    int checked = 1;
    while(it!=path_node->children.end()){
        path_node = it->second;
        int k = 1;
        while(checked < p.size() && path_node->beg+k<=path_node->end && p[checked] == base_str[path_node->beg+k]){
            checked++;
            k++;
        }

        if(checked == p.size())
            return true;
        else if(path_node->beg+k > path_node->end )
            it = path_node->children.find(p[checked++]);
        else{
            return false;
        }
    }
    return false;
}

int sufix_tree::occurrences(std::string p){
    Node *path_node = root;
    std::unordered_map<char,Node*>::iterator it;
    it = path_node->children.find(p[0]);
    int checked = 1;
    while(it!=path_node->children.end()){
        path_node = it->second;
        int k = 1;
        while(checked < p.size() && path_node->beg+k<=path_node->end && p[checked] == base_str[path_node->beg+k]){
            checked++;
            k++;
        }

        if(checked == p.size()){
           if(path_node->children.empty())
               return 1;
           else
               return path_node->under_leaves();
        }
        else if(path_node->beg+k > path_node->end )
            it = path_node->children.find(p[checked++]);
        else{
            return 0;
        }
    }
    return 0;
}

std::string sufix_tree::longest_repeat(){
    std::stack<Node*> stk;
    return "Node*";

}

int sufix_tree::longest_subsequence(std::string){
    return 0;
}



void sufix_tree::print_node(sufix_tree::Node *node)
{
    std::cout<<base_str.substr(node->beg,node->edge_size());
}
