module shift_left_2_tb ();

	wire [31:0] shifted_address;
	reg [31:0] address;
	reg CLK;
	
	shift_left_2 sl(shifted_address, address);
	
	
	initial begin
	 //CLK = 1'b0;
	 address = 32'b11100000000000000000000000001111;
	 #20 $display("address: %32b , in shifted: %32b",address,shifted_address);
		
	 address = 32'b01110000000000001000000000001111;
	 #20 $display("address: %32b , in shifted: %32b",address,shifted_address);
	 
	end
	
	always
		begin
		#1
		CLK = ~CLK;
		end
		
endmodule

	