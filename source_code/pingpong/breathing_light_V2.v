`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/04/09 19:41:32
// Design Name: 
// Module Name: brightness_test
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////
module brightness_test(led,check,    clk,rst);
    output led,check;
    input clk,rst;
	
    /*
		division_clock(clk_d8,clk_d26,    high_clk,rst);
		state_control(led,period_counter,breath_state,    clk,rst); 
	*/
	
	wire clk_d8;
	
	division_clock DIVCLK   (clk_d8,check,    clk,rst);
	state_control  DUTY_FSM (led,              clk_d8,rst); 

endmodule

module state_control(led,   clk,rst);    
    output reg led;
    input clk,rst; //clk_d19
    
	parameter duty_100_percent = 8'd199,
	          duty_1_percent   = 8'd1,
			  fail_condition   = 8'd250;
	
    parameter dark_to_light = 1'd0,          
              light_to_dark = 1'd1;
		  
    reg breath_mode;
	reg [7:0] breath_state;
	reg [7:0] period_counter;
    
	always@(posedge clk or posedge rst)begin
		if(rst)begin
			led <= 0;
		end
		else begin
			if(period_counter <= breath_state)begin
				led <= 1;
			end
			else begin
				led <= 0;
			end
		end
	end
	
    always@(posedge clk or posedge rst)begin
        if(rst)begin
            breath_state   <= 0;
            period_counter <= 0;
            breath_mode    <= 0;
        end
        else begin
            if(period_counter < 8'd200)begin
                breath_state   <= breath_state;
                period_counter <= period_counter + 8'd1;
            end
            else begin
                period_counter <= 0;
                case(breath_mode)                    
                    dark_to_light :begin
                        if(breath_state != duty_100_percent)begin
                            breath_state <= breath_state + 8'd1;
                        end
                        else begin
                            breath_mode  <= light_to_dark;
                        end
                    end
                    light_to_dark :begin
                        if(breath_state != duty_1_percent)begin
                            breath_state <= breath_state - 8'd1;
                        end
                        else begin
                            breath_mode  <= dark_to_light;
                        end
                    end
                    default:begin
                        breath_state <= fail_condition;
                    end
                endcase
            end
        end
    end
endmodule

module division_clock(clk_d8,clk_d26,    high_clk,rst);
    output clk_d8,clk_d26;       //除頻完的頻率
    input  high_clk, rst; //clk為原始時脈(100MHz)
    
    reg [27:0]tmp;
    
//    assign clk_d8 = tmp[2]; //模擬用
//	assign clk_d26 = tmp[3];  //模擬用
    assign clk_d8 = tmp[8];  //FPGA用
	assign clk_d26 = tmp[26];  //FPGA用
    
    always@(posedge high_clk or posedge rst)begin
        if(rst)begin
            tmp <= 28'd0;
        end
        else begin
            tmp <= tmp + 28'd1;
        end
    end
endmodule
