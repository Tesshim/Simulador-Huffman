#include <algorithm>
#include <string>
#include <cassert>
#include <stdexcept>
#include <iostream>
#include<stdio.h>
#include <fstream>

using namespace std;

struct huffman
{
	int frequencia;
	string cod;
	char letra;
	huffman *prox;
	huffman *esq;
	huffman *dir;
};

struct ASCII
{
	string cod;
	char letra;
};

void mapa(huffman vetor[], string texto);

void mapaarv(huffman vetor[], string texto, huffman* &tree);

void mostra(huffman vetor[]);

void mostraarv(huffman *tree);

void mostraarvaux(huffman *tree);

void ordena(huffman vetor[]);

void ordenaarv(huffman* &tree, string texto);

void insere(huffman* &tree, huffman* aux);

void junta(huffman* &tree, string texto, huffman* &aux);

void codfica(huffman* &tree, string cod);

void cod(huffman *tree, string texto);

void busca(char letra, huffman *tree, string &cod);

void decod (string cod, huffman *tree);

void le_arquivo(string nome, huffman* &tree);

void escreve_arquivo(string nome, string texto);

void codASC(ASCII vet[]);

int get_size(const char* file_name);

int main()
{
	int op = 10000, ver = 0;
	ASCII vetor[95];
	huffman *tree = NULL;
	string aux;
	ifstream file("ASCII.txt",ios::in);
	ofstream outfile ("saida.huf");
	outfile.clear();
	ofstream limp ("saidaascii.txt");
	limp.clear();
	
	//parte para ler a tabela ASCII e os seus bits respectivos
	if(!file.is_open())
	{
		cout<<"O arquivo ASCII nao abre"<<endl;
	}
	else
	{
		for(int i = 0; i<95; ++i)
		{
			file>>vetor[i].cod;
			file>>vetor[i].letra;
			if(i == 0)
				vetor[i].letra = ' ';
		}
		file.close();	
	}
	
	codASC(vetor);
	
	while (op != 0)
	{
		cout << "\t" << "MENU" <<endl;
		cout <<"1 - Compactar: "<<endl;
		cout <<"2 - Descompactar: "<<endl;
		cout <<"0 - Sair: "<<endl;
		cin >>op;
		
		switch(op)
		{
			case 1:
			{
				system ("cls");
				aux = "entrada";
				le_arquivo(aux, tree);
				ver = 1;
				system ("pause");
				system ("cls");
				
				break;
			}
			case 2:
				system ("cls");
				if (ver == 0)
				{
					cout<< "Nenhuma arvore foi montada ainda, algum arquivo precisa ser lido antes"<<endl;
				}
				else if (ver == 1)
				{
					ofstream lip ("entrada.txt");
					lip.clear();
					aux = "saida";
					le_arquivo(aux, tree);
				}
				system ("pause");
				system ("cls");
				break;
			case 0:
				{
					return 0;
				}
			default:
				system ("cls");
				cout << "Opcao invalida!"<<endl;
				system ("pause");
				system ("cls");
				break;
		}
	}

	return 0;
}

void mapa(huffman vetor[], string texto)
{
	char aux;
	for (int i = 0; i<1000; ++i)
	{
		if(i < texto.size())
		{
			aux = texto[i];		
			for (int j = 0; j<1000; ++j)
			{
					if(vetor[j].letra == aux)
					{
						vetor[j].frequencia++;
						break;
					}
					else if(vetor[j].letra != aux && vetor[j].frequencia == 1000)
					{
						vetor[j].frequencia = 1;
						vetor[j].letra = aux;
						vetor[j].dir = NULL;
						vetor[j].esq = NULL;
						break;
					}
			}
		}
	}
	
}

void mostra(huffman vetor[])
{
	int i = 0;
	do
	{
		cout<< "Letra: "<<vetor[i].letra<<" Frequencia: "<<vetor[i].frequencia<<endl;
		++i;
		
	}while (vetor[i].frequencia != 1000);
	
}

void ordena(huffman vetor[])
{
	int val, ind;
	huffman aux;
	
	for(int i = 0; vetor[i].frequencia != 1000; ++i)
	{
		val = vetor[i].frequencia;
		ind = i;
		for(int j = i+1; vetor[j].frequencia != 1000; ++j)
		{
			if (val > vetor[j].frequencia)
			{
				val = vetor[j].frequencia;
				ind = j;
			}
			
		}
		
		if (ind != i)
		{
			aux.frequencia = vetor[ind].frequencia;
			aux.letra = vetor[ind].letra;
			aux.dir = vetor[ind].dir;
			aux.esq = vetor[ind].esq;
			aux.prox = vetor[ind].prox;
			vetor[ind].frequencia = vetor[i].frequencia;
			vetor[ind].letra = vetor[i].letra;
			vetor[ind].dir = vetor[i].dir;
			vetor[ind].esq = vetor[i].esq;
			vetor[ind].prox = vetor[i].prox;
			vetor[i].frequencia = aux.frequencia;
			vetor[i].letra = aux.letra;
			vetor[i].dir = aux.dir;
			vetor[i].esq = aux.esq;
			vetor[i].prox = aux.prox;
		}
	}
}

void mapaarv(huffman vetor[], string texto, huffman* &tree)
{
	huffman* aux;
	aux = new huffman;

	for (int i = 0; i < texto.size(); ++i)
	{
		if(vetor[i].letra != '2')
		{
			aux->frequencia = vetor[i].frequencia;
			aux->letra = vetor[i].letra;
			aux->dir = vetor[i].dir;
			aux->esq = vetor[i].esq;
			aux->prox = vetor[i].prox;
			insere(tree, aux);
		}
	}
}

void mostraarv(huffman *tree)
{
	while(tree != NULL)
	{
		cout<<"Frequencia: " <<tree->frequencia;
		cout<<" Letra: " << tree->letra <<endl;
		tree = tree->prox;	
	}	
}

void insere(huffman* &tree, huffman* aux)
{
	if(tree == NULL)
	{
		tree = new huffman;
		tree->frequencia = aux->frequencia;
		tree->letra = aux->letra;
		tree->dir = aux->dir;
		tree->esq = aux->esq;
		tree->prox = NULL;
		return;
	}
	
	insere(tree->prox, aux);
	
	return;
}

void ordenaarv(huffman* &tree, string texto)
{
	huffman *aux;
	aux = tree;
	huffman vet[10000];
	int i = 0;
	
	for(int i = 0; i<10000; ++i)
	{
		vet[i].frequencia = 1000;
		vet[i].letra = '2';
	}
	
	while (aux != NULL)
	{
		vet[i].frequencia = aux->frequencia;
		vet[i].letra = aux->letra;
		vet[i].dir = aux->dir;
		vet[i].esq = aux->esq;
		vet[i].prox = aux->prox;
		aux = aux->prox;
		i++;
	}
	
	delete tree;
	tree = NULL;
	
	ordena(vet);

	mapaarv(vet,texto,tree);

}

void junta(huffman* &tree, string texto, huffman* &aux)
{
	while (tree->prox != NULL)
	{		
		ordenaarv(tree, texto);	
		aux = new huffman;
		aux->dir = tree;
		aux->esq = tree->prox;
		//cout<< "aux: "<<aux->dir->letra<<" "<<aux->esq->letra<<endl;
		aux->prox = tree->prox->prox;
		aux->frequencia = tree->frequencia + tree->prox->frequencia;
		aux->letra = '¿';
		tree = aux;
		//mostraarvaux(tree);
	}
	//cout<< "TREE: "<<tree->dir->letra<<" "<<tree->esq->letra<<endl;
	return;
}

void codfica(huffman* &tree, string cod)
{
	if (tree == NULL)
		return;

	codfica(tree->esq, cod+'0');
	
	if(tree->letra != '¿')
	{
		tree->cod = cod;
	}
	
	codfica(tree->dir, cod+'1');

	return;
}

void mostraarvaux(huffman *tree)
{
	if (tree == NULL)
		return;
		
	mostraarvaux(tree->esq);
	
	if( tree->letra != '¿' )
	{
		cout <<"Letra: "<<tree->letra<<" Cod: "<<tree->cod<<endl;
	}

	mostraarvaux(tree->dir);
	
	return;	
}

void cod(huffman *tree, string texto)
{
	
	string num;
	string arq = "saida";
	char aux;
	
	for (int i = 0; i < texto.size(); ++i)
	{
		aux = texto[i];
		busca(aux, tree, num);
	}
	
	cout << "Codigo: "<<num<<endl;
	
	escreve_arquivo(arq, num);
	//decod(num, tree);
	
}

void busca(char letra, huffman *tree, string &cod)
{
	if(tree == NULL)
		return;
	
	busca(letra,tree->esq, cod);
	
	if( tree->letra == letra )
	{
		cod = cod + tree->cod;
		return;
	}

	busca(letra,tree->dir, cod);
	
	return;
}

void decod (string cod, huffman *tree)
{
	string palavra;
	string aux = palavra;
	string arq = "entrada";
	huffman *anda;
	anda = tree;
	
	cout<<"DECODIFICACAO!!"<<endl;
	
	for (int i = 0; i < cod.size(); ++i)
	{
		if(cod[i] == '1')
			anda = tree->dir;
		else
			anda = tree->esq;
			
			while (anda->dir != NULL && anda->esq != NULL)
			{
				if (cod[i+1] == '1')
				{
					anda = anda->dir;
					i++;
				}
				else if (cod[i+1] == '0')
				{
					anda = anda->esq;
					i++;
				}
				
			}
			
			palavra += anda->letra;
			//cout <<endl<<palavra<<endl;
			cout <<anda->letra;
	}
	if( palavra.length() != 0)
		escreve_arquivo(arq, palavra);
	
	return;	
}

void le_arquivo(string nome, huffman* &tree)
{
	ifstream file;
	string linha;
	huffman *ttt = NULL;
	string aux;
	string comp;
	huffman vet[10000];
	for(int k = 0; k<10000; ++k)
	{
		vet[k].frequencia = 1000;
		vet[k].letra = '2';	
	}
	
	if(nome == "entrada")
		file.open("entrada.txt",ios::in);
	else
		file.open("saida.huf",ios::in);
	
	
	if(!file.is_open())
	{
		cout<<"Nao abre"<<endl;
		return;
	}
	
	if (nome == "entrada")
	{
		//Nessa parte ele lê tudo que esta no arquivo e faz o mapeamento de caracteres
		for(int i = 0 ; !file.fail(); i++)
		{
			getline(file, linha);
			
			if (i == 0)
			{
				comp = linha;
				mapa(vet, linha);    
				cout << endl;
			}
			if (comp != linha)
			{
				comp = linha;
				mapa(vet, linha);    
				cout << endl;
			}
			
		}
		
		//Depois de ler todos os caracteres é passado para uma estrutura de arvore onde sera montada a arvore de huffman
		mapaarv(vet, linha, tree);
		ordenaarv(tree, linha);
		mostraarv(tree);
		junta(tree, linha, ttt);
		codfica(tree, aux);	
		mostraarvaux(tree);
		
		file.close();
		
		//aqui ele volta a ler o arquivo do inicio para codficar as frases contidas no arquivo
		if(nome == "entrada")
			file.open("entrada.txt",ios::in);
		else
			file.open("saida.huf",ios::in);
			
		for(int i = 0 ; !file.fail(); i++)
		{
			getline(file, linha);
			
			if (i == 0)
			{
				comp = linha;  
				cod(tree, linha); 
				cout << endl;
			}
			if (comp != linha)
			{
				comp = linha; 
				cod(tree, linha);  
				cout << endl;
			}
			
		}
		
		
		file.close();
	}
	
	if (nome == "saida")
	{
		for(int i = 0 ; !file.fail(); i++)
		{
			getline(file, linha);
			
			if (i == 0)
			{
				comp = linha;  
				decod( linha, tree ); 
				cout << endl;
			}
			if (comp != linha)
			{
				comp = linha; 
				decod(linha, tree);  
				cout << endl;
			}
			
		}
		file.close();
	}
	//system ("cls");
}

void escreve_arquivo(string nome, string texto)
{
	ofstream outfile;
	
	if(nome == "entrada")
	{
		outfile.open("entrada.txt",ios::app);
		if(get_size("entrada.txt") == 0)
		{
	    	//printf("O arquivo esta vazio.");
	    	outfile << texto;
		}
		else
		{
	    	//printf("O arquivo nao esta vazio.");
	    	outfile << endl << texto;
		}
	}
	else
	{
		outfile.open("saida.huf",ios::app);
		
		outfile << texto <<endl;
	}
		
	outfile.close();
		
}

void codASC(ASCII vet[])
{
	string frase;
	string cod;
	string comp;
	ifstream file;
	file.open("entrada.txt",ios::in);
	ofstream outfile;
	outfile.open("saidaascii.txt",ios::app);
	
	for(int i = 0 ; !file.fail(); i++)
	{
		getline(file, frase);
		cod.clear();
		
		if(i == 0)
		{
			comp = frase;
			for(int k = 0; k<frase.size(); ++k)
			{
				for(int j = 0; j<95; ++j)
				{
					if(frase[k] == vet[j].letra)
						cod += vet[j].cod;
				}
			}	
		}
		if(comp != frase)
		{
			comp = frase;
			for(int k = 0; k<frase.size(); ++k)
			{
				for(int j = 0; j<95; ++j)
				{
					if(frase[k] == vet[j].letra)
						cod += vet[j].cod;
				}
			}
		}
		
		outfile << cod <<endl;
	}
	
	outfile.close();
	file.close();
}

int get_size(const char* file_name)
{
    FILE *file = fopen(file_name, "r");

    if(file == NULL)
        return 0;

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fclose(file);

    return size;
}
