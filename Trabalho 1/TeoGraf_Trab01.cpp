#include<stack> // pilha para DFS
#include<iostream> // usado para "printar" no no terminal, utilizado durante os textes 
#include<ctime> // usado para ver o tempo para rodar os algoritimos
#include<list> // lista para a lista de adjacencia
#include<queue> // fila para BFS 
#include <fstream> // biblioteca usada para leitura/escrita do arquivo .txt
#include <bits/stdc++.h> // utilizado para a função sort nos algoritimos de componentes conexas



using namespace std;

class Grafo // grafo feito com lista de adjacência
{
    int V; // Número de Vértices
    list<int> *adj; //Ponteiro para um array contendo as listas de adjacências

public:
    Grafo(int V); // construtor

    void addAresta(int v1, int v2); //adciona uma aresta no grafo

    int grau(int v); // retorna o grau do vertice v

    int gmax(); // retorna o maior grau do grafo

    float media_grau(); // retorna a média de todos os graus

    float mediana_grau(); //retorna a mediana dos graus

    void dfs(int v); // busca em profundidade - gera um arquivo .txt da árvore gerada por ela

    void bfs(int v); // busca em largura - gera um arquivo .txt da árvore gerada por ela

    int dist(int v1, int v2); // retorna a distância entre os vertices v1 e v2

    int diametro(); // retorna a maior distância entre qualquer dois vertices

    list<list<int> > conexa(); // retorna uma lista com todas as componentes conexas do grafo

};

Grafo::Grafo(int V)
    {
        this->V = V;
        adj = new list<int>[1, V+1];
    }

void Grafo::addAresta(int v1, int v2)
    {
        adj[v1].push_back(v2);
        adj[v2].push_back(v1);
    }

int Grafo::grau(int v)
{
        return adj[v].size();
}

int Grafo::gmax()
{
    int max = 0;
    int atual;
    int vmax;

    for(int i = 0; i < V; i++){
        atual = grau(i+1);
        if(atual > max){
            max = atual;
            vmax = i+1;
        }
    }
    return max;
}

float Grafo::media_grau()
{
    float soma = 0;
    for(int i = 0; i < V; i++){
        soma = soma + grau(i+1);
    }
    return (soma/V);
}

float Grafo::mediana_grau()
{
    float x[V];

    for(int i = 0; i < V; i++){
        x[i] = grau(i+1);
    }

    sort(x, x+V);

    if(V%2 == 0){
        return (x[(V/2)] + x[(V/2) - 1])/2;
    }
    else{
        return x[V/2];
    }
}

void Grafo::dfs(int v)
{
    stack<int> P;
    int L[V];
    int Pai[V];
    clock_t ini;
    fstream arvoredfs;
    bool explorados[V]; // vetor de explorados

    ini = clock();
    for(int i = 0; i < V; i++)
        explorados[i] = false;
    
    L[v-1] = 0;
    Pai[v-1] = 0;

    while (true)
    {
        if(!explorados[v-1])
        {
            explorados[v-1] = true;
            P.push(v);
        }
        
        bool descoberto = false;
        list<int>::iterator it;

        for(it = adj[v].begin(); it != adj[v].end(); it++)
        {
            if(!explorados[*it-1])
            {
                L[*it-1] = L[v-1] + 1;
                Pai[*it-1] = v;
                descoberto = true;
                break;
            }
        }

        if(descoberto)
            v = *it;
        else
        {
            P.pop();

            if(P.empty())
                break;
            
            v = P.top();
        }
        
    }

    ini = clock() - ini;
    arvoredfs.open("dfs.txt", ios::out);
    if(arvoredfs.is_open()){
        arvoredfs << (float)ini/CLOCKS_PER_SEC << " segundos" << endl;
        for(int i = 0; i < V; i++){
        arvoredfs << "Pai de " << i+1 << ": " << Pai[i] << " | " << "Nível de " << i+1 << ": " << L[i] << endl; 
    }arvoredfs.close();
}
}

void Grafo::bfs(int v)
{
    queue<int> Q;
    fstream arvorebfs;
    int L[V];
    int Pai[V];
    clock_t ini;
    bool explorados[V];

    ini = clock();

    for(int i = 0; i < V; i++){
        explorados[i] = false;
        }
    
    explorados[v-1] = true;
    L[v-1] = 0;
    Pai[v-1] = 0;

    while (true)
    {
        list<int>::iterator it;
        for(it = adj[v].begin(); it != adj[v].end(); it++)
        {
            if(!explorados[*it-1])
            {
                L[*it-1] = L[v-1] + 1;
                Pai[*it-1] = v;
                explorados[*it-1] = true;
                Q.push(*it);
            }
        }

        if(!Q.empty())
        {
            v = Q.front();
            Q.pop();
        }
        else
            break;
    }
    

    ini = clock() - ini;

    arvorebfs.open("bfs.txt", ios::out);
    if(arvorebfs.is_open()){
        arvorebfs << (float)ini/CLOCKS_PER_SEC << " segundos" << endl;
        for(int i = 0; i < V; i++){
        arvorebfs << "Pai de " << i+1 << ": " << Pai[i] << " | " << "Nível de " << i+1 << ": " << L[i] << endl; 
    }arvorebfs.close();
}
    
}

int Grafo::dist(int v1, int v2)
{
    queue<int> Q;
    bool explorados[V];
    int L[V];
    bool cheguei = false;

    for(int i = 0; i < V; i++)
        explorados[i] = false;
    
    explorados[v1-1] = true;
    L[v1-1] = 0;

    while (true)
    {
        list<int>::iterator it;
        for(it = adj[v1].begin(); it != adj[v1].end(); it++)
        {   
            if(!explorados[*it-1])
            {
                L[*it-1] = L[v1-1] + 1;
                if(*it == v2)
                {
                    cheguei = true;
                    break;
                }
                explorados[*it-1] = true;
                Q.push(*it);
            }
        }

        if(cheguei)
            break; 

        if(!Q.empty())
        {
            v1 = Q.front();
            Q.pop();
        }
        else
            return -1;
        
    }
    return L[v2-1];
}

int Grafo::diametro()
{
    int depth;
    int diam = 0;
    for(int i=1; i < V+1; i++){
        for(int j=1; j < V+1; j++){
            if(i < j){
                depth = dist(i, j);
                if(depth > diam){
                    diam = depth;
                }
            }
        }
    }
    return diam;
}

list<list<int> > Grafo::conexa()
{   
    int v = 1;
    int iaux = 0;
    bool achados[V], completo = false;
    list<list<int> > componentes;

    for(int i = 0; i < V; i++)
        achados[i] = false;

    while(!completo){
        list<int> comp;
        stack<int> P;
        bool explorados[V]; // vetor de explorados

        for(int i = 0; i < V; i++)
            explorados[i] = false;
    
        while (true)
        {
            if(!explorados[v-1])
            {
                achados[v-1] = true;
                explorados[v-1] = true;
                comp.push_back(v);
                P.push(v);
            }
        
            bool descoberto = false;
            list<int>::iterator it;

            for(it = adj[v].begin(); it != adj[v].end(); it++)
            {
                if(!explorados[*it-1])
                {
                    descoberto = true;
                    break;
                }
            }

            if(descoberto)
                v = *it;
            else
            {
                P.pop();

                if(P.empty()){
                    componentes.push_back(comp);
                    break;
                }
            
                v = P.top();
            }        
        }

        for(int i = iaux; i < V; i++){
            
            if(achados[i] == false){
                iaux = i;
                v = i + 1;
                break;
            }
            else if (i == V-1)
            {
                completo = true;
            }
            
        }
    }
    
list<list<int> >::iterator itr, it;
    for(itr = componentes.begin(); itr != componentes.end(); itr++){
        int tam = 0;
        list<int>x = *itr;
        for(it = itr; it != componentes.end(); it++)
        {
            list<int>tl = *it;
            int atual = tl.size();
            if(atual > tam){
                tam = atual;
                *itr = tl;
                *it = x;
            }

        }
        
    }/**/
    return componentes;
}

class GrafoM // grafo com matriz de adjacência
{
    bool** M; //Ponteiro que aponta para outro ponteiro que aponta para um vetor de booleanos
    int V; // quantidade dos vértices

public:
    GrafoM(int V); // construtor

    void addAresta(int v1, int v2); // cria uma aresta entre os vértices v1 e v2

    int grau(int v); // retorna o grau do vértice v

    int gmax(); // retorna o maior grau do grafo

    float media_grau(); // retorna a média dos graus do grafo

    float mediana_grau(); // retorna a mediana dos graus do grafo

    void dfs(int v); // busca em profundidade - gera um arquivo .txt da árvore gerada por ela

    void bfs(int v); // busca em largura - gera um arquivo .txt da árvore gerada por ela

    int dist(int v1, int v2); // retorna a distância entre os vertices v1 e v2

    int diametro(); // retorna a maior distância entre qualquer dois vertices

    list<list<int> > conexa(); // retorna uma lista com todas as componentes conexas do grafo
};

GrafoM::GrafoM(int V)
{
    this->V = V;
    M =  new bool*[V];
    for(int i = 0; i < V; i++){
        M[i] = new bool[V];
        for(int j = 0; j < V; j++){
            M[i][j] = 0;
        }
    }
}

void GrafoM::addAresta(int v1, int v2)
{
    M[v1-1][v2-1] = 1;
    M[v2-1][v1-1] = 1;
}

int GrafoM::grau(int v)
{
    int soma = 0;
    for(int i = 0; i < V; i++)
        soma += M[v-1][i];
    
    return soma;
}

int GrafoM::gmax()
{
    int max = 0;
    int atual;
    int vmax;

    for(int i = 0; i < V; i++){
        atual = grau(i+1);
        if(atual > max){
            max = atual;
            vmax = i+1;
        }
    }
    return max;
}

float GrafoM::media_grau()
{
    float soma = 0;
    for(int i = 0; i < V; i++){
        soma = soma + grau(i+1);
    }
    return (soma/V);
}

float GrafoM::mediana_grau()
{
    float x[V];

    for(int i = 0; i < V; i++){
        x[i] = grau(i+1);
    }

    sort(x, x+V);

    if(V%2 == 0){
        return (x[(V/2)] + x[(V/2) - 1])/2;
    }
    else{
        return x[V/2];
    }
}

void GrafoM::bfs(int v)
{
    queue<int> Q;
    int L[V];
    int Pai[V];
    bool explorados[V];

    for(int i = 0; i < V; i++){
        explorados[i] = false;
    }
    explorados[v-1] = true;
    L[v-1] = 0;
    Pai[v-1] = 0;

    while (true)
    {
        for(int i = 0; i < V; i++){
            if(M[v-1][i] == 1){
               if(!explorados[i]){
                L[i] = L[v-1] + 1;
                Pai[i] = v;
                explorados[i] = true;
                Q.push(i+1);
               } 
            }
        }
        if(!Q.empty()){
            v = Q.front();
            Q.pop();
        }
        else
            break;
    }
    for(int i = 0; i < V; i++){
        cout << "Pai de " << i+1 << ": " << Pai[i] << " | " << "Nível de " << i+1 << ": " << L[i] << "\n"; 
    }
}

void GrafoM::dfs(int v)
{
    stack<int> P;
    int L[V];
    int Pai[V];
    bool explorados[V];

    for(int i = 0; i < V; i++){
        explorados[i] = false;
    }
    L[v-1] = 0;
    Pai[v-1] = 0;

    while(true)
    {
        if(!explorados[v-1]){
            explorados[v-1] = true;
            P.push(v);
        }

        bool descoberto = false;
        int iaux;

        for(int i = 0; i < V; i++){
            if(M[v-1][i] == 1){
                if(!explorados[i]){
                    L[i] = L[v-1] + 1;
                    Pai[i] = v;
                    descoberto = true;
                    iaux = i;
                    break;
                }
            }
        }
        
        if(descoberto)
            v = iaux + 1;
        else{
            P.pop();

            if(P.empty())
                break;
            
            v = P.top();
        }
    }
    for(int i = 0; i < V; i++){
        cout << "Pai de " << i+1 << ": " << Pai[i] << " | " << "Nível de " << i+1 << ": " << L[i] << "\n"; 
    }
}

int GrafoM::dist(int v1, int v2)
{
    queue<int> Q;
    int L[V];
    bool explorados[V], cheguei = false;

    for(int i = 0; i < V; i++){
        explorados[i] = false;
    }
    explorados[v1-1] = true;
    L[v1-1] = 0;


    while (true)
    {
        for(int i = 0; i < V; i++){
            if(M[v1-1][i] == 1){
               if(!explorados[i]){
                L[i] = L[v1-1] + 1;
                if(i+1 == v2){
                    cheguei = true;
                    break;
                }
                explorados[i] = true;
                Q.push(i+1);
               } 
            }
        }

        if(cheguei)
            break;

        if(!Q.empty()){
            v1 = Q.front();
            Q.pop();
        }
        else
            break;
    }
    return L[v2-1];
}

int GrafoM::diametro()
{
    int depth;
    int diam = 0;
    for(int i = 1; i < V+1; i++){
        for(int j = 1; j < V+1; j++){
            if(i < j){
                depth = dist(i, j);
                if(depth > diam){
                    diam = depth;
                }
            }
        }
    }
    return diam;
}

list<list<int> > GrafoM::conexa()
{
    int v = 1;
    int iaux = 0;
    bool achados[V], completo = false;
    list<list<int> > componentes;

    for(int i = 0; i < V; i++)
        achados[i] = false;
    
    while (!completo){
        list<int> comp;
        stack<int> P;
        bool explorados[V];

        for(int i = 0; i < V; i++){
            explorados[i] = false;
            }

        while(true)
        {
            if(!explorados[v-1])
            {
                achados[v-1] = true;
                explorados[v-1] = true;
                comp.push_back(v);
                P.push(v);
            }

            bool descoberto = false;
            int iaux;

            for(int i = 0; i < V; i++){
            if(M[v-1][i] == 1){
                if(!explorados[i]){
                    descoberto = true;
                    iaux = i;
                    break;
                    }
                }
            }
            if(descoberto)
                v = iaux + 1;
            else{
                P.pop();

                if(P.empty()){
                    componentes.push_back(comp);
                    break;
                }

                v = P.top();
            }   
        }

        for(int i = iaux; i < V; i++){
            if(achados[i] == false){
                v = i + 1;
                iaux = i;
                break;   
            }
            else if (i == V - 1){
                completo = true;
            }
        } 
    }

    list<list<int> >::iterator itr, it;
    for(itr = componentes.begin(); itr != componentes.end(); itr++){
        int tam = 0;
        list<int>x = *itr;
        for(it = itr; it != componentes.end(); it++)
        {
            list<int>tl = *it;
            int atual = tl.size();
            if(atual > tam){
                tam = atual;
                *itr = tl;
                *it = x;
            }

        }
        
    }/**/
    return componentes;   
}

int main(){
    int count = 0;
    int num, num1;
    int v;
    int gmax;
    float medg, medianag;
    string escolha;
    list<list<int> > x;
   fstream newfile, rfile;

    cout << "Escolha a representação do grafo (digite 'matriz' para matriz ou 'lista' para lista): ";
    cin >> escolha;

   newfile.open("grafo_3.txt" ,ios::in); //abre o arquivo .txt apenas para leitura
   if (newfile.is_open())
   {    
        newfile >> v;
        if(escolha == "matriz"){
            GrafoM graph(v);
            do{
            newfile >> num;
            newfile >> num1;
            if (!newfile.fail()) {
                count++;
                graph.addAresta(num, num1);
                
            }
            }while(!newfile.fail());
            gmax = graph.gmax();
            medg = graph.media_grau();
            medianag = graph.mediana_grau();
            x = graph.conexa();
        }
        else if (escolha == "lista"){
            Grafo graph(v);
            do{
            newfile >> num;
            newfile >> num1;
            if (!newfile.fail()) {
                count++;
                graph.addAresta(num, num1);
                
            }
            }while(!newfile.fail());
            gmax = graph.gmax();
            medg = graph.media_grau();
            medianag = graph.mediana_grau();
            x = graph.conexa();
        }
        else{
            throw runtime_error("Digite apenas 'lista' ou 'matriz'!!");
        }

        newfile.close();
        //system("pause");  usado para calcular o tamanho dos grafos em memória
        
        rfile.open("resposta.txt", ios::out);// cria um arquivo e escreve no mesmo
        if (rfile.is_open()){
        rfile << "Número de Arestas: " << count << endl;
        rfile << "Número de Vertices: " << v << endl;
        rfile << "Grau Máximo: " << gmax << endl;
        rfile << "Média dos Graus: " << medg << endl;
        rfile << "Mediana dos Graus: " << medianag << endl;
        rfile << "Quantidade de Componentes Conexas: " << x.size() << endl;
        list<list<int> >::iterator itr;
        for(itr = x.begin(); itr != x.end(); itr++){
            list<int>tl = *itr;
            list<int>::iterator it;
            rfile << "{ ";
            for(it = tl.begin(); it != tl.end(); it++){
                rfile << *it << " ";
            }
            rfile << "}" << " | Tamanho da componente: " << tl.size();
            rfile << endl << "--//--" << endl;
        }
    }rfile.close();
   } 
}