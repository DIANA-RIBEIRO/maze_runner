#include <stdio.h>
#include <stack>
#include <iostream>
#include <chrono> // Para std::chrono
#include <thread> // Para std::this_thread

using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows = 0;
int num_cols = 0;

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
	
	pos_t aux_initial_pos;
			// Abre o arquivo para leitura (fopen)
	FILE *mz = fopen(file_name, "r");
			// Le o numero de linhas e colunas (fscanf) 
			// e salva em num_rows e num_cols
	char aux = ' ';
    if (mz == NULL) printf("ERRO\n"); //Confere se o arquivo abriu corretamente

	//Transformar a entrada de char para int das linhas
    fscanf(mz, "%c", &aux);
    while(aux != ' ')
    {
        num_rows *= 10; 
        num_rows += (int) aux - 48;
        fscanf(mz, "%c", &aux);
    }

	//Transformar a entrada de char para int das colunas
    fscanf(mz, "%c", &aux);
    while(aux != '\n')
    {
        num_cols *= 10; 
        num_cols += (int) aux - 48;
        fscanf(mz, "%c", &aux);
    }

	// Aloca a matriz maze (malloc)
    maze = (char**) malloc(num_rows * sizeof(char*));

	for (int i = 0; i < num_rows; ++i)
    {
		// Aloca cada linha da matriz
		maze[i] = (char*) malloc(num_cols * sizeof(char));
    }

	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j <= num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em aux_initial_pos
            
            fscanf(mz, "%c", &maze[i][j]);
			if(maze[i][j] == 'e') {
				aux_initial_pos.i = i;
				aux_initial_pos.j = j;
			}
		}
	}
    
    fclose(mz);
	return aux_initial_pos;
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

int options, interacoes = 0;
int compare(pos_t pos, char value, pos_t* prox)
{
	options = 0;
	if(pos.j+1 < num_cols) if(maze[pos.i][pos.j+1] == value)
	{
		prox->i = pos.i;
		prox->j = pos.j+1;
		options++;
		if(value == 's') return 1;
	}
	if(pos.j-1 >= 0) if(maze[pos.i][pos.j-1] == value)
	{
		prox->i = pos.i;
		prox->j = pos.j-1;
		options++;
		if(value == 's') return 1;
	}
	if (pos.i+1 < num_rows) if(maze[pos.i+1][pos.j] == value)
	{
		prox->i = pos.i+1;
		prox->j = pos.j;
		options++;
		if(value == 's') return 1;
	}
	if(pos.i-1 >= 0) if(maze[pos.i-1][pos.j] == value)
	{
		prox->i = pos.i-1;
		prox->j = pos.j;
		options++;
		if(value == 's') return 1;
	}

	if (options > 0) return 1;
	return 0;
}

int contar = 0;

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	contar ++;

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
		interacoes++;
		system("clear");

		if(pos.i > 0, pos.i < num_rows, pos.j > 0, pos.j < num_cols)
		{
			if(compare(pos, 's', &prox))
			{
				maze[pos.i][pos.j] = '.';
				print_maze();
				return 1;
			}
			else
			{
				if(compare(pos, 'x', &prox))
				{
					if(options == 1)
					{
						maze[pos.i][pos.j] = '.';
						maze[prox.i][prox.j] = 'o';
						pos.i = prox.i;
						pos.j = prox.j;
					}
					else if(options > 1)
					{
						valid_positions.push(pos);
						maze[pos.i][pos.j] = '.';
						maze[prox.i][prox.j] = 'o';
						pos.i = prox.i;
						pos.j = prox.j;
					}
				}
				else
				{
					maze[pos.i][pos.j] = '.';
					pos.i = valid_positions.top().i;
					pos.j = valid_positions.top().j;
					valid_positions.pop();
				}
			}
		}
		else if(!valid_positions.empty())
		{
			pos.i = valid_positions.top().i;
            pos.j = valid_positions.top().j;
			valid_positions.pop();
		}

		print_maze();
		this_thread::sleep_for(std::chrono::milliseconds(50));

	} while (interacoes < (num_cols*num_rows));
	
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		
	return false;
}

int main(int argc, char* argv[]){

    pos_t initial_pos = load_maze(argv[1]);
    // chamar a função de navegação
	bool exit_found = walk(initial_pos);
    return 0;
}
