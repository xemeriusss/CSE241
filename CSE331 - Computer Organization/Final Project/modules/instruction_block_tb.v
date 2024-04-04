module instruction_block_tb ();

	wire [31:0] inst;
	reg [31:0] pc;
	reg CLK;
	
	instruction_block ib(inst, pc);
	
	
	initial begin
	 //CLK = 1'b0;
	 pc = 0;
	 #20 $display("address: %d , in memory: %32b",pc,inst);
		
	 pc = 4;
	 #20 $display("address: %d , in memory: %32b",pc,inst);
		
	 pc = 8;
	 #20 $display("address: %d , in memory: %32b",pc,inst);
	 
	end
	
	always
		begin
		#1
		CLK = ~CLK;
		end
		
endmodule

	