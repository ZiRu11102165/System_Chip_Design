/*
	程式流程 
	Step1:  初始化邊界 	---> Inital()
	Step2: 	初始化蛇-----|
	main(){
		Step3:	產生好食物 
		Step4:	產生壞食物 
		Step5:  偵測使用者輸入 (change snake dirction) 
		Step6:	吃到沒? 
	}
		eat();
		
		move();
*/
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>


// 定義遊戲區域的邊界寬度
#define   WIDTH        100
#define   HIGH         20 
// 定義蛇的最大長度
#define   MAX_LENGHT   20
// 定義遊戲初始速度
//#define   SPEED        10
// 定義食物產生速度
// 0.5s 產生
#define	  GFOOD_GENERATE_SPEED 250

#define   BFOOD_GENERATE_SPEED 250

/*當前遊戲狀態*/
enum{
	Lose 	 = 0,	 
	Continue = 1,	//進行中 
	Win 	 = 2		
}GameState;
/*當前蛇的方向*/ 
enum{
	UP	= 72,
	DOWN =  80,
	LEFT = 75,
	RIGHT = 77
}SnakeDirection;


// 定義遊戲中的兩個物件：食物和蛇
struct
{
	int x;
	int y;
}food;

struct
{
	int x;
	int y;
}bfood;

struct
{
	int len;
	int x_buf[MAX_LENGHT];
	int y_buf[MAX_LENGHT];
	int score;
}snake;

//eatenfood Buffer
typedef struct{
	int foodtype;
	int x;
	int y;
}EatenFood;

int a,b,c,d;			
int life;				//生命 
int eatfood;

int foodarray[WIDTH][HIGH]={0};		/*宣告一個 WIDTH * HIGH 的空food陣列*/	
int bfoodarray[WIDTH][HIGH]={0};		/*宣告一個 WIDTH * HIGH 的空bfood陣列*/		

//snake head
int snake_head_x;
int snake_head_y;
int EatenFoodBufferLength = 0;
int EatenFoodBufferIndex = -1;


char screen[WIDTH][HIGH]; 

EatenFood EatenFoodBuffer[20];

void Initial(void);
void CreateBorder(void);
void CreateSnake(void);

/*隨機創造一個好食物*/
void CreateGFood(void);
/*隨機創造一個壞食物*/
void CreateBFood(void);

void gotoxy(int x, int y);

void KeyPress(void);
void MoveUp(void);
void MoveDown(void);
void MoveLeft(void);
void MoveRight(void);

void DrawScreen(void);
void Update(void);

void iseatfood(void);
int check(void);


int main(void)
{
	int i,col,row,j;
	//initial
	Initial();
	
	
	
	while (GameState)
	{
		
		CreateGFood();
		CreateBFood();
		Update();
		DrawScreen();
		if(check() == Lose || check() == Win){
			break;
		}
		 
		
	}
}


void Initial(void){
	CreateBorder();		//border
	CreateSnake();	//snake
	GameState = Continue;
}

/*繪製遊戲區域邊界*/ 	//0 
void CreateBorder(void){
	int x, y;
	for (y = 0; y < HIGH ; y++){
		for (x = 0; x < WIDTH ; x++){
			if(x == 0 || x == WIDTH - 1 || y == 0 || y == HIGH - 1){
				screen[x][y] = '0';
			}
			else{
				screen[x][y] = ' ';
			}
		}
	}
} 

/*初始化小蛇*/ 
void CreateSnake(void){	//0 
	SnakeDirection = RIGHT;
	int /*orgin_x, orgin_y,*/i;
	
	life = 3;
	
	/*orgin_x = WIDTH / 2 + 2;
	orgin_y = HIGH / 2 + 1;*/

	snake.len = 5;
	snake.x_buf[0] = 5; snake.y_buf[0] = 1;
	snake.x_buf[1] = 4; snake.y_buf[1] = 1;
	snake.x_buf[2] = 3; snake.y_buf[2] = 1;
	snake.x_buf[3] = 2; snake.y_buf[3] = 1;
	snake.x_buf[4] = 1; snake.y_buf[4] = 1;
	snake.score = 5;

	for (i = 0; i < snake.len ; i++){
		screen[snake.x_buf[i]][snake.y_buf[i]] ='0';
	}
}

/*隨機創造一個好食物*/	//0?
void CreateGFood(void){	
	int flag = 0,i;
	do{
		food.x = (rand() % (WIDTH- 2)) + 1;	
		food.y = (rand() % (HIGH - 2)) + 1; //HIGH - 1 = 19 rand()%19 = 0 ~ 18 => + 1 => 1 ~ 19
		// 判斷生成的食物是否和蛇身重合
		for (i = 0; i < snake.len; i++)
		{
			if (snake.x_buf[i] == food.x && snake.y_buf[i] == food.y)
			{
				flag = 1;
				break;
			}
			else{
				flag = 0;
			}
		}
		if(flag == 0){
			/*判斷食物是否重疊*/
			if(foodarray[food.x][food.y] == 1 || bfoodarray[food.x][food.y] == 1){
				flag = 1;
			}
			else{
				flag = 0;
			}
		} 
	} while (flag);
	foodarray[food.x][food.y] = 1;		/*將食物位置存入food陣列相對應位置*/
	a = food.x;
	b = food.y;
	Sleep(GFOOD_GENERATE_SPEED);
}


/*隨機創造一個壞食物*/	//0?
void CreateBFood(void){	
	int flag = 0,i;
	
	do{
		bfood.x = (rand() % (WIDTH -2)) + 1;
		bfood.y = (rand() % (HIGH - 2)) + 1;
		// 判斷生成的食物是否和蛇身重合
		for (i = 0; i < snake.len; i++)
		{
			if (snake.x_buf[i] == bfood.x && snake.y_buf[i] == bfood.y)
			{
				flag = 1 ;
				break;
			}
			else{
				flag = 0 ;
			}
		}
		if(flag == 0){
			/*判斷食物是否重疊*/
			if(foodarray[bfood.x][bfood.y] == 1 || bfoodarray[bfood.x][bfood.y] == 1){
				flag = 1;
			}
			else{
				flag = 0;
			}
		} 
	} while (flag);
	
	bfoodarray[bfood.x][bfood.y] = 1;		/*將食物位置存入food陣列相對應位置*/
	c = bfood.x;
	d = bfood.y;
	Sleep(BFOOD_GENERATE_SPEED);
}

/*控制游標所在的位置*/ 
void gotoxy(int x, int y){
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void KeyPress(void){
	int key;

	if (_kbhit())//如果使用者按下了鍵盤中的某個鍵
	{
		fflush(stdin);//清空緩衝區的字元

		//getch()讀取方向鍵的時候，會返回兩次，第一次呼叫返回0或者224，第二次呼叫返回的才是實際值
		key = _getch();//第一次呼叫返回的不是實際值
		key = _getch();//第二次呼叫返回實際值
	}

	// 小蛇移動方向不能和上一次的方向相反
	switch(key){
		case UP:
			if(SnakeDirection != DOWN)
				SnakeDirection = UP;
			break;
		case DOWN:
			if(SnakeDirection != UP)
				SnakeDirection = DOWN;
			break;
		case LEFT:
			if(SnakeDirection != RIGHT)
				SnakeDirection = LEFT;
			break;
		case RIGHT:
			if(SnakeDirection != LEFT)
				SnakeDirection = RIGHT;
			break;
		default:
			break;
	}

	
	/*snake_head_x = x;
	snake_head_y = y;
	SnakeMove(x, y);*/
} 

void MoveUp(void){
	int i;
	for(i = snake.len -1 ; i >= 1 ; i--){
		snake.x_buf[i] = snake.x_buf[i-1];
		snake.y_buf[i] = snake.y_buf[i-1];
	}
	snake.y_buf[0] = snake.y_buf[0] - 1 ;
}

void MoveDown(void){
	int i;
	for(i = snake.len -1 ; i >= 1 ; i--){
		snake.x_buf[i] = snake.x_buf[i-1];
		snake.y_buf[i] = snake.y_buf[i-1];
	}
	snake.y_buf[0] = snake.y_buf[0] + 1 ;
}

void MoveLeft(void){
	int i;
	for(i = snake.len -1 ; i >= 1 ; i--){
		snake.x_buf[i] = snake.x_buf[i-1];
		snake.y_buf[i] = snake.y_buf[i-1];
	}
	snake.x_buf[0] = snake.x_buf[0] - 1 ;
}

void MoveRight(void){
	int i;
	for(i = snake.len -1 ; i >= 1 ; i--){
		snake.x_buf[i] = snake.x_buf[i-1];
		snake.y_buf[i] = snake.y_buf[i-1];
	}
	snake.x_buf[0] = snake.x_buf[0] + 1 ;
}

void DrawScreen(void){
	int i,j;
	gotoxy(0,0);
	for(j = 0 ; j < HIGH  ; j++){
		for(i = 0 ; i < WIDTH ; i++){
			printf("%c",screen[i][j]);
		}	
		printf("\n");
	}
}

void Update(void){
	int i,j;
	
	KeyPress();
	switch (SnakeDirection){
	case LEFT:
		MoveLeft();//往左
		break;
	case RIGHT:
		MoveRight();//往右
		break;
	case UP:
		MoveUp();//往上
		break;
	case DOWN:
		MoveDown();//往下
		break;
	}
	CreateBorder();
	for(i = 0 ; i < snake.len ; i++){
		screen[snake.x_buf[i]][snake.y_buf[i]] = '0';
	}
	iseatfood();
	for(j = 0 ; j < HIGH ; j++){
		for(i = 0 ; i < WIDTH ; i++){
			if(foodarray[i][j] == 1){
				screen[i][j] = '+';
			}
			if(bfoodarray[i][j] == 1){
				screen[i][j] = '-';
			}
		}
	}	
}

void iseatfood(void){
	int i;
	//長大往前丟  
	// 0 1 2 3 4 5 
	// 1 2 3 4 5 
	//grow
	if( (snake.x_buf[snake.len-1] == EatenFoodBuffer[EatenFoodBufferIndex].x) && 
		(snake.y_buf[snake.len-1] == EatenFoodBuffer[EatenFoodBufferIndex].y)){
		if(EatenFoodBufferIndex >= 0){
			if(EatenFoodBuffer[EatenFoodBufferIndex].foodtype == 1){
				snake.x_buf[snake.len] 		= EatenFoodBuffer[EatenFoodBufferIndex].x;
				snake.y_buf[snake.len] 		= EatenFoodBuffer[EatenFoodBufferIndex].y;
				snake.x_buf[snake.len + 1] 	= EatenFoodBuffer[EatenFoodBufferIndex].x;
				snake.y_buf[snake.len + 1] 	= EatenFoodBuffer[EatenFoodBufferIndex].y;

				for(i = EatenFoodBufferLength - 1 ; i >= 1  ; i--){
						EatenFoodBuffer[i].foodtype = EatenFoodBuffer[i-1].foodtype;
						EatenFoodBuffer[i].x = EatenFoodBuffer[i-1].x;
						EatenFoodBuffer[i].y = EatenFoodBuffer[i-1].y;
					}
				
				EatenFoodBufferIndex = EatenFoodBufferIndex - 1;
				EatenFoodBufferLength = EatenFoodBufferLength - 1;
				snake.len = snake.len + 2;
			}
			else{
				
				for(i = EatenFoodBufferLength - 1 ; i >= 1  ; i--){
						EatenFoodBuffer[i].foodtype = EatenFoodBuffer[i-1].foodtype;
						EatenFoodBuffer[i].x = EatenFoodBuffer[i-1].x;
						EatenFoodBuffer[i].y = EatenFoodBuffer[i-1].y;
					}
				
				EatenFoodBufferIndex = EatenFoodBufferIndex - 1;
				EatenFoodBufferLength = EatenFoodBufferLength - 1;
				snake.len = snake.len - 1;
			}
		}
	}
	//吃掉往後丟  
	// 0 1 2 3 4 5 
	//   0 1 2 3 4 5 
	/*判斷吃蘋果*/
	if (foodarray[snake.x_buf[0]][snake.y_buf[0]] == 1)
	{
		/*
			--------------------------------------------
			^
			new
		*/
		EatenFoodBufferLength = EatenFoodBufferLength + 1;
		if(EatenFoodBufferIndex + 1 >= 0){
			EatenFoodBufferIndex = EatenFoodBufferIndex + 1;
			for(i = 0  ; i + 1 < EatenFoodBufferLength ; i++){
				EatenFoodBuffer[i+1].foodtype = EatenFoodBuffer[i].foodtype;
				EatenFoodBuffer[i+1].x = EatenFoodBuffer[i].x;
				EatenFoodBuffer[i+1].y = EatenFoodBuffer[i].y;
			}
			EatenFoodBuffer[0].foodtype = 1;//1 :good type 0:bad type
			EatenFoodBuffer[0].x = snake.x_buf[0];
			EatenFoodBuffer[0].y = snake.y_buf[0];
		}
		snake.score = snake.score + 2;
		
		foodarray[snake.x_buf[0]][snake.y_buf[0]] = 0;
		screen[snake.x_buf[0]][snake.y_buf[0]] = '0';
		
		
	}
	if (bfoodarray[snake.x_buf[0]][snake.y_buf[0]] == 1)
	{
		
		EatenFoodBufferLength = EatenFoodBufferLength + 1;
		if(EatenFoodBufferIndex + 1 >= 0){
			EatenFoodBufferIndex = EatenFoodBufferIndex + 1;
			for(i = 0  ; i + 1 < EatenFoodBufferLength ; i++){
				EatenFoodBuffer[i+1].foodtype = EatenFoodBuffer[i].foodtype;
				EatenFoodBuffer[i+1].x = EatenFoodBuffer[i].x;
				EatenFoodBuffer[i+1].y = EatenFoodBuffer[i].y;
			}
			EatenFoodBuffer[0].foodtype = 0;//1 :good type 0:bad type
			EatenFoodBuffer[0].x = snake.x_buf[0];
			EatenFoodBuffer[0].y = snake.y_buf[0];
		}
		snake.score = snake.score-1;
		bfoodarray[snake.x_buf[0]][snake.y_buf[0]] = 0; 
		screen[snake.x_buf[0]][snake.y_buf[0]] = '0';

	}
	
	
}

/*檢查*/
int check(void){
	int i,col,row;
	 
	/**************************** 失敗條件 ****************************/ 
	
	if (snake.len < 4){
		system("cls");
		printf("Game Over!\n");
		GameState = Lose;
		return GameState;
	}
	else{ 
		if (snake.x_buf[0] == 0 || snake.x_buf[0] == WIDTH || snake.y_buf[0] == 0 || snake.y_buf[0] == HIGH ){
			life--;
			for(col = 0  ; col < HIGH  ; col++){
				for(row = 0  ; row < WIDTH  ; row++){
					foodarray[row][col]  = 0;
					bfoodarray[row][col] = 0;
					screen[row][col] = ' ';
				}
			}
			//redraw snake
			for (i = 0; i < snake.len; i++){
				snake.x_buf[i] = (snake.len-i) + 1 ; snake.y_buf[i] = 1;
			}
			SnakeDirection = RIGHT;
			if(life == 0){
				GameState = Lose;
				return GameState;
			}
			else{
				GameState = Continue;
				system("cls");
				return GameState;
			}
		}
		
		/*撞到自己*/
		for (i = 1; i < snake.len; i++){
			if ( (snake.x_buf[0] == snake.x_buf[i] ) && (snake.y_buf[0] == snake.y_buf[i])){
				life--;
				for(col = 0  ; col < HIGH  ; col++){
					for(row = 0  ; row < WIDTH  ; row++){
						foodarray[row][col]  = 0;
						bfoodarray[row][col] = 0;
						screen[row][col] = ' ';
					}
				}
				//redraw snake
				for (i = 0; i < snake.len; i++){
					snake.x_buf[i] = (snake.len-i) + 1 ; snake.y_buf[i] = 1;
				}
				SnakeDirection = RIGHT;
				if(life == 0){
					GameState = Lose;
					return GameState;
				}
				else{
					GameState = Continue;
					system("cls");
					return GameState;
				}
			}
		}
	}
	 
	/**************************** 勝利條件 ****************************/ 
	if (snake.len >= MAX_LENGHT) {//到達最大長度勝利
		system("cls");
		printf("Your are win!\n");
		GameState = Win;
		return GameState;
	}
	
	// 列印得分
	//gotoxy(0, HIGH + 3);
	//printf("\nYour score: %1d", snake.score);
	printf("\nsnake length : %d",snake.len);
	printf("\nYour life: %d", life);
	
	printf("\n蛇頭X%d，蛇頭Y%2d",snake.x_buf[0],snake.y_buf[0]);
	printf("food.x:%3d,food.y:%3d",a,b);	
	printf("      bfood.x:%3d,bfood.y:%3d",c,d);
	return GameState;
	//printf("      %d\n",eatfood);
}
