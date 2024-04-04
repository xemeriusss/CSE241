module instruction_block (
	output reg [31:0] instruction,
	input [31:0] pc
);

// pc 32 bit bu we will use lsb 10 bit of it
	/*
	wire [9:0] new_pc;
	
	or_1bit or1(new_pc[0], pc[0], 0);
	or_1bit or2(new_pc[1], pc[1], 0);
	or_1bit or3(new_pc[2], pc[2], 0);
	or_1bit or4(new_pc[3], pc[3], 0);
	or_1bit or5(new_pc[4], pc[4], 0);
	or_1bit or6(new_pc[5], pc[5], 0);
	or_1bit or7(new_pc[6], pc[6], 0);
	or_1bit or8(new_pc[7], pc[7], 0);
	or_1bit or9(new_pc[8], pc[8], 0);
	or_1bit or10(new_pc[9], pc[9], 0);
	*/

	//reg [31:0] mem [31:0]; // 32 instructions where each instruction is 32-bits
	reg [7:0] mem [64:0]; // 32 instructions where each instruction is 8-bits
	
	//wire [9:0] out1;
	
	always @ (*) begin
		$readmemb("instructions.mem", mem);
		//instruction = mem[new_pc];
		//$display("instruction: %b\n",instruction);
		instruction = {mem[pc], mem[pc+1], mem[pc+2], mem[pc+3]};
	end
	
	//or_32bit or_inst(instruction, {mem[pc], mem[pc+1], mem[pc+2], mem[pc+3]}, 32'b0); // structural
	
	//or_32bit or_inst(instruction, {22'b0000000000000000000000, out1}, 32'b00000000000000000000000000000000);
	

endmodule


