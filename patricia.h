#ifndef PATRICIA_H
#define PATRICIA_H

#include <string>
#include <stack>
#include <unordered_map>
#include <queue>
#include <vector>

namespace ED
{
    struct Aresta;

    typedef union
    {
        char*                             chave;
        std::unordered_map<char, Aresta>* filhos;
    } Conteudo;

    /////////////////////////////////////////////////////
    /// \brief The No struct
    ///
    struct No
    {
        bool     ehFolha;
        Conteudo conteudo;

        No  ();
        No  (const std::string &chave);
        ~No ();
    };

    /////////////////////////////////////////////////////
    /// \brief The Aresta struct
    ///
    struct Aresta
    {
        int   ip;
        char* chave;
        No*   filho;

        Aresta();
        Aresta(const int ip, No * filho = nullptr);
    };

    /////////////////////////////////////////////////////
    /// \brief The Patricia class
    ///
    class Patricia
    {
    public:

        /////////////////////////////////////////////////////
        /// \brief Patricia
        ///
        Patricia  ();

        /////////////////////////////////////////////////////
        /// \brief ~Patricia
        ///
        ~Patricia ();

        /////////////////////////////////////////////////////
        /// \brief  inserir
        /// \param  chave
        /// \return insere se a chave não estiver na
        ///         estrutura, se ela não for prefixo
        ///         de alguma chave que estiver, ou se
        ///         não houver uma chave que seja
        ///         prefixo da chave a ser inserida.
        bool inserir (const std::string &chave);

        /////////////////////////////////////////////////////
        /// \brief  buscar
        /// \param  chave
        /// \return true se achou, false caso contrário
        ///
        bool buscar  (const std::string &chave);

        /////////////////////////////////////////////////////
        /// \brief  remover
        /// \param  chave
        /// \return true se removeu, false caso contrário
        ///
        bool remover (const std::string &chave);

        /////////////////////////////////////////////////////
        /// \brief  listarPorPrefixo
        /// \param  chave
        /// \return vetor com todas as chaves que tem
        ///         a chave recebida como prefixo
        ///
        std::vector<std::string> * listarPorPrefixo(
                const std::string &chave);

        /////////////////////////////////////////////////////
        /// \brief  maiorPrefixo
        /// \param  chave
        /// \return a chave mais longa que eh prefixo
        ///         da chave recebida
        ///
        std::string maiorPrefixo(const std::string &chave);

        /////////////////////////////////////////////////////
        /// \brief  getTamanho
        /// \return quantidade de chaves na arvore
        ///
        unsigned int getTamanho() const;

        /////////////////////////////////////////////////////
        /// \brief The iterator struct
        ///
        struct iterator
        {
            std::stack<No*> caminho;
            std::stack<std::unordered_map<char, Aresta>::iterator> filho;

            bool operator!=(iterator outro);

            std::string operator*() const;

            iterator operator++(int);

            iterator operator++();

        };

        /////////////////////////////////////////////////////
        /// \brief begin
        /// \return iterador para a primeira chave
        ///         da estrutura
        ///
        iterator begin();

        /////////////////////////////////////////////////////
        /// \brief end
        /// \return retorna o iterador end (nenhum elemento
        ///         válido)
        ///
        iterator end();

    private:
        /////////////////////////////////////////////////////
        /// \brief raiz da arvore
        ///
        No* raiz;

        /////////////////////////////////////////////////////
        /// \brief numero de chaves na arvore
        ///
        unsigned int tamanho{0};

        /////////////////////////////////////////////////////
        /// \brief __inserir
        /// \param no
        /// \param chave
        /// \param posicao
        /// \return retorna true se inserir a chave, false
        ///         caso contrário
        ///
        bool __inserir (No*& no, const std::string &chave, int posicao);

        /////////////////////////////////////////////////////
        /// \brief __buscar
        /// \param no
        /// \param chave
        /// \param posicao
        /// \param caminho
        /// \return retorna o no da chave, se a encontrar
        ///         na estrutura
        ///
        No*  __buscar  (No*& no, const std::string &chave,
                        int posicao, std::stack<No*> &caminho);

        /////////////////////////////////////////////////////
        /// \brief __listarChaves
        /// \param no
        /// \return lista todas as chaves pertencentes a
        ///         subarvore enraizada em no
        ///
        std::vector<std::string> * __listarChaves(No *no);

        /////////////////////////////////////////////////////
        /// \brief __destruir: desaloca todos os nos da
        ///        estrutura
        /// \param no
        ///
        void __destruir(No *no);

    };
}


#endif // PATRICIA_H
