#include<iostream> // usado para "printar" no no terminal, utilizado durante os textes 
#include<ctime> // usado para ver o tempo para rodar os algoritimos
#include<list> // lista para a lista de adjacencia
#include <fstream> // biblioteca usada para leitura/escrita do arquivo .txt
#include <bits/stdc++.h> // utilizado para a função sort nos algoritimos de componentes conexas
#include <limits> //utilizado para o infinito, no caso ((2^31) - 1)

using namespace std;

typedef pair<int, float> Par; // estrutura utilizada para armazenar o vértice e o peso de sua aresta na lista de adjacência

class Grafo
{
    int V;
    list<Par> *adj;

public:
    Grafo(int V); // construtor

    void addAresta(int v1, int v2, float W); //adciona uma aresta no grafo

    void dijkstra(int v);

    void dijkstraHeap(int v);

    int minimo(float f[], bool b[]);

    void primMST(int v);
};
Grafo::Grafo(int V)
{
    this->V = V;
    adj = new list<Par>[1, V+1];
}

void Grafo::addAresta(int v1, int v2, float W)
{
    Par p1 = make_pair(v2, W);
    Par p2 = make_pair(v1, W);
    adj[v1].push_back(p1);
    adj[v2].push_back(p2);
}

int Grafo::minimo(float f[], bool b[])
{
    float min = std::numeric_limits<float>::infinity();
    int index;
    for(int i = 0; i < V; i++){
        if(b[i] == false && f[i] < min){
            index = i;
            min = f[i];
        }
    }
    return index;
}

void Grafo::dijkstra(int v)
{
    fstream distancias;
    float dist[V], peso;
    float min = std::numeric_limits<float>::infinity();
    bool explorados[V];
    int menor, vert, s;
    s = v;
    for(int i = 0; i < V; i++){
        dist[i] = std::numeric_limits<float>::infinity();
        explorados[i] = false;
    }
    dist[v-1] = 0;

    while(!explorados[v-1])
    {
        list<Par>::iterator it;
        for(it = adj[v].begin(); it != adj[v].end(); it++)
        {
            vert = it->first;
            peso = it->second;
            if(dist[vert-1] > dist[v-1] + peso){
                dist[vert-1] = dist[v-1] + peso;
            }

        }
        explorados[v-1] = true;
        v = minimo(dist, explorados) + 1;
    }
    distancias.open("djikstra.txt", ios::out);
    if(distancias.is_open()){
        for(int i = 0; i < V; i++){
            distancias << "Distância do vértice " << s << " até o vértice " << i+1 << ": " << dist[i] << endl;
        }
    }distancias.close();
}

void Grafo::dijkstraHeap(int v)
{
    fstream distancias;
    int u, vert;
    float peso;
    priority_queue<Par, vector<Par>, greater<Par> > heap;
    vector<float> dist(V, std::numeric_limits<float>::infinity());
    dist[v-1] = 0;

    list<Par>::iterator it;
    heap.push({0, v});
    while (!heap.empty())
    {
        u = heap.top().second;
        heap.pop();

        for(it = adj[u].begin(); it != adj[u].end(); it++){
            vert = it->first;
            peso = it->second;
            if(dist[vert-1] > dist[u-1] + peso){
                dist[vert-1] = dist[u-1] + peso;
                heap.push({dist[vert-1], vert});
            }
        }
    }
    distancias.open("dijkstraHeap.txt", ios::out);
    if(distancias.is_open()){
        for(int i = 0; i < V; i++){
            distancias << "Distância do vértice " << v << " até o vértice " << i+1 << ": " << dist[i] << endl;
        }
    }distancias.close();
}

void Grafo::primMST(int v)
{
    int u, vert;
    float peso, pesot;
    fstream MST;
    priority_queue< Par, vector<Par> , greater<Par> > heap;
    vector<float> chave(V, std::numeric_limits<float>::infinity());
    vector<int> pai(V, -1);
    vector<bool> explorados(V, false);

    heap.push({0, v});
    chave[v-1] = 0;

    while(!heap.empty())
    {
        u = heap.top().second;
        heap.pop();

        if(explorados[u-1] == true){
            continue;
        }

        explorados[u-1] = true;
        list<Par>::iterator it;
        for (it = adj[u].begin(); it != adj[u].end(); it++){
            vert = it->first;
            peso = it->second;

            if(explorados[vert - 1] == false && chave[vert-1] > peso){
                chave[vert - 1] = peso;
                heap.push(make_pair(chave[vert-1], vert));
                pai[vert-1] = u;
            }
        }
    }

    
    MST.open("MST.txt", ios::out);
    if(MST.is_open()){
        for(int i = 0; i < V; i++){
            pesot += chave[i];
            MST << "Pai de " << i+1 << ": " << pai[i] << endl;
        }
        MST << "Peso total: " << pesot;
    }
    MST.close();
}


int main(){
    int count = 0;
    bool pesoNegativo = false;
    int num, num1;
    float peso;
    int v;
   fstream newfile, rfile;

   newfile.open("grafo_W_1_1.txt" ,ios::in); //abre o arquivo .txt apenas para leitura
   if (newfile.is_open())
   {    
        newfile >> v;
        
            Grafo graph(v);
            do{
            newfile >> num;
            newfile >> num1;
            newfile >> peso;
            if (!newfile.fail()) {
                count++;
                graph.addAresta(num, num1, peso);
                if(peso < 0){
                    pesoNegativo = true;
                }
                
            }
            }while(!newfile.fail());
            if(pesoNegativo){
                cout << "Não é possível calcular as distâncias com os algoritmos atuais";
            }
            else{
                graph.dijkstra(10);
                //graph.dijkstraHeap(10);
            }
            graph.primMST(10);
        }
        newfile.close();
} 