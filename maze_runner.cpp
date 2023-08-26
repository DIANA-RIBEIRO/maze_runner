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
    if (mz == NULL) printf("ERRO\n");

    fscanf(mz, "%c", &aux);
    while(aux != ' ')
    {
        num_rows *= 10; 
        num_rows += (int) aux - 48;
        fscanf(mz, "%c", &aux);
    }

    fscanf(mz, "%c", &aux);
    while(aux != '\n')
    {
        num_cols *= 10; 
        num_cols += (int) aux - 48;
        fscanf(mz, "%c", &aux);
    }

    //cout << "\nFINAL " << num_rows << " " << num_cols << "\n";
    maze = (char**)malloc(num_rows * sizeof(char*));
	// Aloca a matriz maze (malloc)
	for (int i = 0; i < num_rows; ++i)
    {
				// Aloca cada linha da matriz
        
		maze[i] = (char*) malloc(num_cols * sizeof(char));  //LEMBRAR DE USAR O FREE

        /*char** arrayOfStrings = (char**)malloc(numStrings * sizeof(char*));
        for (int i = 0; i < numStrings; ++i) {
            // Alocar memória para cada string individual
            arrayOfStrings[i] = (char*)malloc(stringSize * sizeof(char));
            // Agora você pode copiar ou atribuir valores às strings individuais
            snprintf(maze[i], num_cols, "String %d", i);
        }*/
        
    }

	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j <= num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]			CONFERIR ISSO DO I+1 DEPOIS
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

int options;
int count = 0;
int compare(pos_t pos, char value, pos_t* prox)
{
	options = 0;
	//if (count == 142) return 1;
																	//if (count > 141) cout << value << "\n1:" << maze[pos.i][pos.j+1];
	if(pos.j+1 < num_cols) if(maze[pos.i][pos.j+1] == value)
	{
        															//if (count > 141) cout << " <<<  ";
		prox->i = pos.i;
		prox->j = pos.j+1;
		options++;
		if(value == 's') return 1;
	}
																	//if (count > 141) cout << "\n2:" << maze[pos.i][pos.j-1];
	if(pos.j-1 >= 0) if(maze[pos.i][pos.j-1] == value)
	{
        															//if (count > 141) cout << " <<<  ";
		prox->i = pos.i;
		prox->j = pos.j-1;
		options++;
		if(value == 's') return 1;
	}
																	//if (count > 141) cout << "\n3:" << maze[pos.i+1][pos.j] << pos.i;
	
	if (pos.i+1 < num_rows) if(maze[pos.i+1][pos.j] == value)
	{
        															//if (count > 141) cout << " <<<";
		prox->i = pos.i+1;
		prox->j = pos.j;
		options++;
		if(value == 's') return 1;
	}
																	////if (count > 141) cout << "\n4:" << maze[pos.i-1][pos.j] << pos.i << endl;
	if(pos.i-1 >= 0) if(maze[pos.i-1][pos.j] == value)
	{
        															//if (count > 141) cout << " <<<\n";
		prox->i = pos.i-1;
		prox->j = pos.j;
		options++;
		if(value == 's') return 1;
	}

	//if(value == 's') return 0;
	if (options > 0) return 1;
	cout << "ERRO\n";
	return 0;
}

int contar = 0;

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	contar ++;
	cout << "TÁ NO WALK " << num_cols << num_rows << "\n\n";
	cout << pos.i << num_cols << pos.j << num_rows << endl;
	cout << maze[15][0] << " " << maze[15][1] << endl;
	cout << maze[16][0] << " " << maze[16][1] << endl;

	print_maze();
	pos_t prox = {0, 0};

	//this_thread::sleep_for(std::chrono::seconds(5));
    //printf("%s", "\nbb\n");
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
		count++;
		// TENHO QUE CRIAR UMA MATRIZ COM O CAMINHO FINAL? ACHO Q SIM NÉ, PRA PRINTAR
		//int i, j; //(colocar um post.atual)
		if(pos.i > 0, pos.i < num_rows, pos.j > 0, pos.j < num_cols)
		{
			
																		cout << "i: " << pos.i << ", j:" << pos.j << endl;
																		if (count > 240) print_maze();
																		//cout << maze[pos.i][pos.j] << "\n\n";
            
			if(compare(pos, 's', &prox) /*== 1*/)
			{
				maze[pos.i][pos.j] = '.';
				maze[prox.i][prox.j] = '.';
                //pos = prox
                																			//printf("%s", "\n cc\n");
				return 1;
			}
			else
			{
				if(compare(pos, 'x', &prox))
				{
																						//cout << prox.i << " " << prox.j << endl;
																						//print_maze();
					if(options == 1)
					{
																									//printf("%s", "\n dd1\n");
						//cout << maze[pos.i][pos.j] << "%" << maze[prox.i][prox.j];
						maze[pos.i][pos.j] = '.';
						pos.i = prox.i;
						pos.j = prox.j;
																									//printf("%s", "\n dd11\n");
						//valid_positions.pop();
					}
					else if(options > 1)
					{
						/*																		//cout << "primeiro laço\t";
						while(!valid_positions.empty()) // N sei, parece errado ainda
						{
							cout << "WHILE\n";
							//pos = valid_positions.top();
							//this_thread::sleep_for(std::chrono::seconds(1));
																									maze[pos.i][pos.j] = '.';
																									pos.i = prox.i;
																									pos.j = prox.j;
							if(!walk(valid_positions.top()))
							{
								cout << "WALK\n";
								this_thread::sleep_for(std::chrono::seconds(5));
								valid_positions.pop(); // Em tese vai remover o ponto que a gnt tava usando agora.
							}
							cout << "pulou o While";
							this_thread::sleep_for(std::chrono::seconds(5));
						}
																									//printf("%s", "\n dd2\n");
						*/

						valid_positions.push(pos);
						maze[pos.i][pos.j] = '.';
						pos.i = prox.i;
						pos.j = prox.j;
					}
					else
					{
						cout << "Compare == 1 para x";
						// ALEATÓRIO
						valid_positions.pop();
					}
				}
				else
				{
																								//printf("%s", "\n vazio\n");
					maze[pos.i][pos.j] = '.';
					pos.i = valid_positions.top().i;
					pos.j = valid_positions.top().j;
					valid_positions.pop(); // Em tese vai remover o ponto que a gnt tava usando agora.
                    																			//printf("%s", "\n ee2\n.");
					//printf("%c", valid_positions.top().i);
																								//cout << valid_positions.top().i ;
																								//cout << ".\n";
                   // printf("%s", "\n ee1\n");

					/*pos.i = valid_positions.top().i;
                    pos.j = valid_positions.top().j;
                   // ALEATÓRIO
				    valid_positions.pop();
					return 0;*/
					cout << "Compare == 0 para x";
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
		else if(!valid_positions.empty())
		{
			cout << "Fim da linha";
			pos.i = valid_positions.top().i;
            pos.j = valid_positions.top().j;
			valid_positions.pop();
		}
		//printf("%s", "\n ee\n");
		/* code */
		cout << "\nLOOP " << count << ": ";
	} while (count < (num_cols*num_rows));
	
	cout << "\n"; print_maze();

		printf("%s", "\n ff\n");
	
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		/*if (!valid_positions.empty()) {
			pos_t next_position = valid_positions.top();
			valid_positions.pop();
		}*/
	return false;
}

int main(){

    

    pos_t initial_pos = load_maze("../data/maze7.txt"/*argv[1]*/);
    // chamar a função de navegação
	bool exit_found = walk(initial_pos);
	cout << "\n\n";
	print_maze();
	cout << "FIM\n";
    return 0;
}