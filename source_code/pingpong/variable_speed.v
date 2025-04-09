`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:53:57 04/01/2022 
// Design Name: 
// Module Name:    variable_speed_main 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module variable_speed_main( LED,SEG7_SCORE,WHICH_LIGHT,
                            B_L,B_R,CLK,RST  );
    
	output [7:0] LED,SEG7_SCORE,WHICH_LIGHT;
	input B_L,B_R,CLK,RST;
	
	//FSM					(FSM_State,Left_Score,Right_Score,    LED_Position,Left_Button,Right_Button,First_serve,clk,rst);
	//LED					(LED_Position,First_serve,    Trigger,FSM_State,clk,rst);
	//Random				(Random_value,    clk,rst);//實現變速球的手段
	//LED_Trigger			(Trigger,    FSM_State,Random_value,clk,rst);
	//Seven_segment			(score_show,segment_select,    Left_Score,Right_Score,clk,rst);//選擇想看誰的分數
    //Button				(click,    in,clk,rst);//消除彈跳 *延遲一個CK輸出
    //Frequency_divsion		(clk_d19,clk_d23    high_clk,rst);

	wire Left_stable,Right_stable;
	wire Clk_d19,Clk_d23,Trigger,First_serve;
	wire [2:0] FSM_State;
	wire [3:0] Left_Score,Right_Score;	
	wire [5:0] Random_value;
		
	
	FSM					LED_CONTROL     (FSM_State,Left_Score,Right_Score,    LED,Left_stable,Right_stable,First_serve,CLK,RST);
	LED					LED_POSITION    (LED,First_serve,                     Trigger,FSM_State,Clk_d23,RST);
	Random				VARIABLE_SPEED  (Random_value,                        Clk_d19,RST);//實現變速球的手段
	LED_Trigger			POSITION_CHANGE (Trigger,                             FSM_State,Random_value,Clk_d23,RST);
	Seven_segment	    SCORE_DISPLAY   (SEG7_SCORE,WHICH_LIGHT,              Left_Score,Right_Score,Clk_d19,RST);//選擇想看誰的分數
    Button				LEFT_STABLE     (Left_stable,                         B_L,CLK,RST);//消除彈跳 *延遲一個CK輸出
	Button				RIGHT_STABLE    (Right_stable,                        B_R,CLK,RST);//消除彈跳 *延遲一個CK輸出
    Frequency_divsion	DIVCLK          (Clk_d19,Clk_d23,                     CLK,RST);

endmodule

//Control State
module FSM(FSM_State,Left_Score,Right_Score,    LED_Position,Left_Button,Right_Button,First_serve,clk,rst);
    output reg [2:0]FSM_State;
    output reg[3:0]Left_Score,Right_Score;
    input [7:0]LED_Position;
    input Left_Button,Right_Button;
    input First_serve;                        //紀錄LED_position是不是第一次發球(8'b0000_0000)
    input clk,rst;
    
    parameter Waiting_serve       = 3'd0,
              Move_right          = 3'd1,
              Move_left           = 3'd2,
              Judgment_left_loss  = 3'd3,
              Judgment_right_loss = 3'd4,
              Fail_condition      = 3'd5;
    
    always@(posedge clk or posedge rst)begin
        if(rst)begin
            Left_Score <= 4'd0;
            Right_Score<= 4'd0;
            FSM_State <=Waiting_serve;
        end        
        else begin
            case(FSM_State)
                Waiting_serve:begin
                    //偵測左 or 右發球
                    if(Left_Score ==4'd7 || Right_Score == 4'd7)begin                //有人分數如果是7分，比賽結束
                        FSM_State <= Waiting_serve;
                    end
                    else begin
                        case(LED_Position)
                            8'b0000_0000:begin                                        //第一次發球
                                if(Left_Button == 1 && Right_Button == 0)begin
                                    FSM_State <= Move_right;
                                end
                                else if(Left_Button == 0 && Right_Button == 1)begin
                                    FSM_State <= Move_left;
                                end
                                else begin
                                    FSM_State <= Waiting_serve;
                                end
                            end
                            8'b1111_0000:begin                                        //左邊贏 限制左發球
                                if(Left_Button == 1 && Right_Button == 0)begin
                                    FSM_State <= Move_right;
                                end
                                else begin
                                    FSM_State <= Waiting_serve;
                                end
                            end
                            8'b0000_1111:begin                                        //右邊贏 限制右發球
                                if(Left_Button == 0 && Right_Button == 1)begin
                                    FSM_State <= Move_left;
                                end
                                else begin
                                    FSM_State <= Waiting_serve;
                                end
                            end
                            default:begin
                                FSM_State <= Fail_condition;
                            end
                        endcase
                    end
                end
                Move_right:begin
                    if((LED_Position != 8'b0000_0001 && Right_Button == 1) || (LED_Position == 8'd0 && First_serve == 1))begin//right player hit ball too early or late
                        FSM_State <= Judgment_right_loss;
                    end
                    else if(LED_Position == 8'b0000_0001 && Right_Button == 1)begin                        //right player hit the ball at right time
                        FSM_State <= Move_left;
                    end
                    else begin
                        FSM_State <= Move_right;
                    end
                end
                Move_left:begin
                    if((LED_Position != 8'b1000_0000 && Left_Button == 1) || (LED_Position == 8'd0 && First_serve == 1))begin//right player hit ball too early or late
                        FSM_State <= Judgment_left_loss;
                    end
                    else if(LED_Position == 8'b1000_0000 && Left_Button == 1)begin                        //right player hit the ball at right time
                        FSM_State <= Move_right;
                    end
                    else begin
                        FSM_State <= Move_left;
                    end
                end
                Judgment_left_loss:begin
                    if(LED_Position == 8'b0000_1111)begin
                        FSM_State <= Waiting_serve;
                        Right_Score <= Right_Score +4'd1;
                    end
                    else begin
                        FSM_State <= Judgment_left_loss;
                    end
                end
                Judgment_right_loss:begin
                    if(LED_Position == 8'b1111_0000)begin
                        FSM_State <= Waiting_serve;
                        Left_Score <= Left_Score +4'd1;
                    end
                    else begin
                        FSM_State <= Judgment_right_loss;
                    end
                end
                default:begin
                    FSM_State <= Fail_condition;
                end
            endcase
        end
    end
endmodule

module LED(LED_Position,First_serve,    Trigger,FSM_State,clk,rst);
    output reg [7:0] LED_Position;
    output reg First_serve;
    input [2:0]FSM_State;
	input Trigger;
    input clk,rst;
    
    reg shift_condition; //用於判斷是否為初次右移或是左移
    
    parameter Waiting_serve       = 3'd0,
              Move_right          = 3'd1,
              Move_left           = 3'd2,
              Judgment_left_loss  = 3'd3,
              Judgment_right_loss = 3'd4,
              Fail_condition      = 3'd5;
    
    always@(posedge clk or posedge rst)begin
        if(rst)begin
            LED_Position    <= 8'b0000_0000;
            shift_condition <= 0;
            First_serve     <= 0;
        end
        else begin
            case(FSM_State)
                Waiting_serve:begin
                    LED_Position <= LED_Position;
                    First_serve  <= First_serve;
                end
                
                Move_right:begin
                    if(shift_condition == 0)begin          //第一次右移
                        LED_Position    <= 8'b1000_0000; //球由左方擊出
                        shift_condition <= 1;            //shift_condition設為"1"表示不是第一次右移
                    end
                    else begin
						if(Trigger == 1)begin
							First_serve <= 1;
							LED_Position <= LED_Position >> 1;     //邏輯右移
							shift_condition <= 1;            //保持非第一次右移
						end
						else begin
							First_serve  <= 1;
							LED_Position <= LED_Position;
							shift_condition <= 1; 
						end
                    end
                end
                Move_left:begin
                    if(shift_condition == 0)begin          //第一次左移
                        LED_Position    <= 8'b0000_0001; //球由右方擊出
                        shift_condition <= 1;            //shift_condition設為"1"表示不是第一次左移
                    end
                    else begin
						if(Trigger == 1)begin
							First_serve     <= 1;
							LED_Position    <= LED_Position << 1;     //邏輯左移
							shift_condition <= 1;            //保持非第一次左移
						end	
						else begin
							First_serve     <= 1;
							LED_Position    <= LED_Position ;
							shift_condition <= 1;            
						end
                    end
                end
                Judgment_left_loss  :begin               //左方為擊中球故進入此狀態
                    LED_Position       <= 8'b0000_1111;     //故將LED燈號設為 8'b0000_1111
                    shift_condition <= 0;                //並將移位狀態設為初次移位
                end
                Judgment_right_loss :begin               //右方為擊中球故進入此狀態
                    LED_Position       <= 8'b1111_0000;     //故將LED燈號設為 8'b1111_0000
                    shift_condition <= 0;                //並將移位狀態設為初次移位
                end
                default:begin
                    LED_Position <= 8'b1111_1111;           //若有異常狀態燈號全亮
                end
            endcase
        end
    end
endmodule


// 亂數的數值為接受電路要計數的數值
module Random(Random_value,    clk,rst);//實現變速球的手段
    output [5:0] Random_value; // 要數的次數
    input clk,rst;//clk_d19
    
    reg [3:0] random_speed;
    reg [3:0] seedcnt;//4 bit
    
    parameter Move_right          = 3'd1,
              Move_left           = 3'd2;
    
    /* 1-->d24[2] 0,2-->d27[16]  3-->d29[64] */    
    assign Random_value = (random_speed[1:0] == 2'd0) ? 6'd7  :
                          (random_speed[1:0] == 2'd1) ? 6'd3  :
                          (random_speed[1:0] == 2'd2) ? 6'd7  :
                          (random_speed[1:0] == 2'd3) ? 6'd31 : 6'd7;
    
    always@(posedge clk or posedge rst)begin
        if(rst)begin
            seedcnt<=4'd0;
        end
        else begin
            seedcnt<=seedcnt+4'd1;
        end
    end
    
    always@(posedge clk or posedge rst )begin
        if(rst)begin
            random_speed<=4'd0;
        end
        else begin
            if(random_speed==4'd0)begin
                random_speed<=seedcnt;
            end    
            else begin
                random_speed[0]<=random_speed[3]^random_speed[2];
                random_speed[1]<=random_speed[2]^random_speed[1];
                random_speed[2]<=random_speed[1]^random_speed[0];
                random_speed[3]<=random_speed[0]^random_speed[3];
            end
        end
    end
endmodule

//等等改LED module
/*
24 ==> 2
27 ==> 16
29 ==> 64
*/
module LED_Trigger(Trigger,    FSM_State,Random_value,clk,rst);
    output reg Trigger; //數完了沒? 0:沒啦 1:好了啦
    input [2:0] FSM_State;
    input [5:0] Random_value;
    input clk,rst;
    
    reg [5:0] trigger_number;
    reg [5:0] trigger_counter;
    reg [1:0] trigger_state; //實現產生觸發的狀態機狀態
    
    parameter Waiting_serve       = 3'd0,
              Move_right          = 3'd1,
              Move_left           = 3'd2,
              Judgment_left_loss  = 3'd3,
              Judgment_right_loss = 3'd4,
              Fail_condition      = 3'd5;
    
    parameter   Get_Random_value  = 2'd0,
                Counting          = 2'd1;
    
    always@(posedge clk or posedge rst)begin
        if(rst)begin
            Trigger         <= 0;
            trigger_counter <= 6'd0;
            trigger_number  <= 6'd0;
            trigger_state   <= Get_Random_value;
        end
        else begin
            case(FSM_State)
                Waiting_serve        :begin
                    Trigger <= 0;
                end
                Move_right,Move_left :begin
                    case(trigger_state)
                        Get_Random_value  :begin
							Trigger <= 0;
                            trigger_number <= Random_value;
                            trigger_state  <= Counting;
                        end
                        Counting          :begin
                            if(trigger_counter == trigger_number)begin
                                Trigger         <= 1;
                                trigger_counter <= 6'd0;
								trigger_state   <= Get_Random_value;
                            end
                            else begin
                                trigger_counter <= trigger_counter+6'd1;
                            end
                        end
                        default:begin
                            Trigger <= 0;
                            trigger_state   <= Get_Random_value;
                        end
                    endcase
                end
                default:begin
                    Trigger         <= 0;
                    trigger_counter <= 6'd0;
                    trigger_number  <= 6'd0;
                    trigger_state   <= Get_Random_value;//提早打
                end
            endcase
        end
    end 
endmodule



module Seven_segment(score_show,segment_select,    Left_Score,Right_Score,clk,rst);//選擇想看誰的分數
    output reg [7:0] score_show;//指控制七個分段，無小數點
    output reg [7:0]segment_select;
    input [3:0] Left_Score,Right_Score;
    input clk,rst;
    
    reg random_speed;
    reg [3:0]score_scanned;
    
    always@(posedge clk or posedge rst)begin
        if(rst)begin
            segment_select <= 8'b0111_1111;
            random_speed <= 0;
            score_scanned  <= 4'd0;
        end
        else begin
            case(random_speed)
                1'b0:begin
                    segment_select <= 8'b0111_1111;
                    score_scanned  <= Left_Score;
                    random_speed <= 1;
                end
                1'b1:begin
                    segment_select <= 8'b1110_1111;
                    score_scanned  <= Right_Score;
                    random_speed <= 0;
                end
                default:begin
                    segment_select <= 8'b1111_0000;
                    score_scanned  <= 4'd7;
                end
            endcase
        end
    end

    always @(*)begin
        case(score_scanned)
            4'd0:   begin score_show = 8'b00000011; end //0
            4'd1:   begin score_show = 8'b10011111; end //1
            4'd2:   begin score_show = 8'b00100101; end //2
            4'd3:   begin score_show = 8'b00001101; end //3
            4'd4:   begin score_show = 8'b10011001; end //4
            4'd5:   begin score_show = 8'b01001001; end //5
            4'd6:   begin score_show = 8'b01000001; end //6
            4'd7:   begin score_show = 8'b00011111; end //7
            default:begin score_show = 8'b00000011; end //0 ;
        endcase
    end
endmodule

module Button(click,    in,clk,rst);//消除彈跳 *延遲一個CK輸出
    output reg click;
    input in,clk,rst; //clk為原始時脈(100MHz)

    reg decnt;
    parameter bound = 1'd1;
    
    always @ (posedge clk or posedge rst)begin
        if(rst)begin
            decnt <= 0;
            click <= 0;
        end
        else begin
            if(in)begin
                if(decnt < bound)begin
                    decnt <= decnt + 1; // decnt 0 --> 1
                    click <= 0;
                end
                else begin
                    click <= 1 ;
                end
            end
            else begin
                decnt <= 0;
                click <= 0;
            end
        end
    end
endmodule

module Frequency_divsion(clk_d19,clk_d23,    high_clk,rst);
    output clk_d19,clk_d23;//除頻完的頻率
    input  high_clk, rst; //clk為原始時脈(100MHz)
    
    reg [27:0]tmp;
    
    // assign clk_d23 = tmp[4]; //模擬用
    // assign clk_d19 = tmp[1]; //模擬用
    assign clk_d23 = tmp[21];  //FPGA用
    assign clk_d19 = tmp[19];  //FPGA用
    
    
    always@(posedge high_clk or posedge rst)begin
        if(rst)begin
            tmp <= 28'd0;
        end
        else begin
            tmp <= tmp + 28'd1;
        end
    end
endmodule


