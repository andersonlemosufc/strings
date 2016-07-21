#include "trie.h"

bool compare(const string &s1, const string &s2, int i){    
    int tam_s1 = (int) s1.size();
    int tam_s2 = (int) s2.size();
    if(tam_s1 == tam_s2){
        for(int j = i; j < tam_s1; j++){
            if(s1[j] != s2[j])
                return false;
        }
        return true;
    }
    return false;
}


void No::setString(const string &chave){
    int tamanho = int(chave.size());
    this->chave = new char[tamanho+1];
    for(int i = 0; i < tamanho; i++){
        this->chave[i] = chave[i];
    }
    this->chave[tamanho] = '\0';

    if (this->filhos == nullptr)
        delete this->filhos;
    this->filhos = nullptr;
}


bool No::ehFolha(){
    return this->filhos == nullptr;
}

No::No(){
    this->ehPalavra = false;
    this->filhos = new unordered_map<char, No*>();
    this->chave = nullptr;
}

No::No(const string &chave){
    this->ehPalavra = false;
    this->filhos = nullptr;
    this->setString(chave);
}

No::~No(){
    if(this->filhos == nullptr){
           delete this->chave;
       }
       else{
           delete this->filhos;
       }
}

Trie::Trie(){    
    this->raiz = new No();
}

Trie::~Trie(){    
    delete this->raiz;
}


bool Trie::buscar(const string &chave){
    return buscar_recursivo(this->raiz, chave, 0);
}


void Trie::inserir(const string &chave){
    if(!chave.size())
        this->raiz->ehPalavra = true;
    this->inserir_recursivo(this->raiz, chave, 0);
}

void Trie::remover(const string &chave){
    bool *corrigir = new bool(false);
    bool *deletado = new bool(false);
    this->remover_recursivo(this->raiz, chave, 0, deletado, corrigir);
}

void Trie::inserir_recursivo(No *no, const string &chave, int ind){
    if(no->ehFolha() && !compare(chave, no->chave, ind)){
        No* atual = no;
        while(chave[ind] == no->chave[ind] && no->chave[ind]){
            No* novo = new No();
            atual->filhos = new unordered_map<char, No*>();
            (*atual->filhos)[chave[ind]] = novo;
            atual = novo;
            ind++;
        }
        if(!chave[ind]){
            atual->ehPalavra = true;
            atual->setString(string(no->chave));
            delete no->chave;

        }else if(!no->chave[ind]){
            atual->ehPalavra = true;
            atual->setString(chave);
            delete no->chave;

        }else{
            No* novo = new No(string(no->chave));
            atual->filhos = new unordered_map<char, No*>();
            (*atual->filhos)[no->chave[ind]] = novo;
            delete no->chave;
            novo = new No(chave);
            (*atual->filhos)[chave[ind]] = novo;
        }

    }
    else{
        unordered_map<char, No*>::iterator it = no->filhos->find(chave[ind]);
        if(it == no->filhos->end()){
            (*no->filhos)[chave[ind]] = new No(chave);

        }else{
            this->inserir_recursivo(it->second, chave, ind+1);
        }
    }

}

void Trie::remover_recursivo(No *no, const string &chave, int ind, bool *deletado, bool *corrigir){
    if(!chave[ind]){
        no->ehPalavra = false;
        *deletado = true;
        *corrigir = true;
        return;

    }else if(no->ehFolha() && compare(chave, string(no->chave), ind)){
        *corrigir = true;
        return;

    }else{
        unordered_map<char, No*>::iterator it = no->filhos->find(chave[ind]);
        if(it != no->filhos->end())
            remover_recursivo(it->second, chave, ind+1, deletado, corrigir);
    }

    if(!*deletado){
        unordered_map<char, No*>::iterator it = no->filhos->find(chave[ind]);
        No* filho = it->second;
        if (filho->ehPalavra){
            delete filho->chave;
            filho->chave = new char[ind+2];
            chave.copy(filho->chave, ind+1, 0);
            filho->chave[ind+1] = '\0';
            filho->ehPalavra = false;

        }else{
            no->filhos->erase(it);
            delete filho;
        }
        *deletado = true;
    }

    if(*corrigir){
        if(no->filhos->size()==1 && no!=this->raiz){
            unordered_map<char, No*>::iterator it = no->filhos->begin();
            No* filho = it->second;
            if  (filho->ehFolha() && !filho->ehPalavra){
                no->chave = filho->chave;
                filho->chave = nullptr;
                no->filhos->erase(it);
                delete filho;
                no->filhos=nullptr;
                return;
            }

        }
        *corrigir = false;
    }

}

bool Trie::buscar_recursivo(No *no, const string &chave, int ind){
    if(!chave[ind])
        return (no->ehPalavra);
    else if(no->ehFolha()){
        cout <<"teste "<<chave<<" "<<compare(chave, string(no->chave), ind)<<"\n";
        return compare(chave, string(no->chave), ind);
    }
    else{
        unordered_map<char, No*>::iterator it = no->filhos->find(chave[ind]);
        if(it == no->filhos->end())
            return false;
        else
            return buscar_recursivo(it->second, chave, ind+1);

    }
}

