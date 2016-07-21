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


bool Trie::buscar(const string &chave){
    return buscar_recursivo(this->raiz, chave, 0);
}


void Trie::inserir(const string &chave){
    if(!chave.size())
        this->raiz->ehPalavra = true;
    this->inserir_recursivo(this->raiz, chave, 0);
}

void Trie::remover(const string &chave){
    bool *flag = new bool(true);
    this->remover_recursivo(this->raiz, this->raiz, chave, 0, flag);
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

void Trie::remover_recursivo(No* pai, No *no, const string &chave, int ind, bool *flag){
    if(!chave[ind]){
        no->ehPalavra = false;
        return;

    }else if(no->ehFolha() && compare(chave, string(no->chave), ind)){
        pai->filhos->erase(pai->filhos->find(no->chave[ind-1]));
        delete no;
        return;

    }else{
        unordered_map<char, No*>::iterator it = no->filhos->find(chave[ind]);
        if(it != no->filhos->end())
            remover_recursivo(no, it->second, chave, ind+1, flag);
    }

    if(no->filhos->size() == 1 && !no->ehPalavra && no!=this->raiz && *flag){

        unordered_map<char, No*>::iterator it = no->filhos->begin();
        No* filho = it->second;
        if(filho->ehFolha()){
            (*pai->filhos)[chave[ind-1]] = filho;
            delete no;
        }

    }else
        *flag = false;

}

bool Trie::buscar_recursivo(No *no, const string &chave, int ind){
    if(!chave[ind])
        return (no->ehPalavra || no->ehFolha());
    else if(no->ehFolha())
        return compare(chave, string(no->chave), ind);
    else{
        unordered_map<char, No*>::iterator it = no->filhos->find(chave[ind]);
        if(it == no->filhos->end())
            return false;
        else
            return buscar_recursivo(it->second, chave, ind+1);

    }
}

