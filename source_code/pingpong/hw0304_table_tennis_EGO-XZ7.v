`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    10:50:14 03/20/2022 
// Design Name: 
// Module Name:    hw0304_pingpong 
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
//BOARD --> xilinx EGO-XZ7
module hw0304_pingpong( LED,SEG7_SCORE,
                        B_L,B_R,CLK,RST );
    
    output [7:0] LED;
    output [6:0] SEG7_SCORE;
    input B_L,B_R,CLK,RST;
    
    wire Clk_d26;
    wire Stable_L,Stable_R;
    wire [3:0] Score_Left;
    wire [2:0] Fsm_state;
    /*
    division_clock(clk_d26,    high_clk,rst);
    position_control(led_state,    fsm_state,clk,rst);
    table_tennis_FSM (state,left_score,    ball_position,left_stable,right_stable,clk,rst);
    score_decoder(score_show,    score_L);
    buttom(click,    in,clk,rst);
    */
    //                                            /* output <--|--> input */ 
    division_clock      DIVCLK_26     (Clk_d26,                 CLK,RST);
    position_control    BALL_CONTROL  (LED,                     Fsm_state,Clk_d26,RST);
    table_tennis_FSM    STATE_CONTROL (Fsm_state,Score_Left,    LED,Stable_L,Stable_R,CLK,RST);
    score_decoder       SEG7_DECODER  (SEG7_SCORE,              Score_Left);
    buttom              SING_STABLE_L (Stable_L,                B_L,CLK,RST);
    buttom              SING_STABLE_R (Stable_R,                B_R,CLK,RST);
    
endmodule



module position_control(led_state,    fsm_state,clk,rst);
    output reg [7:0] led_state; //球的位置
    input [2:0] fsm_state; //狀態機狀態
    input clk,rst;
    
    reg shift_condition; //用於判斷是否為初次右移或是左移
    
    parameter waiting_serve       = 3'd0,
              move_right          = 3'd1,
              move_left           = 3'd2,
              judgment_left_loss  = 3'd3,
              judgment_right_loss = 3'd4;
    
    always@(posedge clk or posedge rst)begin
        if(rst)begin
            led_state <= 8'd0;
            shift_condition <= 0;
        end
        else begin
            case(fsm_state)
                waiting_serve       :begin
                    led_state <= led_state;              //清場狀態 或是 有一方勝利 或是 有一方漏接
                end
                move_right          :begin
                    if(shift_condition==0)begin          //第一次右移
                        led_state       <= 8'b1000_0000; //球由左方擊出
                        shift_condition <= 1;            //shift_condition設為"1"表示不是第一次右移
                    end
                    else begin
                        led_state <= led_state >> 1;     //邏輯右移
                        shift_condition <= 1;            //保持非第一次右移
                    end
                end
                move_left           :begin
                    if(shift_condition==0)begin          //第一次左移
                        led_state       <= 8'b0000_0001; //球由右方擊出
                        shift_condition <= 1;            //shift_condition設為"1"表示不是第一次左移
                    end
                    else begin
                        led_state <= led_state << 1;     //邏輯左移
                        shift_condition <= 1;            //保持非第一次左移
                    end
                end
                judgment_left_loss  :begin               //左方未擊中球故進入此狀態
                    led_state       <= 8'b0000_1111;     //故將LED燈號設為 8'b0000_1111
                    shift_condition <= 0;                //並將移位狀態設為初次移位
                end
                judgment_right_loss :begin               //右方未擊中球故進入此狀態
                    led_state       <= 8'b1111_0000;     //故將LED燈號設為 8'b1111_0000
                    shift_condition <= 0;                //並將移位狀態設為初次移位
                end
                default:begin
                    led_state <= 8'b1111_1111;           //若有異常狀態燈號全亮
                end
            endcase
        end
    end
endmodule


module table_tennis_FSM (state,left_score,    ball_position,left_stable,right_stable,clk,rst);//狀態機   
    output reg [2:0] state;
    output reg [3:0] left_score;
    input [7:0] ball_position;
    input left_stable,right_stable;
    input clk,rst;
    
    reg judgment_cindition;
    reg [3:0] right_score;
    reg [25:0] wait_counter;//球到位時在時間內等待球擊中 0~26
                           //模擬用 0~6 FPGA用0~24
    parameter Waiting_serve       = 3'd0,
              Move_right          = 3'd1,
              Move_left           = 3'd2,
              Judgment_left_loss  = 3'd3,
              Judgment_right_loss = 3'd4,
			  Fail_condition      = 3'd5;
    
    always@(posedge clk or posedge rst)begin
        if(rst)begin
            state              <= Waiting_serve;
            left_score         <= 4'd0;
            right_score        <= 4'd0;
            wait_counter       <= 26'd0;
            judgment_cindition <= 0;
        end
        else begin
            case(state)
                Waiting_serve      :begin
					if(right_score==4'd7 || left_score==4'd7)begin
						state <= Waiting_serve;
					end
					else begin
						judgment_cindition <= 0;
							if(left_stable==1 && right_stable==0)begin//左方發球 
								state <= Move_right; //球開始右移
							end
							else if(left_stable==0 && right_stable==1)begin//右方發球
								state <= Move_left; //球開始左移
							end
							else begin//無人發球
								state <= Waiting_serve;
							end
					end

                end
                Move_right         :begin
                    if(ball_position==8'b0000_0001)begin//球到擊球點
                        if(wait_counter <26'd67108862)begin//FPGA用
                        //if(wait_counter <6)begin//模擬用
                            case(right_stable)
                                1'b0:begin//右尚未擊中
                                    wait_counter <= wait_counter+5'd1;
                                end
                                1'b1:begin//右擊中
                                    wait_counter <= 5'd0;
                                    state        <= Move_left;
                                end
                                default:begin
                                    wait_counter <= 5'd0;
                                    state        <= Waiting_serve;
                                end
                            endcase
                        end
                        else begin
                            wait_counter <= 5'd0;
                            state        <= Judgment_right_loss;
                        end
                    end
                    else if(ball_position > 8'b0000_0001 || ball_position == 8'd0 || ball_position == 8'hf0 || ball_position == 8'h0f)begin
                        state <= Move_right;//球未到擊球點
                    end
					else begin
						state <= Fail_condition;
					end
                end
                Move_left          :begin
                    if(ball_position==8'b1000_0000)begin//球到擊球點
                        if(wait_counter <26'd67108862)begin//FPGA用
                        //if(wait_counter <6)begin//模擬用
                            case(left_stable)
                                1'b0:begin//左尚未擊中
                                    wait_counter <= wait_counter+5'd1;
                                end
                                1'b1:begin//左擊中
                                    wait_counter <= 5'd0;
                                    state        <= Move_right;
                                end
                                default:begin
                                    wait_counter <= 5'd0;
                                    state        <= Waiting_serve;
                                end
                            endcase
                        end
                        else begin
                            wait_counter <= 5'd0;
                            state        <= Judgment_left_loss;
                        end
                    end
                    else if(ball_position <8'b1000_0000 || ball_position == 8'd0 || ball_position == 8'hf0 || ball_position == 8'h0f)begin
                        state <= Move_left;//球未到擊球點
                    end
					else begin
						state <= Fail_condition;
					end
                end
                Judgment_left_loss  :begin
                    case(judgment_cindition)
                        1'd0:begin
                            if(right_score<=6)begin
								right_score        <= right_score+4'd1;
								judgment_cindition <= 1;
							end
							else begin
								right_score        <= 4'd7;
								judgment_cindition <= 1;
							end
                        end
                        1'd1:begin
							case(right_stable||left_stable)
								1'd0:begin
									judgment_cindition <= 1;
								end
								1'd1:begin
									state <= Waiting_serve;
								end
								default:begin
									state <= Waiting_serve;
								end
							endcase
                        end
                        default:begin
							state <= 3'd5;
                        end
                    endcase
                end
                Judgment_right_loss :begin
                    case(judgment_cindition)
                        1'd0:begin
                            if(left_score<=6)begin
								left_score         <= left_score+4'd1;
								judgment_cindition <= 1;
							end
							else begin
								left_score         <= 4'd7;
								judgment_cindition <= 1;
							end
                        end
                        1'd1:begin
							case(right_stable||left_stable)
								1'd0:begin
									judgment_cindition <= 1;
								end
								1'd1:begin
									state <= Waiting_serve;
								end
								default:begin
									state <= Waiting_serve;
								end
							endcase
                        end
                        default:begin
							state <= 3'd5;
                        end
                    endcase
                end
                default:begin
                    state <= state;
                end
            endcase
        end
    end
endmodule


module score_decoder(score_show,    score_L);//選擇想看誰的分數

    output reg [6:0] score_show;//指控制七個分段，無小數點
    input [3:0] score_L;
    
    always @(*)begin
        case(score_L)
            4'd0:   begin score_show = 7'b0000001; end //0
            4'd1:   begin score_show = 7'b1001111; end //1
            4'd2:   begin score_show = 7'b0010010; end //2
            4'd3:   begin score_show = 7'b0000110; end //3
            4'd4:   begin score_show = 7'b1001100; end //4
            4'd5:   begin score_show = 7'b0100100; end //5
            4'd6:   begin score_show = 7'b0100000; end //6
            4'd7:   begin score_show = 7'b0001111; end //7
            default:begin score_show = 7'b0000001; end //0 ;
        endcase
    end    
endmodule

module buttom(click,    in,clk,rst);//消除彈跳 *延遲一個CLK輸出
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

module division_clock(clk_d26,    high_clk,rst);
    output clk_d26;       //除頻完的頻率
    input  high_clk, rst; //clk為原始時脈(100MHz)
    
    reg [27:0]tmp;
    
    //assign clk_d26 = tmp[2]; //模擬用
    assign clk_d26 = tmp[25];  //FPGA用
    
    always@(posedge high_clk or posedge rst)begin
        if(rst)begin
            tmp <= 28'd0;
        end
        else begin
            tmp <= tmp + 28'd1;
        end
    end
endmodule
    
