#include <fstream>
#include <vector>
#include <iostream>
#include <queue>
#include <set>
using namespace std;

#define INF 1000000000

struct edge{
  int dest;
  int weight;
};

struct nodo{
  vector<edge> vic;               //lista dei vicini
  int distance = INF;                          //metto inizialmente a infinto la distanza, come richiesta dall'algoritmo di Dijkstra
  bool visitato = false;                   //serve per vedere se è stato già visitato quel nodo
  int citta;
  int numeroPercorsi = 1;
  }; 

struct corrispondenza{                     //si intende per quante volte una certa città verrebbe percorsa dai gufi; serve per trovare la città che Colui Che Non Deve Essere Nominato potrebbe attaccare per massimizzare il numero di studenti che non riceveranno in tempo l'invito
	set<int> nodi;                         //lista delle città che si possono raggiungere dalla città attaccata
};

vector<nodo> grafo;
vector<corrispondenza> corrispondenze;     //serve per tenere conto quante volte una certa città verrebbe percorsa


void risolvi(int powarts){                  //questa funzione si basa sull'algoritmo di Dijkstra
   
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int,int>>> contenitore;
	contenitore.push(make_pair(0,powarts));
	grafo[powarts].distance = 0;                //è il nodo di partenza
	grafo[powarts].citta = powarts;
	
	while(!contenitore.empty()){
    auto p=contenitore.top();
    contenitore.pop();
    int n = p.second;
    if(!grafo[n].visitato){
      grafo[n].visitato=true;
	  
      for(edge& e:grafo[n].vic){

        if(grafo[n].distance + e.weight < grafo[e.dest].distance){
            grafo[e.dest].distance = grafo[n].distance + e.weight;
            contenitore.push(make_pair(grafo[e.dest].distance, e.dest));
		  
		    grafo[e.dest].numeroPercorsi = grafo[n].numeroPercorsi;
		
			if (grafo[n].citta == powarts){
				grafo[e.dest].citta = e.dest;
			}else{
				corrispondenze[grafo[e.dest].citta].nodi.erase(e.dest);
				grafo[e.dest].citta = grafo[n].citta;
			}
				    
				int cittaDaAttaccare = grafo[e.dest].citta;    //prendiamo la variabile città del nodo
				corrispondenze[cittaDaAttaccare].nodi.insert(e.dest);  //metto nella lista la città che ha quel percorso
		    }else if(grafo[n].distance + e.weight == grafo[e.dest].distance){
				
				grafo[e.dest].numeroPercorsi += grafo[n].numeroPercorsi;
						
			    if (grafo[e.dest].citta == grafo[n].citta){
				    grafo[e.dest].citta = grafo[n].citta;
			    }else{
					corrispondenze[grafo[e.dest].citta].nodi.erase(e.dest);
				    grafo[e.dest].citta = e.dest;

				    contenitore.push(make_pair(grafo[e.dest].distance, e.dest));
				    grafo[e.dest].visitato = false;
					        
			    }
						

					int cittaDaAttaccare = grafo[e.dest].citta;    //prendiamo la variabile città del nodo
				    corrispondenze[cittaDaAttaccare].nodi.insert(e.dest);  //metto nella lista la città che ha quel percorso	
				
			}
		
      }
    }
	
  }
	
}


int main(){
	int K = 0;                             //si inizializza a 0 nel caso ipotetico in cui Powarts è l'unica città presente nel grafo (che negli input si presume non sarà mai cosi)
	int cittaAttaccata;                        //variabile che indica la città attaccata
    int frequenzaMassima = 0;                  //serve per indicare con quale frequenza la città attaccata verrebbe attaccata, serve per distinguere dalle altre frequenze
    int N,M,P;
	                            
	ifstream in("input.txt");
    in >> N >> M >> P;                     //prendo in input la prima riga
	grafo.resize(N);
	corrispondenze.resize(N);
	
	for(int j=0; j<M; j++) {               
        int from, to, peso;
        edge vicino1, vicino2;	   // in queste variabile memorizzo una coppia che indica nodo vicino e il peso dell'arco congiungente ad esso
        in >> from >> to >> peso;          //prendo in input le altre righe
        vicino1.dest = to;
		vicino1.weight = peso;
		grafo[from].vic.push_back(vicino1);
		vicino2.dest = from;               //ripeto le 3 righe precedenti siccome il grafo è non orientato
		vicino2.weight = peso;
		grafo[to].vic.push_back(vicino2);
    }

	//operazioni
    risolvi(P);
	
	//per debug
	/*
	for(int i = 0; i < N; i++){
		    cout << i << ": ";
		    cout << grafo[i].citta << " " << grafo[i].frequenza << " " << grafo[i].numeroPercorsi;
		    cout << endl;
	        }*/
			
	//i cout sono per debug (visualizza il numero di città e le città bloccate se scegliamo una certa città che blocherebbe più di una città)
	for(int i = 0; i < N; i++){
		//cout << i << ": " << corrispondenze[i].nodi.size() << " citta' bloccate: ";
		for(int j: corrispondenze[i].nodi){
	    //cout << j << ", ";
	} 
	    //cout << endl;
		if (frequenzaMassima < corrispondenze[i].nodi.size()){
			frequenzaMassima = corrispondenze[i].nodi.size();
			cittaAttaccata = i;
		}
	}
	
	K = corrispondenze[cittaAttaccata].nodi.size();
	ofstream out("output.txt");
	out << K << endl;                      // stampo il numero di studenti che ho trovato
	
	for(int i: corrispondenze[cittaAttaccata].nodi){
		out << i << endl;
	}
	
	return 0;
}