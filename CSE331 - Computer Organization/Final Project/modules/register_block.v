module register_block (
	output [31:0] read_data1,
	output [31:0] read_data2,
	input byteOperations,
	input [31:0] write_data,
	input [4:0] read_reg1,
	input [4:0] read_reg2, 
	input [4:0] write_reg, 
	input regWrite
);


	reg [31:0] registers [31:0];
	
	initial begin
	
		$readmemb("registers.mem", registers);
	
	end
	
	or_32bit or1(read_data1, registers[read_reg1], 32'b0); // structural
	or_32bit or2(read_data2, registers[read_reg2], 32'b0); // structural
	
	always @(*) begin  
		
		if(regWrite) begin
			// first register is 0
			if (write_reg != 0) begin
				registers[write_reg] = write_data;
				$writememb("registers.mem", registers);
			end  
		end
		
	end

	//assign read_data1 = (read_reg1 != 0) ? registers[read_reg1] : 0;
	//assign read_data2 = (read_reg2 != 0) ? registers[read_reg2] : 0;
	
	

	
	
endmodule
