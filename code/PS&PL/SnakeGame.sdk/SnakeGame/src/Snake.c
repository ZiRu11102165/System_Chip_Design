#include "snake.h"
#include <stdlib.h>

int check(snake *Snake,int** screen,int** foodarray,int** bfoodarray,int GameState,foodcount* bgfood_count){
	u8 i,col,row;

	/**************************** 失敗條件 ****************************/

	if (Snake->len == 3){
		GameState = Lose;
		return GameState;
	}
	else{
		if (Snake->x_buf[0] == 0 || Snake->x_buf[0] == WIDTH - 1 || Snake->y_buf[0] == 0 || Snake->y_buf[0] == HEIGHT - 1){
			Snake->life = Snake->life - 1;
			for(col = 0  ; col < HEIGHT  ; col++){
				for(row = 0  ; row < WIDTH  ; row++){
					foodarray[row][col]  = OBJECT_EMPTY;
					bfoodarray[row][col] = OBJECT_EMPTY;
					screen[row][col]     = OBJECT_EMPTY;
				}
			}
			//redraw snake
			for (i = 0; i < Snake->len; i++){
				if(i < 13){
					Snake->x_buf[i] = (Snake->len-i) + 1 ;
					Snake->y_buf[i] = 1;
				}
				else{
					Snake->x_buf[i] = 13 ;
					Snake->y_buf[i] = i - 12;
				}

			}
			bgfood_count ->bfood_count = 0;
			bgfood_count ->gfood_count = 0;
			if(Snake->len > 13){
				Snake->SnakeDirection = DIRECTION_DOWN;
			}
			else{
				Snake->SnakeDirection = DIRECTION_RIGHT;
			}

			if(Snake->life == 0){
				GameState = Lose;
				return GameState;
			}
			else{
				GameState = Continue;
				//system("cls");
				return GameState;
			}
		}
	}
	//沒命的話，遊戲結束
	if(Snake->life == 0){
		GameState = Lose;
		return GameState;
	}
	else{
		GameState = Continue;
		//system("cls");
		return GameState;
	}
	/**************************** 勝利條件 ****************************/
	if (Snake->len >= MAX_LENGHT) {//到達最大長度勝利
		//system("cls");
		//printf("Your are win!\n");
		GameState = Win;
		return GameState;
	}

	return GameState;
}


void iseatfood(snake *Snake,int** foodarray,int** bfoodarray,int** screen,eatenbuffer *EatenFoodBuffer,foodcount* food_count){
	u8 i;

	//grow
	if( (Snake->x_buf[Snake->len-1] == EatenFoodBuffer->foodbuffer[EatenFoodBuffer->EatenFoodBufferIndex].x) &&
		(Snake->y_buf[Snake->len-1] == EatenFoodBuffer->foodbuffer[EatenFoodBuffer->EatenFoodBufferIndex].y)){
		if(EatenFoodBuffer->EatenFoodBufferIndex >= 0){
			if(EatenFoodBuffer->foodbuffer[EatenFoodBuffer->EatenFoodBufferIndex].foodtype == 1){
				Snake->x_buf[Snake->len] 		= EatenFoodBuffer->foodbuffer[EatenFoodBuffer->EatenFoodBufferIndex].x;
				Snake->y_buf[Snake->len] 		= EatenFoodBuffer->foodbuffer[EatenFoodBuffer->EatenFoodBufferIndex].y;
				Snake->x_buf[Snake->len + 1] 	= EatenFoodBuffer->foodbuffer[EatenFoodBuffer->EatenFoodBufferIndex].x;
				Snake->y_buf[Snake->len + 1] 	= EatenFoodBuffer->foodbuffer[EatenFoodBuffer->EatenFoodBufferIndex].y;
				for(i = EatenFoodBuffer->EatenFoodBufferLength - 1 ; i >= 1  ; i--){
						EatenFoodBuffer->foodbuffer[i].foodtype = EatenFoodBuffer->foodbuffer[i-1].foodtype;
						EatenFoodBuffer->foodbuffer[i].x        = EatenFoodBuffer->foodbuffer[i-1].x;
						EatenFoodBuffer->foodbuffer[i].y        = EatenFoodBuffer->foodbuffer[i-1].y;
					}
				EatenFoodBuffer->EatenFoodBufferIndex  = EatenFoodBuffer->EatenFoodBufferIndex - 1;
				EatenFoodBuffer->EatenFoodBufferLength = EatenFoodBuffer->EatenFoodBufferLength - 1;
				Snake->len = Snake->len + 2;
			}
			else{
				for(i = EatenFoodBuffer->EatenFoodBufferLength - 1 ; i >= 1  ; i--){
						EatenFoodBuffer->foodbuffer[i].foodtype = EatenFoodBuffer->foodbuffer[i-1].foodtype;
						EatenFoodBuffer->foodbuffer[i].x        = EatenFoodBuffer->foodbuffer[i-1].x;
						EatenFoodBuffer->foodbuffer[i].y        = EatenFoodBuffer->foodbuffer[i-1].y;
					}
				EatenFoodBuffer->EatenFoodBufferIndex  = EatenFoodBuffer->EatenFoodBufferIndex - 1;
				EatenFoodBuffer->EatenFoodBufferLength = EatenFoodBuffer->EatenFoodBufferLength - 1;
				Snake->len = Snake->len - 1;
			}
		}
	}

	/*判斷吃蘋果*/
	if (foodarray[Snake->x_buf[0]][Snake->y_buf[0]] == OBJECT_GFOOD)
	{
		EatenFoodBuffer->EatenFoodBufferLength = EatenFoodBuffer->EatenFoodBufferLength + 1;
		if(EatenFoodBuffer->EatenFoodBufferIndex + 1 >= 0){
			EatenFoodBuffer->EatenFoodBufferIndex = EatenFoodBuffer->EatenFoodBufferIndex + 1;
			for(i = 0  ; i + 1 < EatenFoodBuffer->EatenFoodBufferLength ; i++){
				EatenFoodBuffer->foodbuffer[i+1].foodtype = EatenFoodBuffer->foodbuffer[i].foodtype;
				EatenFoodBuffer->foodbuffer[i+1].x        = EatenFoodBuffer->foodbuffer[i].x;
				EatenFoodBuffer->foodbuffer[i+1].y        = EatenFoodBuffer->foodbuffer[i].y;
			}
			EatenFoodBuffer->foodbuffer[0].foodtype = 1;//1 :good type 0:bad type
			EatenFoodBuffer->foodbuffer[0].x = Snake->x_buf[0];
			EatenFoodBuffer->foodbuffer[0].y = Snake->y_buf[0];
		}
		food_count->gfood_count = food_count->gfood_count - 1;
		Snake->score = Snake->score + 2;
		foodarray[Snake->x_buf[0]][Snake->y_buf[0]] = OBJECT_EMPTY;
		screen[Snake->x_buf[0]][Snake->y_buf[0]] = OBJECT_SNAKE;
	}
	if (bfoodarray[Snake->x_buf[0]][Snake->y_buf[0]] == OBJECT_BFOOD)
	{
		EatenFoodBuffer->EatenFoodBufferLength = EatenFoodBuffer->EatenFoodBufferLength + 1;
		if(EatenFoodBuffer->EatenFoodBufferIndex + 1 >= 0){
			EatenFoodBuffer->EatenFoodBufferIndex = EatenFoodBuffer->EatenFoodBufferIndex + 1;
			for(i = 0  ; i + 1 < EatenFoodBuffer->EatenFoodBufferLength ; i++){
				EatenFoodBuffer->foodbuffer[i+1].foodtype = EatenFoodBuffer->foodbuffer[i].foodtype;
				EatenFoodBuffer->foodbuffer[i+1].x        = EatenFoodBuffer->foodbuffer[i].x;
				EatenFoodBuffer->foodbuffer[i+1].y        = EatenFoodBuffer->foodbuffer[i].y;
			}
			EatenFoodBuffer->foodbuffer[0].foodtype = 0;//1 :good type 0:bad type
			EatenFoodBuffer->foodbuffer[0].x = Snake->x_buf[0];
			EatenFoodBuffer->foodbuffer[0].y = Snake->y_buf[0];
		}
		food_count->bfood_count = food_count->bfood_count - 1;
		Snake->score = Snake->score - 1;
		bfoodarray[Snake->x_buf[0]][Snake->y_buf[0]] = OBJECT_EMPTY;
		screen[Snake->x_buf[0]][Snake->y_buf[0]] = OBJECT_SNAKE;
	}
}

void MoveUp(snake *Snake){
	u8 i;
	for (i = 1; i < Snake->len; i++){
		//頭撞到身體（頭的x = 身體的x &&頭的y = 身體的y）
		if ( (Snake->x_buf[0] == Snake->x_buf[i] ) && (Snake->y_buf[0] == Snake->y_buf[i])){
			Snake->life = Snake->life - 1;
			break;
		}
	}
	for(i = Snake->len -1 ; i >= 1 ; i--){
		Snake->x_buf[i] = Snake->x_buf[i-1];
		Snake->y_buf[i] = Snake->y_buf[i-1];
	}
	Snake->y_buf[0] = Snake->y_buf[0] - 1 ;
}

void MoveDown(snake *Snake){
	u8 i;
	for(i = 1; i < Snake->len; i++){
		if ( (Snake->x_buf[0] == Snake->x_buf[i] ) && (Snake->y_buf[0] == Snake->y_buf[i])){
			Snake->life = Snake->life - 1;
			break;
		}
	}
	for(i = Snake->len -1 ; i >= 1 ; i--){
		Snake->x_buf[i] = Snake->x_buf[i-1];
		Snake->y_buf[i] = Snake->y_buf[i-1];
	}
	Snake->y_buf[0] = Snake->y_buf[0] + 1 ;
}

void MoveLeft(snake *Snake){
	u8 i;
	for(i = 1; i < Snake->len; i++){
		if ( (Snake->x_buf[0] == Snake->x_buf[i] ) && (Snake->y_buf[0] == Snake->y_buf[i])){
			Snake->life = Snake->life - 1;
			break;
		}
	}
	for(i = Snake->len -1 ; i >= 1 ; i--){
		Snake->x_buf[i] = Snake->x_buf[i-1];
		Snake->y_buf[i] = Snake->y_buf[i-1];
	}
	Snake->x_buf[0] = Snake->x_buf[0] - 1 ;
}

void MoveRight(snake *Snake){
	u8 i;
	for(i = 1; i < Snake->len; i++){
		if ( (Snake->x_buf[0] == Snake->x_buf[i] ) && (Snake->y_buf[0] == Snake->y_buf[i])){
			Snake->life = Snake->life - 1;
			break;
		}
	}
	for(i = Snake->len -1 ; i >= 1 ; i--){
		Snake->x_buf[i] = Snake->x_buf[i-1];
		Snake->y_buf[i] = Snake->y_buf[i-1];
	}
	Snake->x_buf[0] = Snake->x_buf[0] + 1 ;
}

void Update(snake *Snake,int** foodarray,int** bfoodarray,int** screen,eatenbuffer *EatenfoodBuffer,foodcount* bfood_count){
	int i,j;
	switch (Snake->SnakeDirection){
	case DIRECTION_LEFT:
		MoveLeft(Snake);//往左
		break;
	case DIRECTION_RIGHT:
		MoveRight(Snake);//往右
		break;
	case DIRECTION_UP:
		MoveUp(Snake);//往上
		break;
	case DIRECTION_DOWN:
		MoveDown(Snake);//往下
		break;
	}
	CreateBorder(screen);

	for(i = 0 ; i < Snake->len ; i++){
		screen[Snake->x_buf[i]][Snake->y_buf[i]] = OBJECT_SNAKE;
	}
	iseatfood(Snake,foodarray,bfoodarray,screen,EatenfoodBuffer,bfood_count);
	for(j = 0 ; j < HEIGHT ; j++){
		for(i = 0 ; i < WIDTH ; i++){
			if(foodarray[i][j] == OBJECT_GFOOD){
				screen[i][j] = OBJECT_GFOOD;
			}
			if(bfoodarray[i][j] == OBJECT_BFOOD){
				screen[i][j] = OBJECT_BFOOD;
			}
		}
	}
}

/*繪製遊戲區域邊界*/ 	//0
void CreateBorder(int** screen){
	u8 x, y;
	for (y = 0; y < HEIGHT ; y++){
		for (x = 0; x < WIDTH ; x++){
			if(x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1){
				screen[x][y] = OBJECT_BORDER;
			}
			else{
				screen[x][y] = OBJECT_EMPTY;
			}
		}
	}
}

/*隨機創造一個好食物*/
void CreateGFood(snake *Snake,int** foodarray,int** bfoodarray,fruit *food,foodcount* bgfood_count){
	int flag = 0,i;
	do{
		food->x = (rand() % (WIDTH- 2)) + 1;
		food->y = (rand() % (HEIGHT - 2)) + 1;
		// 判斷生成的食物是否和蛇身重合
		for (i = 0; i < Snake->len; i++){
			if (Snake->x_buf[i] == food->x && Snake->y_buf[i] == food->y){
				flag = 1;
				break;
			}
			else{
				flag = 0;
			}
		}
		if(flag == 0){
			if(foodarray[food->x][food->y] == OBJECT_GFOOD || bfoodarray[food->x][food->y] == OBJECT_BFOOD){
				flag = 1;
			}
			else{
				flag = 0;
			}
		}
	} while (flag && bgfood_count->gfood_count<5);
	if(bgfood_count->gfood_count <5){
		bgfood_count->gfood_count = bgfood_count->gfood_count + 1;
		foodarray[food->x][food->y] = OBJECT_GFOOD;
	}

}

/*隨機創造一個壞食物*/
void CreateBFood(snake *Snake,int** foodarray,int** bfoodarray,fruit *bfood,foodcount *bgfood_count){
	int flag = 0,i;
	do{
		bfood->x = (rand() % (WIDTH -2)) + 1;
		bfood->y = (rand() % (HEIGHT - 2)) + 1;
		// 判斷生成的食物是否和蛇身重合
		for (i = 0; i < Snake->len; i++){
			if (Snake->x_buf[i] == bfood->x && Snake->y_buf[i] == bfood->y){
				flag = 1 ;
				break;
			}
			else{
				flag = 0 ;
			}
		}
		if(flag == 0){
			if(foodarray[bfood->x][bfood->y] == OBJECT_GFOOD || bfoodarray[bfood->x][bfood->y] == OBJECT_BFOOD){
				flag = 1;
			}
			else{
				flag = 0;
			}
		}
	} while (flag && bgfood_count->bfood_count <5);
	if(bgfood_count-> bfood_count <5){
		bgfood_count-> bfood_count = bgfood_count-> bfood_count + 1;
		bfoodarray[bfood->x][bfood->y] = OBJECT_BFOOD;
	}
}

/*初始化小蛇*/
void CreateSnake(int** screen,snake *Snake,eatenbuffer *EatenfoodBuffer){	//0
	int i;
	Snake->SnakeDirection = DIRECTION_RIGHT;
	Snake->life = 3;
	EatenfoodBuffer->EatenFoodBufferIndex  = -1;
	EatenfoodBuffer->EatenFoodBufferLength =  0;

	Snake->len = 5;
	Snake->x_buf[0] = 5; Snake->y_buf[0] = 1;
	Snake->x_buf[1] = 4; Snake->y_buf[1] = 1;
	Snake->x_buf[2] = 3; Snake->y_buf[2] = 1;
	Snake->x_buf[3] = 2; Snake->y_buf[3] = 1;
	Snake->x_buf[4] = 1; Snake->y_buf[4] = 1;
	Snake->score = 5;


	for (i = 0; i < Snake->len ; i++){
		screen[Snake->x_buf[i]][Snake->y_buf[i]] = OBJECT_SNAKE;
	}
}

void TwoDArraytoOneDArray(int** screen,u8* TransmitBuffer,snake *Snake){
	u16 i,j;
	for(j = 0 ; j <  HEIGHT ; j++){
		for(i = 0 ; i < WIDTH ; i++){
			TransmitBuffer[((j<<4)&0xF8)+i] = screen[i][j];

		}
	}
	TransmitBuffer[257] = Snake->len << 3 | Snake->life ;
}
