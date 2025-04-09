#include "xil_types.h"
#include "xil_assert.h"

// �w�q�C���ϰ쪺��ɼe��
#define   WIDTH          16
#define   HEIGHT         16
// �w�q�D���̤j����
#define   MAX_LENGHT   15
// �w�q�C����l�t��
//#define   SPEED        10
// �w�q�������ͳt��
// 0.5s ����
//
#define	  GFOOD_GENERATE_SPEED 	250
#define   BFOOD_GENERATE_SPEED 	250
#define   SNAKE_SPEED			250
typedef enum{
	OBJECT_EMPTY  = 0,
	OBJECT_BORDER = 1,
	OBJECT_SNAKE = 2,
	OBJECT_GFOOD = 3,
	OBJECT_BFOOD = 4
}gameobject;
/*��e�C�����A*/
typedef enum {
	Lose 	 = 0,
	Continue = 1,	//�i�椤
	Win 	 = 2
}State;
/*��e�D����V*/
typedef enum {
	DIRECTION_UP	= 72,
	DIRECTION_DOWN  = 80,
	DIRECTION_LEFT  = 75,
	DIRECTION_RIGHT = 77
}Direction;
// �w�q�C��������Ӫ���G�����M�D
typedef struct {
	int x;
	int y;
}fruit;
typedef struct {
	int len;
	int x_buf[MAX_LENGHT];
	int y_buf[MAX_LENGHT];
	Direction SnakeDirection;
	int score;
	int life;
}snake;
//eatenfood Buffer
typedef struct{
	int foodtype;
	int x;
	int y;
}EatenFood;
typedef struct{
	EatenFood foodbuffer [20];
	int EatenFoodBufferLength;
	int EatenFoodBufferIndex ;
}eatenbuffer;

typedef struct{
	int gfood_count;
	int bfood_count;
}foodcount;

//Boundary
void CreateBorder(int** screen);
//Snake
void CreateSnake(int** screen,snake *Snake,eatenbuffer *EatenfoodBuffer);
//Print Screen buffer
//void DrawScreen(int** screen);

//�H���гy�@��grow apple
void CreateGFood(snake *Snake,int** foodarray,int** bfoodarray,fruit *food,foodcount* bgfood_count);
//�H���гy�@��degraded apple
void CreateBFood(snake *Snake,int** foodarray,int** bfoodarray,fruit *bfood,foodcount* bgfood_count);

//Update screen buffer
void Update(snake *Snake,int** foodarray,int** bfoodarray,int** screen,eatenbuffer *EatenfoodBuffer,foodcount* bgfood_count);
//ISR Read press Key
//void Keypress(snake *Snake);
//Snake Move Direction
void MoveUp(snake *Snake);
void MoveDown(snake *Snake);
void MoveLeft(snake *Snake);
void MoveRight(snake *Snake);

//Snake eat food?
void iseatfood(snake *Snake,int** foodarray,int** bfoodarray,int** screen,eatenbuffer *EatenfoodBuffer,foodcount* bgfood_count);
//Game Status has changed?
int check(snake *Snake,int** screen,int** foodarray,int** bfoodarray,int GameState,foodcount* bgfood_count);

void TwoDArraytoOneDArray(int** screen,u8* TransmitBuffer,snake *Snake);

