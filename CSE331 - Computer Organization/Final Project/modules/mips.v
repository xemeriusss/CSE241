module mips(
	input clock
);

	reg [31:0] pc;
	
	initial begin
		pc = 32'b00000000000000000000000000000000;
	end
	
	wire [31:0] inc_pc;
	
	wire regWrite, memRead, memWrite, regDst, branch, jump, alusrc, byteOperations, move;
	wire [2:0] aluop;
	
	
	
	wire [31:0] instruction;
	
	wire [4:0] write_reg, read_reg1, read_reg2;
	
	wire [31:0] read_data1, read_data2;
	
	wire [31:0] read_data_mem;
	
	
	wire [31:0] write_data_reg;
	wire [31:0] write_data_mem;
	
	wire [31:0] alu_result, alu_a, alu_b, alu_res_for_move;
	wire zero_bit;
	
	
	wire [31:0] sign_ext_imm;
	
	wire [2:0] alu_ctr;
	wire [5:0] function_code;
	
	wire [5:0] opcode;
	wire [5:0] func;
	wire [3:0] shamt;
	
	wire [31:0] next_pc;
	
	wire [15:0] immediate;
	
	wire [31:0] branch_addr;
	wire [31:0] calculated_pc;
	wire [31:0] shifted_address;
	wire is_branch;
	
	wire beq_signal, bne_signal;
	
	wire [25:0] jump_addr;
	
	wire [31:0] new_jump_addr;
	
	
	instruction_block ib(instruction, pc);
	
	assign opcode = instruction[31:26];
	
	assign jump_addr = {22'b0, instruction[9:0]};
	
	initial begin
		#10
		$display("jump_addr: %b\n",jump_addr);
	end
	
	
	
	
	
	
	// beq opcode: 100011
	and_1bit and51(beq_out1, opcode[0], opcode[1]);
	and_1bit and52(beq_out2, beq_out1, ~opcode[2]);
	and_1bit and53(beq_out3, beq_out2, ~opcode[3]);
	and_1bit and54(beq_out4, beq_out3, ~opcode[4]);
	and_1bit and55(beq_signal, beq_out4, opcode[5]);
	
	
	// bne opcode:100111
	and_1bit and56(bne_out1, opcode[0], opcode[1]);
	and_1bit and57(bne_out2, bne_out1, opcode[2]);
	and_1bit and58(bne_out3, bne_out2, ~opcode[3]);
	and_1bit and59(bne_out4, bne_out3, ~opcode[4]);
	and_1bit and60(bne_signal, bne_out4, opcode[5]);
	
	
	
	
	
	// make signals according to opcode
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
  
  
   // destination reg: rd or rt?
	assign write_reg = regDst ? instruction[15:11] : instruction[20:16];
		
	assign read_reg1 = instruction[25:21]; // rs
	
	assign read_reg2 = instruction[20:16]; // rd or rt
	
	
	
	register_block rb (
    .read_data1(read_data1),
    .read_data2(read_data2),
    .write_data(write_data_reg),
    .read_reg1(read_reg1),
    .read_reg2(read_reg2),
    .write_reg(write_reg),
    .regWrite(regWrite),
	 .byteOperations (byteOperations)
   );
	
	

  
  
   assign immediate = instruction[15:0];
  
   sign_extend se (
		.sign_ext_imm(sign_ext_imm),
		.imm(immediate)
   );
  

	
	assign alu_b = alusrc ? sign_ext_imm : read_data2;
  
   assign alu_a = read_data1;
  
  
   assign function_code = instruction[5:0];
	
	 
  
   alu_control ac (
		alu_ctr,
		function_code,
		aluop
   );
  
  
	 alu my_alu (
		alu_result,
		zero_bit, 
		alu_a,
		alu_b,
		alu_ctr
	 );
	 
	 
	 
	 assign write_data_mem = read_data2;
	 
	 memory_block mb (
		 .read_data(read_data_mem),
		 .byteOperations(byteOperations),
		 .address(alu_result[17:0]),
		 .write_data(write_data_mem),
		 .memRead(memRead),
		 .memWrite(memWrite)
	  );
	  
	  
	
	  
	 assign write_data_reg = move ? read_data1: (memRead ? read_data_mem : alu_result);
  
		
	assign calculated_pc = pc + 32'b00000000000000000000000000000100;
	
	
	shift_left_2 sl(shifted_address, sign_ext_imm);
	
	

	
	assign branch_addr = calculated_pc + shifted_address;

		
	assign is_branch = branch & ((zero_bit & beq_signal) | (~zero_bit & bne_signal)); // sadece beq
		
		
	assign next_pc = jump ? shifted_address : (is_branch ? branch_addr : calculated_pc);
	
	
		

	
	always @(posedge clock) begin
		
		 $display("next_pc: %d\n",next_pc);
		 pc <= next_pc;
	end
		
	

endmodule
