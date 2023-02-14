#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define ROCK "moves/rock.txt"
#define PAPER "moves/paper.txt"
#define SCISSOR "moves/scissor.txt"

#define TARGET_SCORE 10
#define PLAY_AGAIN '0'
#define BUFFER_SIZE 300

int player = 0, cpu = 0; // scores
char moveset[6][BUFFER_SIZE];    // ASCII arts

typedef enum color
{
	RED = 12, GREEN = 10, YELLOW = 14
}color;

static inline void reset()
{
	player = 0;
	cpu = 0;
}
static inline void gotoxy( short x, short y)
{
	COORD pt = { x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pt);
}
static inline void colorf( const char* msg, color hue)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), hue);
	printf("%s", msg);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
void readmoveset(char** files)
{
	FILE* f;
	for( int i = 0; i < 3; i++)
	{
		fopen_s( &f, files[i], "r");
		fscanf_s( f, "%[^X]", moveset[2*i], sizeof(moveset[i]));
		fscanf_s( f, "%[^NULL]", moveset[(2*i)+1], sizeof(moveset[i]));
		moveset[(2*i)+1][0] = ' ';
	}
	fclose(f);
}

static inline void update_score()
{
	gotoxy( 4, 0);
	printf("SCORES");
	gotoxy( 2, 2);
	printf("PLAYER | CPU");
	gotoxy( 2, 3);
	printf("%6d | %3d", player, cpu);
}
bool xwins( int x, int y) // win condition for p1 (x)
{
	short eval = y+1;
	if(eval == 3) eval = 0;
	if(eval == x)
		return true;
	return false;
}
void display_result( int pmove, int cmove)
{
	if(xwins( pmove, cmove))
	{
		colorf("\n\n\n\t\t  YOU WIN!", GREEN);
		player++;
	}
	else if(xwins( cmove, pmove))
	{
		colorf( "\n\n\n\t\t  YOU LOSE!", RED);
		cpu++;
	}
	else
		colorf( "\n\n\n\t\t  DRAW..", YELLOW);
}
void draw(int move) // utility fn for identifying moves
{
	static int turn = 0;
	switch(move)
	{
		case 0 :
			printf("%s\n", moveset[(turn%2 != 0)?0:1]);
			break;
		case 1 :
			printf("%s\n", moveset[(turn%2 != 0)?2:3]);
			break;
		case 2 :
			printf("%s\n", moveset[(turn%2 != 0)?4:5]);
			break;
	}
	turn++;
}

short rng() // random number generation [CPU's input]
{
	int move = rand();
	move = move%2?move*2:move/3;
	return move%3;
}

int input() // input and validation [Player's input]
{
	int move;
	printf("MAKE YOUR MOVE :");
	printf("\n\n%64s 1 : ROCK\n%64s 2 : PAPER\n%64s 3 : SCISSOR", "PRESS", "PRESS", "PRESS");
	while(1)
	{
		move = _getch() - 49;
		if(move <= 2 && move >= 0)
			break;
		else printf("\a");
	}
	return move;
}

void cycle() // user input + rng() = one cycle
{
	gotoxy( 25, 3);
	printf("ROCK.. PAPER.. SCISSOR!!");
	gotoxy( 60, 5);
	int cmove = rng();
	int pmove = input();
	gotoxy( 40, 5);
	draw(pmove);
	printf("\n%22s\n", "VS");
	draw(cmove);
	display_result( pmove, cmove);
}
char game_over()
{
	if(player == TARGET_SCORE)
		printf("WOO HOO! YOU WON!!");
	else
		printf("YOU LOST.. :[");

	gotoxy( 55, 17);
	_getch();
	printf("Press 0 to play again! ^_^");

	return _getch();
}

int main() // driver code
{
	char* files[] = { ROCK, PAPER, SCISSOR};
	srand(time(NULL));
	readmoveset(files);

	while(1)
	{
		update_score();
		cycle();

		if((cpu == TARGET_SCORE || player == TARGET_SCORE))
		{
			update_score();
			gotoxy( 60, 15);
			Sleep(100);

			if(game_over() == PLAY_AGAIN) reset();
			else break;
		}
		else
		{
			gotoxy( 60, 14);
			system("pause");
		}
		system("cls");
	}

	system("cls");
	printf("\n\n\tSEE YA!!\n");
	return 0;
}
