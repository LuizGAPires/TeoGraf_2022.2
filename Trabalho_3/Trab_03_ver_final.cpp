#include <iostream> // usado para "printar" no no terminal, utilizado durante os textes 
#include <ctime> // usado para ver o tempo para rodar os algoritimos
#include <list> // lista para a lista de adjacencia
#include <fstream> // biblioteca usada para leitura/escrita do arquivo .txt
#include <bits/stdc++.h> // utilizado para a função sort nos algoritimos de componentes conexas
#include <limits> //utilizado para o infinito, no caso ((2^31) - 1)
#include <time.h>
#include <math.h> 

using namespace std;

typedef pair<int, int> Par; // estrutura utilizada para armazenar o vértice e o peso de sua aresta na lista de adjacência

class Grafo
{
    bool direcionado;
    int V;
    list<Par> *adj;

public:
    Grafo(int V, bool direcionado); // construtor

    int countbfs = 0;

    int delta;

    vector<int> descobertos;

    vector<int> Pai;

    void addAresta(int v1, int v2, int W); //adiciona uma aresta no grafo

    int gargalo(list<int> caminho); // Encontra o gargalo do caminho

    bool bfs_mod(int v, int t); // busca em largura - gera um arquivo .txt da árvore gerada por ela

    int fordFulkerson(int v, int t, int delta2); // Loop principal
};
Grafo::Grafo(int V, bool direcionado)
{
    this->direcionado = direcionado;
    this->V = V;
    adj = new list<Par>[1, V+1];
}

void Grafo::addAresta(int v1, int v2, int W)
{
    if(direcionado) {
        Par p1 = make_pair(v2, W);
        adj[v1].push_back(p1);
    }
    else {
        Par p1 = make_pair(v2, W);
        Par p2 = make_pair(v1, W);
        adj[v1].push_back(p1);
        adj[v2].push_back(p2);
    }
}

int Grafo::gargalo(list<int> caminho) {
    int garga = 400000;

    list<int>::iterator it;
    for(it = caminho.begin(); it != caminho.end(); it++) {
        list<Par>::iterator it2;
        for(it2 = adj[*it].begin(); it2 != adj[*it].end(); it2++) {
            if((*it2).first == *(next(it, 1))) {
                if((*it2).second < garga) {
                    garga = (*it2).second;
                }
                break;
            }
        }
    }
    return garga;
}

bool Grafo::bfs_mod(int s, int t) // Se retornar falso é porque não tem mais como achar caminho
{
    queue<Par> Q;

    if(countbfs == 1) {
        descobertos.resize(V);
        Pai.resize(V);
        for(int i = 0; i < V; i++){
        descobertos[i] = 0;
        }
    }
    
    Pai[s-1] = 0;
    descobertos[s-1] = countbfs;
    int v = s;

    while (true)
    {
        list<Par>::iterator itc;
        for(itc = adj[v].begin(); itc != adj[v].end(); itc++)
        {   
            if(descobertos[(*itc).first-1] != countbfs && (*itc).second > delta)
            {
                Pai[(*itc).first-1] = v;
                if((*itc).first == t) {
                    int filho_aux = t;
                    list<int> caminho;
                    while(filho_aux != s) {        // Representando o caminho achado
                        caminho.push_front(filho_aux);
                        filho_aux = Pai[filho_aux-1];
                    }
                    caminho.push_front(s);
                    int garg = gargalo(caminho);  // Achei o gargalo do caminho
                    list<int>::iterator it;
                    for(it = caminho.begin(); it != prev(caminho.end(),1); it++) {     // Acrescimo ou criacao da aresta reversa 
                        list<Par>::iterator it2;
                        for(it2 = adj[*it].begin(); it2 != adj[*it].end(); it2++) {
                            if((*it2).first == *(next(it, 1))) {
                                (*it2).second = (*it2).second - garg;
                                if((*it2).second == 0) adj[*it].remove(*it2);
                                
                                list<Par>::iterator it3;
                                if(!adj[*(next(it, 1))].empty()){
                                    for(it3 = adj[*(next(it, 1))].begin(); it3 != adj[*(next(it, 1))].end(); it3++) {
                                    if((*it3).first == *it) {
                                        (*it3).second = (*it3).second + garg;
                                        break;
                                    }
                                    if(((it3 == adj[*(next(it, 1))].end()))) addAresta(*(next(it, 1)), *it, garg);
                                    }
                                }
                                else addAresta(*(next(it, 1)), *it, garg);
                                break;
                            }
                        }
                    }
                    
                    return true; // Consegui melhorar o caminho, vamos para o prox.
                }
                descobertos[(*itc).first-1] = countbfs;
                Q.push(*itc);
            }
        }

        if(!Q.empty())
        {
            v = (Q.front()).first;
            Q.pop();
        }
        else{
            return false;
        }
    }
}

int Grafo::fordFulkerson(int v, int t, int delta2) {
    bool loop = true; 
    int maxFluxo1 = 0;
    int maxFluxo2 = 0;
    delta = delta2;

    list<Par>::iterator it2;
    for(it2 = adj[v].begin(); it2 != adj[v].end(); it2++) {
        maxFluxo2 = maxFluxo2 + (*it2).second;
    }
    while(loop || delta > 1) {
        countbfs++;
        loop = bfs_mod(v,t);
        if(!loop && delta > 1) delta = delta/2;
    }
    
    list<Par>::iterator it1;
    if(!adj[v].empty()){
        for(it1 = adj[v].begin(); it1 != adj[v].end(); it1++) {
        maxFluxo1 = maxFluxo1 + (*it1).second;
        }
    }

    return maxFluxo2 - maxFluxo1;
}

int main(){
    clock_t t;
    int count = 0;
    int num, num1;
    int peso;
    int maiorpeso = 0;
    int v, delta;
    bool direcionado = true;
    fstream newfile, rfile;

   newfile.open("grafo_rf_8.txt" ,ios::in); //abre o arquivo .txt apenas para leitura
   if (newfile.is_open())
   {    
        newfile >> v;
        
            Grafo graphR(v, direcionado);   // Representação do grafo residual
            do{
            newfile >> num;
            newfile >> num1;
            newfile >> peso;
            if (!newfile.fail()) {
                count++;
                graphR.addAresta(num, num1, peso);
                if(count <= 20 && peso > maiorpeso) maiorpeso = peso;

            }
            }while(!newfile.fail());
            delta = pow(2,log2(maiorpeso));
            t = clock();
            cout << graphR.fordFulkerson(1,2,delta);
            t = clock() - t;
            cout << " ";
            cout << t/1000;
        }
        newfile.close();
        return 1;
}