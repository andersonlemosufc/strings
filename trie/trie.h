#ifndef TRIE_H
#define TRIE_H
//#include<map>
#include <unordered_map>
#include<string>
#include<iostream>
using namespace std;

struct No;

//typedef union{
//    char* chave;
//    unordered_map<char, No>* filhos;
//} Conteudo;


struct No{
    bool ehPalavra;
//    bool ehFolha;
//    Conteudo conteudo;
    char* chave;
    unordered_map<char, No*>* filhos;

    No();
    No (const string &chave);
    ~No();
    void setString(const string &chave);
    bool ehFolha();
};

class Trie{
public:
    Trie();

    bool buscar(const string &chave);
    void inserir(const string &chave);
    void remover(const string &chave);

private:
    No* raiz;
    unsigned int tamanho{0};
    void inserir_recursivo(No *no, const string &chave, int ind);
    void remover_recursivo(No * pai, No *no, const string &chave, int ind, bool *flag);
    bool buscar_recursivo(No *no, const string &chave, int ind);


};

#endif // TRIE_H

