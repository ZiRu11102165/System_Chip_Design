
#include <stdio.h>
#include <stdlib.h>
#include "platform.h"

#include "xparameters.h"
#include "xgpio.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xuartps.h"
#include "sleep.h"
#include "xscutimer.h"
#include "xstatus.h"

#include "snake.h"
// Parameter definitions


#define NUM_OF_SENDBYTE (WIDTH * HEIGHT)


#define INTC_DEVICE_ID 			XPAR_PS7_SCUGIC_0_DEVICE_ID
#define BTNS_DEVICE_ID			XPAR_AXI_GPIO_1_DEVICE_ID
#define LEDS_DEVICE_ID			XPAR_AXI_GPIO_0_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID 	XPAR_FABRIC_AXI_GPIO_1_IP2INTC_IRPT_INTR
#define BTN_INT 				XGPIO_IR_CH1_MASK

#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR

#define TIMER_LOAD_VALUE 	0x683

#define GENERATE_FOOD_SPEED 100000
#define SNAKE_MOVE_SPEED 	200000
XUartPs_Config *Config_1;
XUartPs Uart_PS_1;

XScuTimer TimerInst;
XGpio LEDInst, BTNInst;
XScuGic INTCInst;



static int mask = 0;
static int led_data;
static int btn_value;

static int flag = 0;
static int mode = 0;
static int pwm_count =0;
static int count_number = 200;
static int period = 0;
static int pwm_led;

static int start = 0;
static int reload = 0;


volatile static int foodarray[WIDTH][HEIGHT]={0};		    /*宣告一個 WIDTH * HIGH 的空food陣列*/
volatile static int bfoodarray[WIDTH][HEIGHT]={0};		/*宣告一個 WIDTH * HIGH 的空bfood陣列*/

volatile static int screen[WIDTH][HEIGHT]={0};
volatile static u8 TransmitBuffer[WIDTH*HEIGHT + 1] = {0};
//static u32 StatusRegister;
volatile static int SendEnable = 0;
eatenbuffer EatenfoodBuffer;

snake 		Snake;
fruit 		food;
fruit 		bfood;
Direction 	SnakeDirection;
State 		GameState;
gameobject  Gameobject;
foodcount  bgfood_count;

volatile static int SnakeGameState;
volatile static u32 ByteSend = 0 , TotalByteSend = 0;

volatile static int *pointer_of_screen[WIDTH];
volatile static int *pointer_of_foodarray[WIDTH];
volatile static int *pointer_of_bfoodarray[WIDTH];
//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------


static void BTN_Intr_Handler(void *baseaddr_p);
static int InterruptSystemSetup(XScuGic *XScuGicInstancePtr);
static int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr);

static int Uart_Init(void);

//----------------------------------------------------
// INTERRUPT HANDLER FUNCTIONS
// - called by the timer, button interrupt, performs
// - LED flashing
//----------------------------------------------------

void BTN_Intr_Handler(void *InstancePtr) {
	// Disable GPIO interrupts
	XGpio_InterruptDisable(&BTNInst, BTN_INT);
	// Ignore additional button presses
	if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) != BTN_INT) {
		return;
	}

	btn_value = XGpio_DiscreteRead(&BTNInst, 1);


	if(btn_value == 0x04){
		if(Snake.SnakeDirection == DIRECTION_UP){
			Snake.SnakeDirection = DIRECTION_LEFT;
		}
		else if(Snake.SnakeDirection == DIRECTION_DOWN){
			Snake.SnakeDirection = DIRECTION_RIGHT;
		}
		else if(Snake.SnakeDirection == DIRECTION_LEFT){
			Snake.SnakeDirection = DIRECTION_DOWN;
		}
		else if(Snake.SnakeDirection == DIRECTION_RIGHT){
			Snake.SnakeDirection = DIRECTION_UP;
		}
		else{
			Snake.SnakeDirection = Snake.SnakeDirection;
		}

		//key = LEFT;
	}
	if(btn_value == 0x08){
		if(Snake.SnakeDirection == DIRECTION_UP){
			Snake.SnakeDirection = DIRECTION_RIGHT;
		}
		else if(Snake.SnakeDirection == DIRECTION_DOWN){
			Snake.SnakeDirection = DIRECTION_LEFT;
		}
		else if(Snake.SnakeDirection == DIRECTION_LEFT){
			Snake.SnakeDirection = DIRECTION_UP;
		}
		else if(Snake.SnakeDirection == DIRECTION_RIGHT){
			Snake.SnakeDirection = DIRECTION_DOWN;
		}
		else{
			Snake.SnakeDirection = Snake.SnakeDirection;
		}
		//key = RIGHT;
	}
	if(btn_value == 0x10){
		start = 1;
		//Snake.SnakeDirection = DIRECTION_UP;
		//key = UP;
	}
	if(btn_value == 0x02){
		reload = 1;
		//Snake.SnakeDirection = DIRECTION_DOWN;
		//key = DOWN;
	}
	usleep(100000);
	//led_data = (int)TotalByteSend;
	//XGpio_DiscreteWrite(&LEDInst, 1, 0xFF);
	(void) XGpio_InterruptClear(&BTNInst, BTN_INT);
	// Enable GPIO interrupts
	XGpio_InterruptEnable(&BTNInst, BTN_INT);
}

//----------------------------------------------------
// INITIAL SETUP FUNCTIONS
//----------------------------------------------------

int InterruptSystemSetup(XScuGic *XScuGicInstancePtr) {
	// Enable interrupt
	XGpio_InterruptEnable(&BTNInst, BTN_INT);
	XGpio_InterruptGlobalEnable(&BTNInst);

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler) XScuGic_InterruptHandler,
			XScuGicInstancePtr);
	Xil_ExceptionEnable();

	return XST_SUCCESS;

}

int IntcInitFunction(u16 DeviceId, XGpio *GpioInstancePtr) {
	XScuGic_Config *IntcConfig;
	int status;

	// Interrupt controller initialisation
	IntcConfig = XScuGic_LookupConfig(DeviceId);
	status = XScuGic_CfgInitialize(&INTCInst, IntcConfig,
			IntcConfig->CpuBaseAddress);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	// Call to interrupt setup
	status = InterruptSystemSetup(&INTCInst);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	// Connect GPIO interrupt to handler
	status = XScuGic_Connect(&INTCInst, INTC_GPIO_INTERRUPT_ID,
			(Xil_ExceptionHandler) BTN_Intr_Handler, (void *) GpioInstancePtr);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	// Enable GPIO interrupts interrupt
	XGpio_InterruptEnable(GpioInstancePtr, 1);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	// Enable GPIO and timer interrupts in the controller
	XScuGic_Enable(&INTCInst, INTC_GPIO_INTERRUPT_ID);

	return XST_SUCCESS;
}

void TimerIntrHandler(void *InstancePtr){

	XScuTimer *TimerInstancePtr = (XScuTimer *) InstancePtr;

	if(XScuTimer_IsExpired(TimerInstancePtr)){
		XScuTimer_ClearInterruptStatus(TimerInstancePtr);	//清除中斷


		/*
		 *	static int mode = 0;
		 *	static int pwm_count =0;
		 *  static int count_number = 0;
		 *	static int period = 0;
	     *	static int pwm_led;
		 */
		if(mode == 0){
			pwm_count = pwm_count + 1;
			if(pwm_count < count_number){
				pwm_led = 0;
			}
			else{
				pwm_led = 1;
			}
			if(pwm_count == 200){
				period = period + 1;		//暗週期
				count_number = count_number - 1;
				pwm_count = 0;
				if(period == 500){
					pwm_count = 0;
					mode = 1;
					period = 0;
					flag = 1;
				}
			}
		}
		else if(mode == 1){
			pwm_count = pwm_count + 1;
			if(pwm_count  < count_number){
				pwm_led = 0;
			}
			else{
				pwm_led = 1;
			}
			if(pwm_count == 200){
				period = period + 1;		//亮週期
				count_number = count_number + 1;
				pwm_count = 0;
				if(period == 500){
					pwm_count = 0;
					mode = 0;
					period = 0;
					flag = 2;
				}
			}
		}

		if(mask == 0){
			if(flag == 1 || flag ==2){
				if(flag == 2){
					CreateGFood(&Snake,pointer_of_foodarray,pointer_of_bfoodarray,&food,&bgfood_count);
					CreateBFood(&Snake,pointer_of_foodarray,pointer_of_bfoodarray,&bfood,&bgfood_count);
					flag = 0;
				}
				Update(&Snake,pointer_of_foodarray,pointer_of_bfoodarray,pointer_of_screen,&EatenfoodBuffer,&bgfood_count);
				TwoDArraytoOneDArray(pointer_of_screen,(u8*)&TransmitBuffer,&Snake);
				while(TotalByteSend < NUM_OF_SENDBYTE){
					ByteSend = XUartPs_Send(&Uart_PS_1, (u8*)&TransmitBuffer[TotalByteSend], 1);
					TotalByteSend += ByteSend;
					//StatusRegister = XUartPs_ReadReg(Uart_PS_1.Config.BaseAddress,XUARTPS_SR_OFFSET);
				}
				TotalByteSend = 0;
				flag = 0;
			}
		}
		led_data = Snake.len << 3 | pwm_led << 2 | Snake.life ;
		XGpio_DiscreteWrite(&LEDInst, 1, led_data);
	}
}

void PrivateTimer_Init(XScuTimer *TimerInstancePtr, u16 TimerDeviceId){
	XScuTimer_Config *ConfigPtr;
	//Initialize private timer
	ConfigPtr = XScuTimer_LookupConfig(TimerDeviceId);
	XScuTimer_CfgInitialize(TimerInstancePtr,ConfigPtr,ConfigPtr->BaseAddr);

	//Enabel auto-reload mode
	XScuTimer_EnableAutoReload(TimerInstancePtr);
	XScuTimer_LoadTimer(TimerInstancePtr,TIMER_LOAD_VALUE);
}
void PrivateTimer_Intr_Init(XScuTimer *TimerInstancePtr,XScuGic *IntcInstancePtr,u16 TimerIntrId){
	XScuGic_Config *IntcConfig;
	//Initial Gic
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	XScuGic_CfgInitialize(IntcInstancePtr,IntcConfig,IntcConfig->CpuBaseAddress);
	//interrupt exception handle
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
								(Xil_ExceptionHandler)XScuGic_InterruptHandler,
								IntcInstancePtr);
	Xil_ExceptionEnable();
	//setting timer Interrupt Hadler
	XScuGic_Connect(IntcInstancePtr,TimerIntrId,
					(Xil_ExceptionHandler)TimerIntrHandler,
					(void *)TimerInstancePtr);
	//Enable Gic
	XScuGic_Enable(IntcInstancePtr,TimerIntrId);
	//Enable timer Interrupt
	XScuTimer_EnableInterrupt(TimerInstancePtr);
}

int Uart_Init(void){
	int status = 0;
	//UART 1 initialization
	Config_1 = XUartPs_LookupConfig(XPAR_XUARTPS_0_DEVICE_ID);
	if (NULL == Config_1) {
		return XST_FAILURE;
	}

	status = XUartPs_CfgInitialize(&Uart_PS_1, Config_1, Config_1->BaseAddress);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	//XUARTPS_TXWM_OFFSET
	status = XUartPs_SetBaudRate(&Uart_PS_1,115200);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XUartPs_SetOperMode(&Uart_PS_1, XUARTPS_OPER_MODE_NORMAL);

	XUartPs_WriteReg(Uart_PS_1.Config.BaseAddress,XUARTPS_TXWM_OFFSET, 0x20U);

	return XST_SUCCESS;

}

int main()
{
    init_platform();
    int status,i,col,row;

	//----------------------------------------------------
	// INITIALIZE THE PERIPHERALS & SET DIRECTIONS OF GPIO
	//----------------------------------------------------
	// Initialise LEDs
	status = XGpio_Initialize(&LEDInst, LEDS_DEVICE_ID);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	// Initialise Push Buttons
	status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	// Set LEDs direction to outputs
	XGpio_SetDataDirection(&LEDInst, 1, 0x00);

	// Set all buttons direction to inputs
	XGpio_SetDataDirection(&BTNInst, 1, 0xFF);

	// Initialize interrupt controller
	status = IntcInitFunction(INTC_DEVICE_ID, &BTNInst);
	if (status != XST_SUCCESS)
		return XST_FAILURE;

	if( Uart_Init() == XST_FAILURE )
		return XST_FAILURE;

	PrivateTimer_Init(&TimerInst, TIMER_DEVICE_ID);
	PrivateTimer_Intr_Init(&TimerInst,&INTCInst,TIMER_IRPT_INTR);


	led_data = 0x00;
	srand(0);

	//pointer initial
	for(i = 0 ; i < WIDTH ; i++){
		pointer_of_screen[i] = screen[i];
		pointer_of_foodarray[i] = foodarray[i];
		pointer_of_bfoodarray[i] = bfoodarray[i];
	}

	CreateBorder(pointer_of_screen);
	CreateSnake(pointer_of_screen,&Snake,&EatenfoodBuffer);
	GameState = Continue;
	SnakeGameState = GameState;
	Snake.SnakeDirection = DIRECTION_RIGHT;
	bgfood_count.bfood_count = 0;
	bgfood_count.gfood_count = 0;
	while(1){
		while(start != 1);
		XScuTimer_Start(&TimerInst);
		while(start){

			SnakeGameState = check(&Snake,pointer_of_screen,pointer_of_foodarray,pointer_of_bfoodarray,SnakeGameState,&bgfood_count);
			if(SnakeGameState == Lose /*|| SnakeGameState == Win*/){
				start = 0;
				XScuTimer_Stop(&TimerInst);
				led_data = led_data & 0xFB;
				XGpio_DiscreteWrite(&LEDInst, 1, led_data);
				Update(&Snake,pointer_of_foodarray,pointer_of_bfoodarray,pointer_of_screen,&EatenfoodBuffer,&bgfood_count);
				TwoDArraytoOneDArray(pointer_of_screen,(u8*)&TransmitBuffer,&Snake);
			}
			if(SnakeGameState == Win){
				start = 0;
				mask = 1;
			}
		}
		while(reload != 1);
		if(reload == 1){
			CreateBorder(pointer_of_screen);
			CreateSnake(pointer_of_screen,&Snake,&EatenfoodBuffer);
			GameState = Continue;
			SnakeGameState = GameState;
			Snake.SnakeDirection = DIRECTION_RIGHT;
			reload = 0;
			start = 1;
			mask = 0;
		}
	}

    cleanup_platform();
    return 0;
}
