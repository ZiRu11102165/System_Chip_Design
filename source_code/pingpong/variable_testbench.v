`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   22:14:59 04/06/2022
// Design Name:   hw0304_pingpong
// Module Name:   D:/ISE_program/table_tennis/testbench_2.v
// Project Name:  table_tennis
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: hw0304_pingpong
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module  hw0325_variable_speed_tb;

    // Inputs
    reg B_L;
    reg B_R;
    reg CLK;
    reg RST;

    // Outputs
    wire [7:0] LED;
    wire [7:0] SEG7_SCORE;
    wire [7:0] WHICH_LIGHT;

    // Instantiate the Unit Under Test (UUT)
    variable_speed_main uut (
        .LED(LED), 
        .SEG7_SCORE(SEG7_SCORE), 
        .WHICH_LIGHT(WHICH_LIGHT), 
        .B_L(B_L), 
        .B_R(B_R), 
        .CLK(CLK), 
        .RST(RST)
    );
	
	reg [10:0]step_condition;
	reg times_back;
	parameter   state_0  = 11'd0,
				state_1  = 11'd1,
				state_2  = 11'd2,
				state_3  = 11'd3,
				state_4  = 11'd4,
				state_5  = 11'd5,
				state_6  = 11'd6,
				state_7  = 11'd7,
				state_8  = 11'd8,
				state_9  = 11'd9,
				state_10 = 11'd10,
				state_11 = 11'd11;
	
	
	initial CLK = 0;
    always #10 CLK = ~CLK;	
	
    initial begin
        // Initialize Inputs
        B_L = 0;
        B_R = 0;
        RST = 0;
		// testbench register initial condition
		step_condition = 0;
		times_back     = 0;

		#40		RST = 1;
		#2000
		#40 	RST = 0;
		#40     step_condition = 1;
		
		
		#10000000
		$display("************************************************************"); 
		$display($time, ,"Finish testing with no error");
		$display("************************************************************"); 
		$finish;
		
    end
	
	always@(posedge CLK)begin
		case(step_condition)
			state_0  : begin
				step_condition = step_condition ;
			end
			state_1  : begin
				#100 B_L = 1;
				#40  B_L = 0;
				step_condition = state_2;
			end
			state_2  : begin
				if(LED==8'b0000_0001)begin
					#320 B_R = 1;
					#40  B_R = 0;
					step_condition = state_3;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_3  : begin
				if(LED==8'b1000_0000)begin
					#320 B_L = 1;
					#40  B_L = 0;
					step_condition = state_4;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_4  : begin
				if(LED==8'b1111_0000)begin
					#200 B_R = 1;
					#40  B_R = 0;
					#200 B_L = 1;
					#40  B_L = 0;
					step_condition = state_5;				
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_5  : begin
				if(LED==8'b0001_0000)begin
					#320 B_R = 1;
					#40  B_R = 0;
					step_condition = state_6;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_6  : begin
				if(LED==8'b1111_0000)begin
					#320 B_L = 1;
					#40  B_L = 0;
					step_condition = state_7;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_7  : begin
				if(LED==8'b0000_0001)begin
					#320 B_R = 1;
					#40  B_R = 0;
					step_condition = state_8;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_8  : begin
				if(LED==8'b0000_1111)begin
					#320 B_L = 1;
					#40  B_L = 0;
					#320 B_R = 1;
					#40  B_R = 0;
					step_condition = state_9;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_9  : begin
				if(LED==8'b0001_0000)begin
					#320 B_L = 1;
					#40  B_L = 0;
					step_condition = state_10;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_10 : begin
				if(LED==8'b0000_1111)begin
					#800 B_R = 1;
					#40  B_R = 0;
					step_condition = state_11;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end
			state_11 : begin
				if(LED==8'b0001_0000)begin
					step_condition = state_10;
				end
				else begin
					B_L = 0; B_R = 0;
				end
			end	
			default :begin
				B_L = 0; B_R = 0;
			end
		endcase
	end
endmodule

