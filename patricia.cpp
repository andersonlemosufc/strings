#include "patricia.h"

// _________________________ ARVORE _________________________
ED::Patricia::Patricia()
{
    raiz = new No();
}

ED::Patricia::~Patricia()
{
    __destruir(raiz);
}

bool ED::Patricia::inserir(const std::string &chave)
{
    bool inseriu = __inserir(raiz, chave, 0);
    tamanho += int(inseriu);
    return inseriu;
}

bool ED::Patricia::buscar(const std::string &chave)
{
    std::stack<No*> caminho;
    if (__buscar(raiz, chave, 0, caminho) != nullptr)
        return true;
    return false;
}

bool ED::Patricia::remover(const std::string &chave)
{
    std::stack<No*> caminho;
    No * no = __buscar(raiz, chave, 0, caminho);

    if (no != nullptr)
    {
        No *pai = caminho.top();
        caminho.pop();
        for (auto it = pai->conteudo.filhos->begin();
             it != pai->conteudo.filhos->end(); it++)
        {
            if (it->second.filho == no)
            {
                delete no;
                pai->conteudo.filhos->erase(it);
                break;
            }
        }

        if (pai != raiz)
        {
            Aresta aresta = pai->conteudo.filhos->begin()->second;
            char * outraChave = aresta.chave;

            // comprimindo a arvore
            if (int(pai->conteudo.filhos->size()) == 1)
            {
                no = pai;
                pai = caminho.top();
                caminho.pop();

                for (auto it = pai->conteudo.filhos->begin();
                     it != pai->conteudo.filhos->end(); it++)
                {
                    if (it->second.filho == no)
                    {
                        it->second = aresta;
                        delete no;
                        break;
//                        pai->conteudo.filhos->erase(it);
                    }
                }
            }

            // consertando as chaves do caminho
            while (!caminho.empty())
            {
                no = pai;
                pai = caminho.top();
                caminho.pop();
                for (auto it = pai->conteudo.filhos->begin();
                     it != pai->conteudo.filhos->end(); it++)
                {
                    if (it->second.filho == no)
                    {
                        it->second.chave = outraChave;
                    }
                }
            }
        }

        while (!caminho.empty())
        {
            pai = caminho.top();
            caminho.pop();
        }

        tamanho--;
        return true;
    }

    return false;
}

std::vector<std::string> *ED::Patricia::listarPorPrefixo(const std::string &chave)
{
    if (chave.empty())
        return __listarChaves(raiz);

    std::queue<No*> fila;
    fila.push(raiz);
    int posicao = 0;

    while (!fila.empty())
    {
        No *no = fila.front();
        fila.pop();

        if (!no->ehFolha)
        {
            std::unordered_map<char, Aresta>::iterator it =
                    no->conteudo.filhos->find(chave[posicao]);

            if (it != no->conteudo.filhos->end())
            {
                Aresta * aresta = &it->second;

                while (true)
                {
                    if (posicao == int(chave.size()))
                        return __listarChaves(aresta->filho);

                    if (posicao == aresta->ip)
                        break;

                    if (chave[posicao] != aresta->chave[posicao])
                        return nullptr;

                    posicao++;
                }

                fila.push(aresta->filho);
            }
        }
        else
        {
            if (int(chave.size()) == posicao)
                return __listarChaves(no);
        }

    }

    return nullptr;
}

std::string ED::Patricia::maiorPrefixo(const std::string &chave)
{
    std::queue<No*> fila;
    fila.push(raiz);
    int posicao = 0;

    while (!fila.empty())
    {
        No *no = fila.front();
        fila.pop();

        if (!no->ehFolha)
        {
            std::unordered_map<char, Aresta>::iterator it =
                    no->conteudo.filhos->find(chave[posicao]);

            if (it != no->conteudo.filhos->end())
            {
                Aresta * aresta = &it->second;

                while (true)
                {
                    if (posicao == aresta->ip)
                        break;

                    if (posicao == int(chave.size()))
                        return "";

                    if (chave[posicao] != aresta->chave[posicao])
                        return "";

                    posicao++;
                }

                fila.push(aresta->filho);
            }
        }
        else
        {
            return no->conteudo.chave;
        }
    }

    return "";
}

unsigned int ED::Patricia::getTamanho() const
{
    return tamanho;
}

ED::Patricia::iterator ED::Patricia::begin()
{
    iterator it;
    it.caminho.push(raiz);
    ++it;
    return it;
}

ED::Patricia::iterator ED::Patricia::end()
{
    return iterator();
}

bool ED::Patricia::__inserir(ED::No *&no, const std::string &chave, int posicao)
{
        if (!no->ehFolha)
        {
            auto it = no->conteudo.filhos->find(chave[posicao]);

            if (it != no->conteudo.filhos->end())
            {
                Aresta * aresta = &it->second;

                while (true)
                {
                    if (posicao == int(chave.size()))
                        return false;

                    if (posicao == aresta->ip)
                        return __inserir(aresta->filho, chave, posicao);

                    if (chave[posicao] != aresta->chave[posicao])
                        break;

                    posicao++;
                }

                // criar novo nó
                No * novoNo = new No;
                Aresta novaAresta(aresta->ip, aresta->filho);
                novaAresta.chave = aresta->chave;

                // novo nó adiciona aresta para aresta.filho
                (*novoNo->conteudo.filhos)[aresta->chave[posicao]] =
                        novaAresta;

                // atualiza o ip da aresta
                aresta->ip = posicao;

                // aresta filho = novo nó
                aresta->filho = novoNo;

                return __inserir(novoNo, chave, posicao);
            }
            else
            {
                ED::Aresta aresta(int(chave.size()));
                aresta.filho = new No(chave);
                aresta.chave = aresta.filho->conteudo.chave;

                (*no->conteudo.filhos)[chave[posicao]] = aresta;
                return true;
            }
        }

    return false;
}

ED::No * ED::Patricia::__buscar(ED::No *&no, const std::string &chave,
                                int posicao, std::stack<No*> &caminho)
{
    if (no->ehFolha)
    {
        if (int(chave.size()) == posicao)
        {
            return no;
        }
    }
    else
    {
        caminho.push(no);
        std::unordered_map<char, Aresta>::iterator it =
                no->conteudo.filhos->find(chave[posicao]);

        if (it != no->conteudo.filhos->end())
        {
            Aresta * aresta = &it->second;

            if (int(chave.size()) < aresta->ip)
                return nullptr;

            while (posicao < aresta->ip)
            {
                if (chave[posicao] != aresta->chave[posicao])
                    return nullptr;
                posicao++;
            }

            return __buscar(aresta->filho, chave, posicao, caminho);
        }
    }

    return nullptr;
}

std::vector<std::string> *ED::Patricia::__listarChaves(ED::No *no)
{
    std::vector<std::string> *chaves = new std::vector<std::string>();
    std::queue<No*> fila;
    fila.push(no);

    while (!fila.empty())
    {
        No *no = fila.front();
        fila.pop();

        if (no->ehFolha)
        {
            chaves->push_back(no->conteudo.chave);
        }
        else
        {
            for (std::unordered_map<char, Aresta>::iterator it =
                 no->conteudo.filhos->begin();
                 it != no->conteudo.filhos->end(); it++)
            {
                fila.push(it->second.filho);
            }
        }
    }

    return chaves;
}

void ED::Patricia::__destruir(ED::No *no)
{
    if (!no->ehFolha)
    {
        for (std::unordered_map<char, Aresta>::iterator it =
             no->conteudo.filhos->begin();
             it != no->conteudo.filhos->end(); it++)
        {
            __destruir(it->second.filho);
        }
    }

    delete no;
}

// _________________________ NO _________________________
ED::No::No()
{
    this->ehFolha = false;
    this->conteudo.filhos = new std::unordered_map<char, Aresta>();
}

ED::No::No(const std::string &chave)
{
    this->ehFolha = true;

    int size = int(chave.size());
    this->conteudo.chave = new char[size + 1];

    for (int i = 0 ;i < size; i++)
        this->conteudo.chave[i] = chave[i];
    this->conteudo.chave[size] = '\0';
}

ED::No::~No()
{
    if (this->ehFolha)
        delete this->conteudo.chave;
    else
        delete this->conteudo.filhos;
}

// _________________________ ARESTA _________________________
ED::Aresta::Aresta()
{

}

ED::Aresta::Aresta(const int ip, No *filho)
{
    this->ip = ip;
    this->filho = filho;
}

// _________________________ ITERATOR _________________________
bool ED::Patricia::iterator::operator!=(ED::Patricia::iterator outro)
{
    iterator eu = *this;

    while (!eu.caminho.empty() && !outro.caminho.empty())
    {
        if (eu.caminho.top() != outro.caminho.top())
            return true;
    }

    if (eu.caminho.size() != outro.caminho.size())
        return true;

    return false;
}

std::string ED::Patricia::iterator::operator*() const
{
    return std::string(caminho.top()->conteudo.chave);
}

ED::Patricia::iterator ED::Patricia::iterator::operator++()
{
    if (caminho.top()->ehFolha)
        caminho.pop();

    while (!caminho.empty())
    {
        // se eh folha
        if (caminho.top()->ehFolha)
        {
            break;
        }
        else
        {
            if (filho.size() == caminho.size())
            {
                filho.top()++;
                if (filho.top() != caminho.top()->conteudo.filhos->end())
                {
                    caminho.push(filho.top()->second.filho);
                }
                else
                {
                    caminho.pop();
                    filho.pop();
                }
            }
            else
            {
                filho.push(caminho.top()->conteudo.filhos->begin());
                caminho.push(filho.top()->second.filho);

            }
        }
    }

    return *this;
}

ED::Patricia::iterator ED::Patricia::iterator::operator++(int)
{
    iterator temp = *this;
    ++(*this);
    return temp;
}
