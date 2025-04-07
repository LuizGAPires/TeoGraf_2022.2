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
    clock_t ini;
    bool explorados[V];
    int menor, vert, s;
    ini = clock();
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
    ini = clock() - ini;
    distancias.open("djikstra.txt", ios::out);
    if(distancias.is_open()){
        distancias << "Tempo de execução: " << (double(ini))/CLOCKS_PER_SEC << endl;
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
    clock_t ini;
    ini = clock();
    vector<int> pai(V);
    priority_queue<Par, vector<Par>, greater<Par> > heap;
    vector<float> dist(V, std::numeric_limits<float>::infinity());
    dist[v-1] = 0;
    pai[v-1] = -1;


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
                pai[vert-1] = u;
            }
        }
    }
    ini = clock() - ini;
    distancias.open("dijkstraHeap.txt", ios::out);
    if(distancias.is_open()){
        distancias << "Tempo de execução: " << (double(ini))/CLOCKS_PER_SEC << endl;
        for(int i = 0; i < V; i++){
            //distancias << "Distância do vértice " << v << " até o vértice " << i+1 << ": " << dist[i] << endl;
            distancias << "Pai de " << i+1 << ": " << pai[i] << endl;
        }
    }distancias.close();
}

void Grafo::primMST(int v)
{
    fstream MST;
    float pesos[V], peso, pesot = 0;
    bool explorados[V];
    int vert, s, pai[V];
    s = v;
    for(int i = 0; i < V; i++){
        pesos[i] = std::numeric_limits<float>::infinity();
        explorados[i] = false;
        pai[i] = -1;
        }
    pesos[v-1] = 0;

    for (int cont = 0; cont < V-1; cont++){
        int u = minimo(pesos, explorados) + 1;
        explorados[u-1] = true;

        list<Par>::iterator it;
        for(it = adj[u].begin(); it != adj[u].end(); it++){
            vert = it->first;
            peso = it->second;
            if(explorados[vert-1] == false && pesos[vert - 1] > peso){
                pesos[vert-1] = peso;
                pai[vert-1] = u;
            }
        }
    }
    MST.open("Mst.txt", ios::out);
    if(MST.is_open()){
        for(int i = 0; i < V; i++){
            pesot += pesos[i];
            MST << "Pai de " << i+1 << ": " << pai[i] << endl;
        }
        MST << "Peso total: " << pesot << endl;
    }MST.close();
}

int main(){
    int count = 0;
    bool pesoNegativo = false;
    int num, num1;
    float peso;
    int v;
   fstream newfile, rfile;

   newfile.open("rede_colaboracao.txt" ,ios::in); //abre o arquivo .txt apenas para leitura
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
                //graph.dijkstra(10);
                graph.dijkstraHeap(2722);
            }
            //graph.primMST(1);
        }
        newfile.close();
        return 1;
} 
