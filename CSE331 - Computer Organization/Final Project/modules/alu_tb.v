module alu_tb();

	wire [31:0] result;
	wire zero_bit;
	reg [31:0] a;
	reg [31:0] b;
	reg [2:0] alu_ctr;
	
	reg CLK;
 
	alu my_alu (result, zero_bit, a, b, alu_ctr);
 
	initial begin
		/*
		// AND
		a = 40; b = 32;
		alu_ctr[2] = 0; 
		alu_ctr[1] = 0; 
		alu_ctr[0] = 0; 
		
		#15;
		$display("AND: Time=%0t a=%b, b=%b, result=%b, zerobit = %b", $time, a, b, result, zero_bit);
		
		
		
		// OR
		a = 14; b = 14;
		alu_ctr[2] = 0; 
		alu_ctr[1] = 0; 
		alu_ctr[0] = 1; 
		
		#15;
		$display("OR: Time=%0t a=%b, b=%b, result=%b, zerobit = %b", $time, a, b, result, zero_bit);
		
		
		
		// XOR
		a = 333; b = 45;
		alu_ctr[2] = 0; 
		alu_ctr[1] = 1; 
		alu_ctr[0] = 0; 
		
		#15;
		$display("XOR: Time=%0t a=%b, b=%b, result=%b", $time, a, b, result);
		
		
		// NOR
		a = 56; b = 54;
		alu_ctr[2] = 0; 
		alu_ctr[1] = 1; 
		alu_ctr[0] = 1; 
		
		#15;
		$display("NOR: Time=%0t a=%b, b=%b, result=%b", $time, a, b, result);

		
		// SLT
		a = 55; b = 4;
		alu_ctr[2] = 1; 
		alu_ctr[1] = 0; 
		alu_ctr[0] = 0; 
		
		#15
		$display("SLT: Time=%0t a=%d, b=%d, result=%d", $time, a, b, result);
		*/

		// ADD
		a = 45; b = 45;
		alu_ctr[2] = 1; 
		alu_ctr[1] = 0; 
		alu_ctr[0] = 1;  
		
		#15
		$display("AND: Time=%0t a=%b, b=%b, result=%b, zerobit = %b", $time, a, b, result, zero_bit);

		// SUB
		a = 30; b = 25;
		alu_ctr[2] = 1; 
		alu_ctr[1] = 1; 
		alu_ctr[0] = 0; 
		
		#15
		$display("AND: Time=%0t a=%b, b=%b, result=%b, zerobit = %b", $time, a, b, result, zero_bit);
		
		/*
		// MOD
		#20
		CLK = 1'b0; reset = 1'b1;
		a = 35; b = 7;
		alu_ctr[2] = 1; 
		alu_ctr[1] = 1; 
		alu_ctr[0] = 1; 
		
		#5
		reset = 1'b0;
		
		#80
		$display("MOD: Time=%0t a=%d, b=%d, result=%d", $time, a, b, result);
		*/
	end
	 

	always
		begin
			#1
			CLK = ~CLK;
		end
			
endmodule
