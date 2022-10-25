#include <iostream>
#include <string>
using namespace std;

// 1 -  Faça uma função que recebe uma certa medida e ajusta ela percentualmente 
// entre dois valores mínimo e máximo e retorna esse valor

//função para calcular o percentual de um numero entre valores maximos e minimos
float converteSensor(float valor, float minimo, float maximo) {
	// realiza o calculo do percentual
	int percentual = int((float(valor - minimo) / float(maximo - minimo))*100);
	// retorna o valor do percentual;
	return percentual; 
}

// 2 - Faça uma função que simule a leitura de um sensor lendo o 
// valor do teclado ao final a função retorna este valor

int leituraSensor(void){
	//define variavel para recebvber o valor do sensor
    int ValorSensor = 0;
	//recebe o valor do sensor
    cout << "Digite o valor do sensor: ";
    cin >> ValorSensor;
	//retorna o valor do sensor
    return ValorSensor;
}


// 3 - Faça uma função que armazena uma medida inteira qualquer 
// em um vetor fornecido. Note que como C não possui vetores 
// nativos da linguagem, lembre-se que você precisa passar o 
// valor máximo do vetor assim como a última posição preenchida
// Evite também que, por acidente, um valor seja escrito em 
// uma área de memória fora do vetor

int ArmazenarVetor(int medida, int posicao_atual, int *v, int maior_posicao){
	//declara ponteiro para o vetor
    int *ponteiro = v;
	//adiciona a medida no vetor
    ponteiro[posicao_atual] = medida;
	//retorna a proxima posicao do vetor
    return posicao_atual+1;
}

// 4 - Faça uma função que recebe um vetor com 4 posições que contém 
// o valor da distância de um pequeno robô até cada um dos seus 4 lados.
// A função deve retornar duas informações: A primeira é a direção 
// de maior distância ("Direita", "Esquerda", "Frente", "Tras") e a 
// segunda é esta maior distância.

const char* direcaoMenorCaminho(int *vetor, int *maximo){
	//declara matriz para receber os textos das direções
	const char* direcoes[] = {"Direita", "Esquerda", "Frente", "Tras"};
	int index = -1;
	//compara valores para ver qual e a maior distancia
	for(int i = 0; i < 4; i++){
		if(vetor[i] > *maximo){
			//gaurda a mais distancia na variavel maximo
			*maximo = vetor[i];
			index = i;
		}
	}	//retorna a direção da maior distancia
		return direcoes[index];
	}



// 5 - Faça uma função que pergunta ao usuário se ele deseja continuar o mapeamento e 
// retorna verdadeiro ou falso
bool leComando(){
//pergunta ao usuario se deseja continuar 1 ou 2 e retorna true ou false
	int comando = 0;
	cout << "Deseja continuar? 1 - Sim 2 - Nao: ";
	cin >> comando;
	if(comando == 1){
		return true;
	}else{
		return false;
	}
}



// 6 - A função abaixo (que está incompleta) vai "dirigindo" virtualmente um robô 
// e através de 4 sensores em cada um dos 4 pontos do robo ("Direita", "Esquerda", 
// "Frente", "Tras"). 
//      A cada passo, ele verifica as distâncias aos objetos e vai mapeando o terreno 
// para uma movimentação futura. 
//      Ele vai armazenando estas distancias em um vetor fornecido como parâmetro 
// e retorna a ultima posicao preenchida do vetor.
//      Esta função deve ir lendo os 4 sensores até que um comando de pare seja enviado 
//
//      Para simular os sensores e os comandos de pare, use as funções já construídas 
// nos ítens anteriores e em um looping contínuo até que um pedido de parada seja 
// enviado pelo usuário. 
//
//      Complete a função com a chamada das funções já criadas
int dirige(int *v,int maxv){
	int maxVetor = maxv;
	int *vetorMov = v;
	int posAtualVetor = 0;
	int dirigindo = 1;		
	while(dirigindo){
		for (int i = 0; i < 4; i++)
		{
		int medida = leituraSensor();
		medida = converteSensor(medida,0,830);
		cout << medida;
		posAtualVetor = ArmazenarVetor(medida, posAtualVetor, vetorMov, maxVetor);
		cout<< vetorMov[posAtualVetor-1];
		}
		dirigindo = leComando();		
	}
	return posAtualVetor;
}

// O trecho abaixo irá utilizar as funções acima para ler os sensores e o movimento
// do robô e no final percorrer o vetor e mostrar o movimento a cada direção baseado 
// na maior distância a cada movimento
void percorre(int *v,int tamPercorrido){		
	int *vetorMov = v;
	int maiorDir = 0;
	for(int i = 0; i< 4; i+=4){
		const char *direcao = direcaoMenorCaminho(&(vetorMov[i]),&maiorDir);
		printf("Movimentando para %s distancia = %i\n",direcao,maiorDir);
	}
}

int main(int argc, char **argv) {
	int maxVetor = 100;
	int vetorMov[maxVetor*4];
	int posAtualVet = 0;
	posAtualVet = dirige(vetorMov,maxVetor);
	percorre(vetorMov,posAtualVet);
	
	return 0;
}