#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
using namespace std;

vector<vector<int>> Graph;

// tamanho = V, index = vertices do grafo
// o valor nesse index e igual ao identificar 
// do SCC ao qual v pertence
vector<int> SCCid;

int V;
int E;
int numSCCs = 0;
int numLig = 0;


// DFS recursivo principal
void Tarjan_visit(int u, vector<int> &pre, vector<int> &low, vector<int> &stack, vector<bool> &onStack) 
{
	// Persiste em todas as interadoes recursivas
	static int counter = 0;
	pre[u] = low[u] = counter;
	counter++;
	stack.push_back(u);
	onStack[u] = true;

	for (std::size_t i=0; i< Graph[u].size(); ++i)
	{
		// precisamos dos vertices v conectados a u em index 0
		// para trabalhar com as arrays de auxilio
		int v = Graph[u][i];
		v--;
		// Se ainda nao visitamos este v, visita-lo
		if ( pre[v] == -1)
		{
			Tarjan_visit(v, pre, low, stack, onStack);

			// Este v ja foi visitado, analisemos o seu valor low
			// o menor dos 2 sera o que queremos guardar no low do u
			low[u] = min(low[v], low[u]);
		}
		else if (onStack[v])
		{
			low[u] = min(low[u], pre[v]);
		}
	}

	// encontramos um vertice cabeca de um SCC
	// a stack que fomos criando contem este SCC
	if (pre[u] == low[u])
	{
		vector<int> temp;
		int min = V;
		int r;
		while (true)
		{
			r = stack.back();
			// para encontrar o indentificador de um SCC
			if(r<min){min=r;}
			temp.push_back(r);
			onStack[r] = false;
			if (u == r) {stack.pop_back(); break;}
			stack.pop_back();
		}

		// Para cada vertice do SCC, guardar em SCCid que o seu 
		// respetivo identificador = min
		for(int i:temp){SCCid[i]=min;}
		numSCCs++;
	}
}

// Algoritmo de tarjan que e utilizado para encontrar os SCCs do grafo
// Para cada vertice do grafo faz-se uma DFS (Tarjan_visit)
void SCC_Tarjan()
{
	vector<int> stack;
	vector<int> pre(V);
	vector<int> low(V);
	vector<bool> onStack(V);

	fill(pre.begin(), pre.end(), -1);
	fill(low.begin(), low.end(), -1);
	fill(onStack.begin(), onStack.end(), false);

	// este loop certifica-se que mesmo que um vertice v' nao seja
	// atingivel de um v inicial, sera visitado
	for (int i = 0; i < V; ++i)
	{
		if (pre[i] == -1)
			Tarjan_visit(i, pre, low, stack, onStack);
	}
}


int main() 
{
	string line;

	getline(cin, line);
	V = stol(line);
	getline(cin, line);
	E = stol(line);

	Graph.resize(V);
	SCCid.resize(V);

	while (getline(cin, line))
	{
		string::size_type s;
		
		int u = stol(line, &s);
		int v = stol(line.substr(s));
		Graph[u-1].push_back(v);
	}

	SCC_Tarjan();

	// Grafo de Componentes(SCCs) que vamos construir
	vector<vector<int>> CG;
	CG.resize(V);

	// Iterar pelos edges do grafo G
	// Construir grafo de SCCs (CG)
	for (std::size_t i = 0; i < Graph.size(); ++i)
	{
		int u = i;
		for (std::size_t j=0; j < Graph[i].size(); ++j)
		{
			int v = Graph[i][j];
			int a = SCCid[u];
			int b = SCCid[v - 1];
			if(a!=b){
				CG[a].push_back(b + 1);
			}
		}
	}

	// ordenar os edges do grafo de SCCs
	// remover edges duplos
	for(std::size_t i=0; i < CG.size();++i)
	{
		vector<int> &l = CG[i];
		sort(l.begin(), l.end());
		l.erase( unique(l.begin(),l.end()), l.end() );
		numLig += l.size();
	}


	// Imprimir os resultados no cout
	cout<<numSCCs<<"\n"<<numLig<<"\n";
	for(std::size_t a=0; a<CG.size();++a)
	{
		for( int b:CG[a])
		{
			cout<<a+1<<" "<<b<<"\n";
		}
	}

	return 0;
}