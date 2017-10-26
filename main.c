#include <assert.h>
#include <stdio.h>
#include <graphviz/cgraph.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int indice = 0;

void CalculaPosOrdem(Agraph_t *grafo, Agnode_t *v, char **pos_ordem) {
	agset(v, "visitado", "1");
	for (Agedge_t *edge = agfstout(grafo, v); edge; edge = agnxtout(grafo, edge)) {
		if (*(agget(edge->node, "visitado")) == '0') {
			CalculaPosOrdem(grafo, edge->node, pos_ordem);
		}
	}
	pos_ordem[indice] = malloc(sizeof(agnameof(v)));
	strcpy(pos_ordem[indice], agnameof(v));
	indice++;
}

void BuscaComponente(Agraph_t *grafo, Agnode_t *v) {
	agset(v, "visitado", "1");
	for (Agedge_t *edge = agfstout(grafo, v); edge; edge = agnxtout(grafo, edge)) {
		if (*(agget(edge->node, "visitado")) == '0') {
			BuscaComponente(grafo, edge->node);
		}
	}
	printf("  %s;\n",agnameof(v));
}

Agraph_t* ComputaTranposto(Agraph_t *grafo) {
	Agraph_t *tranposto;
	Agnode_t *v1;
	Agnode_t *u1;
	tranposto = agopen("Tranposto", Agdirected, NULL);
	for (Agnode_t *v = agfstnode(grafo); v; v=agnxtnode(grafo,v)){ 
		v1 = agnode(tranposto, agnameof(v), TRUE);
		for (Agedge_t *edge = agfstout(grafo, v); edge; edge = agnxtout(grafo, edge)) {
			u1 = agnode(tranposto, agnameof(edge->node), TRUE);
			agedge(tranposto, u1, v1, "", TRUE);
		}
	}
	return tranposto;
}

void AchaComponentes(Agraph_t *grafo, char **lista, int n){
	int cluster = 1;
	Agnode_t *v;
	for (int i = n-1; i >= 0; i--){	
		v = agnode(grafo,lista[i],FALSE);  
		if (*(agget(v, "visitado")) == '0'){
			printf(" subgraph cluster%d {\n",cluster);
			BuscaComponente(grafo,v);
			cluster++;
			printf(" }\n");
		}
	}
}

void Imprime_grafo(Agraph_t *grafo, Agnode_t *v){
	agset(v, "visitado", "0");
	for (Agedge_t *edge = agfstout(grafo, v); edge; edge = agnxtout(grafo, edge)) {
			if (*(agget(edge->node, "visitado")) == '1'){
				Imprime_grafo(grafo, edge->node);
			}
			printf(" %s -> %s;\n", agnameof(v), agnameof(edge->node));
	}
}		

void main() {
	char **pos_ordem;
	Agraph_t *grafo = agread(stdin, NULL);
	int n = agnnodes(grafo);
	pos_ordem = malloc(sizeof(char*) * (n-1));
	agattr(grafo, AGNODE, "visitado", "0");

	CalculaPosOrdem(grafo, agfstnode(grafo), pos_ordem);
	Agraph_t *grafo_transposto = ComputaTranposto(grafo);
	agattr(grafo_transposto, AGNODE, "visitado", "0");
	printf("strict digraph resultado{\n");
	AchaComponentes(grafo_transposto, pos_ordem, n);
	printf("\n");
	Imprime_grafo(grafo, agfstnode(grafo));
	printf("}");
}

