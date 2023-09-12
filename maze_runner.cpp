#include <stdio.h>
#include <stack>
#include <vector>
#include <iostream>
#include <chrono> // Para std::chrono
#include <thread> // Para std::this_thread

using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows = 0;
int num_cols = 0;
int options = 0;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

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
			if(maze[i][j] == 'o') cout << "\x1b[31mo\x1b[0m";
			else printf("%c", maze[i][j]);
		}
		printf("\n");
	}
    printf("\n");
}

void cleanStack(stack<pos_t>* valid_positions_n){
    while (!valid_positions_n->empty())
    {
        valid_positions_n->pop();
    }
}

int compare(pos_t pos, char value, pos_t* prox, stack<pos_t>* valid_positions_n)
{
    cleanStack(valid_positions_n);
	options = 0;
	if(pos.j+1 < num_cols) if(maze[pos.i][pos.j+1] == value)
	{
		prox->i = pos.i;
		prox->j = pos.j+1;
		options++;
        valid_positions_n->push(pos);
		if(value == 's') return 1;
	}
	if(pos.j-1 >= 0) if(maze[pos.i][pos.j-1] == value)
	{
		prox->i = pos.i;
		prox->j = pos.j-1;
		options++;
        valid_positions_n->push(pos);
		if(value == 's') return 1;
	}
	if (pos.i+1 < num_rows) if(maze[pos.i+1][pos.j] == value)
	{
		prox->i = pos.i+1;
		prox->j = pos.j;
		options++;
        valid_positions_n->push(pos);
		if(value == 's') return 1;
	}
	if(pos.i-1 >= 0) if(maze[pos.i-1][pos.j] == value)
	{
		prox->i = pos.i-1;
		prox->j = pos.j;
		options++;
        valid_positions_n->push(pos);
		if(value == 's') return 1;
	}

	if (options > 0) return 1;
	return 0;
}

int contar = 0, totalThreads = 0;
bool sFound = 0;

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {

	int interacoes = 0;
    stack<pos_t> valid_positions;
	contar ++;

	pos_t prox = {0, 0};

    do
    {
        interacoes++;

        if(pos.i > 0, pos.i < num_rows, pos.j > 0, pos.j < num_cols)
		{
			if(compare(pos, 's', &prox, &valid_positions))
			{
				maze[pos.i][pos.j] = '.';
				sFound = 1;
				return 1;
			}
			else
			{
				if(compare(pos, 'x', &prox, &valid_positions))
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
						maze[pos.i][pos.j] = '.';
						maze[prox.i][prox.j] = 'o';
						pos.i = prox.i;
						pos.j = prox.j;

						vector<thread> valid_positions_threads;
						while(!valid_positions.empty())
						{
							maze[pos.i][pos.j] = '.';
							valid_positions_threads.push_back(thread(walk, pos));
							
							pos.i = valid_positions.top().i;
							pos.j = valid_positions.top().j;
							valid_positions.pop();
						}

						for (auto& th : valid_positions_threads) th.join();
                        
					}
				}
				else
				{
					maze[pos.i][pos.j] = '.';
					return 0;
				}
			}
		}
		else if(!valid_positions.empty())
		{
            return 0;
        }

        this_thread::sleep_for(chrono::milliseconds(50));

    }while(interacoes < (num_cols*num_rows));

    return 1;
}

void printTimer()
{
    do
	{
		system("clear");
		print_maze();
		this_thread::sleep_for(chrono::milliseconds(50));
	}while (true);
}

int main(int argc, char* argv[]){

    pos_t initial_pos = load_maze(argv[1]);    
    
    thread first(walk, initial_pos);
    thread timer(printTimer);

    first.join();
    timer.detach();
    return 1;
}