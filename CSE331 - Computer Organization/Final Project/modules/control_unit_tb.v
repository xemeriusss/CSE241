module control_unit_tb ();

	wire regDst;
	wire branch;
	wire memRead; // (This can also be used as memToReg)
	wire memWrite;
	wire [2:0] aluop; 
	wire alusrc;
	wire regWrite;
	
	wire jump;
	wire byteOperations; // (This is an optional signal for lb and sb operations)
	wire move; // (This is an optional signal for MOVE operation)
	
	reg [5:0] opcode;
	reg CLK;
	
	control_unit cu (
    .regDst   (regDst),
    .branch   (branch),
    .memRead  (memRead),
    .memWrite (memWrite),
    .aluop    (aluop),
    .alusrc   (alusrc),
    .regWrite (regWrite),
    .opcode    (opcode),
	 .jump (jump),
	 .byteOperations (byteOperations),
	 .move (move)
  );
	
	
	initial begin
	 CLK = 1'b0;
	 //#20 $display("memread: %b , memwrite: %b",memRead, memWrite);
	 
	 // sw
	 #10 opcode = 6'b010000; 
	 #100 $display("regDst: %b, branch: %b, memRead: %b, memWrite: %b, aluop: %b, alusrc: %b, regWrite: %b, jump: %b, byteOperations: %b, move: %b",
             regDst, branch, memRead, memWrite, aluop, alusrc, regWrite, jump, byteOperations, move);		
	 // lw
	 #10 opcode = 6'b001000; 
	 #100 $display("regDst: %b, branch: %b, memRead: %b, memWrite: %b, aluop: %b, alusrc: %b, regWrite: %b, jump: %b, byteOperations: %b, move: %b",
             regDst, branch, memRead, memWrite, aluop, alusrc, regWrite, jump, byteOperations, move);	 
	 // rtype
	 #10 opcode = 6'b000000;
	 #100 $display("regDst: %b, branch: %b, memRead: %b, memWrite: %b, aluop: %b, alusrc: %b, regWrite: %b, jump: %b, byteOperations: %b, move: %b",
             regDst, branch, memRead, memWrite, aluop, alusrc, regWrite, jump, byteOperations, move);
				
	 // addi	
	 #10 opcode = 6'b000010;
	 #100 $display("regDst: %b, branch: %b, memRead: %b, memWrite: %b, aluop: %b, alusrc: %b, regWrite: %b, jump: %b, byteOperations: %b, move: %b",
             regDst, branch, memRead, memWrite, aluop, alusrc, regWrite, jump, byteOperations, move);
		
	 
	 
	end
	
	always
		begin
		#1
		CLK = ~CLK;
		end
		
endmodule

	