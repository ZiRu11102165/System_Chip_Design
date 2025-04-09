`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   22:10:04 04/09/2022
// Design Name:   brightness_test
// Module Name:   D:/ISE_program/brightness_test/breath_testbench_v2.v
// Project Name:  brightness_test
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: brightness_test
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module breath_testbench;

	// Inputs
	reg clk;
	reg rst;

	// Outputs
	wire led;
	wire check;

	// Instantiate the Unit Under Test (UUT)
	brightness_test uut (
		.led(led), 
		.check(check), 
		.clk(clk), 
		.rst(rst)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		rst = 0;

		// Wait 100 ns for global reset to finish
		#100 rst = 1 ;
		#500   rst = 0 ;
        
		// Add stimulus here

	end
	initial forever #10 clk = ~clk;
      
endmodule

