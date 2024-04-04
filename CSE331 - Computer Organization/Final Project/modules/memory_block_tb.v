module memory_block_tb ();

	wire [31:0] read_data;
	reg [17:0] address;
	reg [31:0] write_data;
	reg memRead;
	reg memWrite;
	reg byteOperations;
	reg CLK;

	memory_block mb (
    .read_data(read_data),
    .byteOperations(byteOperations),
    .address(address),
    .write_data(write_data),
    .memRead(memRead),
    .memWrite(memWrite)
  );
  

	
	
	initial begin
		//CLK = 0;
		//#10 memRead = 0;
		
		#10 byteOperations=1;
		#10 address = 18'b000000000000000111; 
		#10 memRead = 1; 
		//#10 memWrite = 0; 
		#10 write_data = 32'b00100000000000000000000000111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);
		
		
		#10 byteOperations=1;
		#10 address = 18'b000000000000001011; 
		#10 memRead = 1; 
		//#10 memWrite = 0; 
		#10 write_data = 32'b00100000000000000000000000111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);
		
		#10 memRead = 0;
		
		#10 byteOperations=0;
		#10 address = 18'b000000000000010000; 
		 
		#10 memWrite = 1; 
		#10 write_data = 32'b00100000000000000000000000111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);
	/*
		#10 memRead = 0;
		
		#10 byteOperations=1;
		#10 address = 18'b000000000000010000; 
		#10 memRead = 1; 
		#10 memWrite = 0; 
		#10 write_data = 32'b00100000000000000000000000111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);
		
		#10 memRead = 0;
		
		#10 byteOperations=1;
		#10 address = 18'b000000000000001111; 
		#10 memRead = 1; 
		#10 memWrite = 0; 
		#10 write_data = 32'b00100000000000000000000000111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);
		
		
		#10 memRead = 0;
		
		#10 byteOperations=0;
		#10 address = 18'b000000000000000100; 
		#10 memRead = 1; 
		#10 memWrite = 0; 
		#10 write_data = 32'b00111111111111111111111111111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);
		
		
		
		
		#10 memRead = 0;
		
		#10 byteOperations=0;
		#10 address = 18'b000000000000000100; 
		#10 memRead = 0; 
		#10 memWrite = 1; 
		#10 write_data = 32'b00111111111111111111111111111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);
		
		
		
		

		
		#50 byteOperations=0;
		#50 address = 18'b000000000000001100; 
		#50 memRead = 1;
		
		#10 memWrite = 0; 
		#10 write_data = 32'b00100000000000000000000000111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);
		
		#10 memRead = 0;
		
		#50 byteOperations=0;
		#50 address = 18'b000000000000000100; 
		#50 memRead = 1;
		
		#10 memWrite = 0; 
		#10 write_data = 32'b00100000000000000000000000111111; 
		
		#100 $display("address: %b,  memread: %b,  memwrite: %b,  read_data: %32b, bo=%b",address, memRead, memWrite,read_data, byteOperations);

		*/

    #10 $finish; // Finish simulation

  end
	
	always
		begin
		#1
		CLK = ~CLK;
		end
		
		
endmodule
