#include <stdio.h>
#include <stack>
#include <iostream>

using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
stack<pos_t> valid_positions;

/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	cout << "b";
	pos_t initial_pos;
			// Abre o arquivo para leitura (fopen)
	FILE *mz = fopen(file_name, "r");
			// Le o numero de linhas e colunas (fscanf) 
			// e salva em num_rows e num_cols
	char aux;
	fscanf(mz, "%c", &aux);
	num_rows = (int) aux + 48;
	fscanf(mz, "%c", &aux);
	num_cols = (int) aux + 48;

	// Aloca a matriz maze (malloc)
	for (int i = 0; i < num_rows; ++i)
				// Aloca cada linha da matriz
		maze[i] = (char*) malloc(num_cols * sizeof(char));
	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]			CONFERIR ISSO DO I+1 DEPOIS
			// Se o valor for 'e' salvar o valor em initial_pos
			fscanf(mz, "%c", &maze[i+1][j]);
			if(maze[i+1][j] == 'e') {
				initial_pos.i = i+1;
				initial_pos.j = j;
			}
		}
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

int compare(pos_t pos, char value, pos_t* prox)
{
	if(maze[pos.i][pos.j+1] == value && pos.j+1 < num_cols)
	{
		prox->i = pos.i;
		prox->j = pos.j;
		if(value == 's') return 1;
	}
	if(maze[pos.i][pos.j-1] == value && pos.j+1 > 0)
	{
		if (prox->i > 0)
		{
			valid_positions.push(*prox);
		}
		prox->i = pos.i;
		prox->j = pos.j;
		if(value == 's') return 1;
	}
	if(maze[pos.i+1][pos.j] == value && pos.j+1 < num_rows)
	{
		if (prox->i > 0)
		{
			valid_positions.push(*prox);
		}
		prox->i = pos.i;
		prox->j = pos.j;
		if(value == 's') return 1;
	}
	if(maze[pos.i-1][pos.j] == value && pos.j+1 > 0)
	{
		if (prox->i > 0)
		{
			valid_positions.push(*prox);
		}
		prox->i = pos.i;
		prox->j = pos.j;
		if(value == 's') return 1;
	}

	if (!valid_positions.empty()) return 1;
	return 0;
}

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	
	pos_t prox = {0, 0};
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
		// Marcar a posição atual com o símbolo '.'
		// Limpa a tela
		// Imprime o labirinto
		
		/* Dado a posição atual, verifica quais sao as próximas posições válidas
			Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		 	e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		 	cada uma delas no vetor valid_positions
		 		- pos.i, pos.j+1
		 		- pos.i, pos.j-1
		 		- pos.i+1, pos.j
		 		- pos.i-1, pos.j
		 	Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	 	*/
	do
	{
		// TENHO QUE CRIAR UMA MATRIZ COM O CAMINHO FINAL? ACHO Q SIM NÉ, PRA PRINTAR
		//int i, j; //(colocar um post.atual)
		if(pos.i > 0, pos.i < num_rows, pos.j > 0, pos.j < num_cols)
		{
			if(compare(pos, 'r', &prox) /*== 1*/)
			{
				maze[prox.i][prox.j] = '.';
				return 1;
			}

			else
			{
				if(compare(pos, 'x', &prox) && valid_positions.size() == 1)
				{
					maze[pos.i][pos.j] = '.';
					pos = prox; 
				}
				if(compare(pos, 'x', &prox) && valid_positions.size() > 1)
				{
					while(valid_positions.empty()) // N sei, parece errado ainda
					{
						//pos = valid_positions.top();
						if(!walk(valid_positions.top()))
						{
							valid_positions.pop(); // Em tese vai remover o ponto que a gnt tava usando agora.
						}
					}
				}
				if(!compare(pos, 'x', &prox))
				{
					valid_positions.pop(); // Em tese vai remover o ponto que a gnt tava usando agora.
					pos = valid_positions.top();
					return 0;
				}

			}


			/*
			if(maze[pos.i][pos.j+1] == 'x' && pos.j+1 < num_cols)
			{
				prox.i = pos.i;
				prox.j = pos.j;
			}
			if(maze[pos.i][pos.j-1] == 'x' && pos.j+1 > 0)
			{
				if (prox.i > 0)
				{
					valid_positions.push(prox);
				}
				prox.i = pos.i;
				prox.j = pos.j;
			}
			if(maze[pos.i+1][pos.j] == 'x' && pos.j+1 < num_rows)
			{
				if (prox.i > 0)
				{
					valid_positions.push(prox);
				}
				prox.i = pos.i;
				prox.j = pos.j;
			}
			if(maze[pos.i-1][pos.j] == 'x' && pos.j+1 > 0)
			{
				if (prox.i > 0)
				{
					valid_positions.push(prox);
				}
				prox.i = pos.i;
				prox.j = pos.j;
			}*/


		}	
		
		/* code */
	} while (true);
	

		
	
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		/*if (!valid_positions.empty()) {
			pos_t next_position = valid_positions.top();
			valid_positions.pop();
		}*/
	return false;
}

int main(/*int argc, char* argv[]*/) {
	cout << "/n/n a";
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("maze.txt"/*argv[1]*/);
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	
	// Tratar o retorno (imprimir mensagem)
	
	
	return 0;
}
