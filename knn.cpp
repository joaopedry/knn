#include <iostream>
#include <vector>
#include <math.h>
#include <set>
#include <map>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>// necessário p/ as funções rand() e srand()

#include<time.h>//necessário p/ função time()

using namespace std;

class Individuo
{
private:
	string classe;
	double a, b, c, d;

public:
	Individuo(double a, double b, double c, double d, string classe)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->classe = classe;
	}

	string getClasse()
	{
		return classe;
	}

	double getA()
	{
		return a;
	}

	double getB()
	{
		return b;
	}

	double getC()
	{
		return c;
	}

	double getD()
	{
		return d;
	}
};

// função que retorna a distância euclidiana entre 2 indivíduos
double obterDistEuclidiana(Individuo ind1, Individuo ind2)
{
	/*
		a distância euclidiana é a raiz quadrada da soma das
		diferenças dos valores dos atributos elevado ao quadrado
	*/
	
	double soma = pow((ind1.getA() - ind2.getA()), 2) +
				  pow((ind1.getB() - ind2.getB()), 2) +
				  pow((ind1.getC() - ind2.getC()), 2) +
				  pow((ind1.getD() - ind2.getD()), 2);
	
	return sqrt(soma);
}

// essa função classifica uma nova amostra
string classificarAmostra(vector<Individuo>& individuos,
						  Individuo novo_exemplo, int K)
{


	// obtém o tamanho do vetor
	int tam_vet = individuos.size();

	/*
		set de pairs da distância de cada indivíduo
		do conjunto de treinamento para o novo exemplo
		cada pair é composto pela distância e o índice
		do indivíduo no vetor
	*/
	set<pair<double, int> > dist_individuos;

	/*
		calcula-se a distância euclidiana do novo exemplo
		para cada amostra do conjunto de treinamento
	*/
	for(int i = 0; i < tam_vet; i++)
	{
		double dist = obterDistEuclidiana(individuos[i], novo_exemplo);
		dist_individuos.insert(make_pair(dist, i));
	}
	/*
	para decidir a qual classe pertence o novo exemplo,
	basta verificar a classe mais frequente dos K
	vizinhos mais próximos
	*/
	set<pair<double, int> >::iterator it;
	
	/*
		o contador de Iris-setosa estará no índice 0,
		o contador de Iris-versicolor estará no índice 1
		e o contador de Iris-virginica estará no índice 2
	*/
	vector<int> cont_classes(3);
	
	int contK = 0;

	for(it = dist_individuos.begin(); it != dist_individuos.end(); it++)
	{
		if(contK == K) break;
		
		string classe = individuos[it->second].getClasse();

		if(classe == "Iris-setosa")
			cont_classes[0]++;
		else if(classe == "Iris-versicolor")
			cont_classes[1]++;
		else
			cont_classes[2]++;

		contK++;
	}
	
	string classe_classificacao;
	
	if(cont_classes[0] >= cont_classes[1] && cont_classes[0] >= cont_classes[2])
		classe_classificacao = "Iris-setosa";
	else if(cont_classes[1] >= cont_classes[0] && cont_classes[1] >= cont_classes[2])
		classe_classificacao = "Iris-versicolor";
	else
		classe_classificacao = "Iris-virginica";

	return classe_classificacao;
}

int main(int argc, char *argv[])
{
	
	time_t timer;
  	struct tm *horarioLocal;

  	time(&timer); // Obtem informações de data e hora
  	horarioLocal = localtime(&timer); 
	
	int dia = horarioLocal->tm_mday;
 	int mes = horarioLocal->tm_mon + 1;
 	int ano = horarioLocal->tm_year + 1900;

 	int hora = horarioLocal->tm_hour;
  	int min  = horarioLocal->tm_min;
 	int sec  = horarioLocal->tm_sec;
 	
	int K = 1;
	int MK = 0;
	int MAcertos = 0;
	int conta =0;
	vector<Individuo> individuos;
	float porc = 0;
	int erros = 0;
	
	FILE *ponteiro; //ponteiro do log
		
	// tamanho do conjunto de dados de treinamento
	
	int tam_treinamento = 50;
	/*
		o processo de treinamento consiste em apenas
		armazenar o conjunto de dados de treinamento
	*/
	do{
		srand(time(NULL));
		do
		{
			string classe;
			double a, b, c, d;
			int t = rand() % 50;
			if(t % 2 == 0)
			{
				cin >> a >> b >> c >> d >> classe;
				individuos.push_back(Individuo(a, b, c, d, classe));	
				conta = conta + 1;
			}
			else
			{
				individuos.push_back(Individuo(a, b, c, d, classe));	
			}
		}while(conta < 50);
		
	int acertos = 0;
	int tam_testes = 100 - tam_treinamento;
		// processo de classificação
		for(int i = 0; i < tam_testes; i++)
		{
			string classe;
			double a, b, c, d;
	
			cin >> a >> b >> c >> d >> classe;
			
			Individuo ind(a, b, c, d, classe);
			
			string classe_obtida = classificarAmostra(individuos, ind, K);
			
			//cout << "Classe esperada: " << classe << "\n";
			//cout << "Classe obtida: " << classe_obtida << "\n\n";
			
			if(classe == classe_obtida)
			{
				acertos++; 
			}
			
			if(acertos > MAcertos)
			{
				MAcertos = acertos;
				MK = K;
			}
		}
		erros = tam_testes - acertos;
		
		porc = (100 * acertos) / tam_testes;
		
		cout << acertos << " acertos de um total de " << tam_testes << " tentativas." << porc << "%. \n";
		cout << "Valor de K: " << K << "\n";
		
	//Gravar log de execução
	
	ponteiro = fopen ("log.txt", "a");
	fprintf(ponteiro, "Teste Final\n");
	fprintf(ponteiro, "Total de Acertos: %d\n", acertos);
	fprintf(ponteiro, "Total de Tentativas: %d\n", tam_testes );
	fprintf(ponteiro, "Total de Erros: %d\n", erros );
	fprintf(ponteiro, "Valor de K: %d\n\n", K );
	fprintf(ponteiro, "---------------------------------------------------------------------------------\n\n\n\n\n");
	
	K = K + 1;
	conta = 0;
	
	}while(K <= 10);
		cout << "\n\nMelhor valor de K: " << MK << "\n";
		
	fprintf(ponteiro, "Melhor valor de K: %d\n\n", MK);
	return 0;
}
