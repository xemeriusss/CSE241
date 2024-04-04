module memory_block (
	output reg [31:0] read_data,
	input byteOperations, //(This is an optional signal for lb and sb operations)
	input [17:0] address,
	input [31:0] write_data,
	input memRead,
	input memWrite
);

	reg [31:0] mem [31:0];
	
	reg [1:0] byte_num;
	
	

	always @ (*) begin
	
		$readmemb("memory.mem", mem);
	
		if(memWrite) begin
			if (address != 0) begin
				//$display("swwwwwww");
				mem[address>>2] = write_data; // write to mem, for sw
				#1
				$writememb("memory.mem", mem);
			end
		end
	
		if(memRead) begin
			
			byte_num <= address[1:0]; // which byte will it read
			//$display("byte num out: %b\n",byte_num);
			
			
			if(byteOperations) begin
				
				//$display("byte num in: %b\n",byte_num);
				//$display("lbbbbbb");
								
				
				if(byte_num == 2'b00) begin
					read_data = {24'b0, mem[address>>2][7:0]};
				end
				
				if(byte_num == 2'b01) begin
					read_data = {24'b0, mem[address>>2][15:8]};
				end
				
				if(byte_num == 2'b10) begin
					read_data = {24'b0, mem[address>>2][23:16]};
				end
				
				if(byte_num == 2'b11) begin
					read_data = {24'b0, mem[address>>2][31:24]};
				end
				
			
			end  
			
			else begin
				//$display("lwwwwww");
				read_data = mem[address>>2]; // read from mem, for lw
			end
		end
		
	end

endmodule
