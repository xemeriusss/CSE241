module alu (
	output [31:0] alu_result,
	output zero_bit,
	input [31:0] alu_src1,
	input [31:0] alu_src2,
	input [2:0] alu_ctr // generated and sent by alu_control and received by alu
);

	wire [31:0] and_result;
	wire [31:0] or_result;
	wire [31:0] xor_result;
	wire [31:0] nor_result;
	wire [31:0] slt_result;
	wire [31:0] add_result;
	wire [31:0] sub_result;
	wire [31:0] mod_result;
	
	wire cout_add;
	wire cout_sub;
	
	wire [30:0] zb_result;
	wire zero_bit_or;
	
	and_32bit my_and (and_result, alu_src1, alu_src2);
	or_32bit my_or (or_result, alu_src1, alu_src2);
	xor_32bit my_xor (xor_result, alu_src1, alu_src2);
	nor_32bit my_nor (nor_result, alu_src1, alu_src2);
	slt_32bit my_slt (slt_result, alu_src1, alu_src2);
	fa_claunit_32bit my_cla (add_result, cout_add, alu_src1, alu_src2, 1'b0);
	sub_32bit my_sub (sub_result, cout_sub, alu_src1, alu_src2, 1'b0);
	//mod1 my_mod (mod_result, a, b, reset, CLK);

	mux8x1_32bit my_mux(alu_result, and_result, or_result, xor_result, nor_result, slt_result, add_result, sub_result, mod_result, alu_ctr[0], alu_ctr[1], alu_ctr[2]);
	
	//or_32bit zero_or(zero_signal, alu_result, 32'b0);
	
	//not not1(zero_bit, ~zero_signal);
	
	or (zb_result[0],xor_result[0],xor_result[1]);
	or (zb_result[1], zb_result[0], xor_result[2]);
	or (zb_result[2], zb_result[1], xor_result[3]);
	or (zb_result[3], zb_result[2], xor_result[4]);
	or (zb_result[4], zb_result[3], xor_result[5]);
	or (zb_result[5], zb_result[4], xor_result[6]);
	or (zb_result[6], zb_result[5], xor_result[7]);
	or (zb_result[7], zb_result[6], xor_result[8]);
	or (zb_result[8], zb_result[7], xor_result[9]);
	or (zb_result[9], zb_result[8], xor_result[10]);
	or (zb_result[10], zb_result[9], xor_result[11]);
	or (zb_result[11], zb_result[10], xor_result[12]);
	or (zb_result[12], zb_result[11], xor_result[13]);
	or (zb_result[13], zb_result[12], xor_result[14]);
	or (zb_result[14], zb_result[13], xor_result[15]);
	or (zb_result[15], zb_result[14], xor_result[16]);
	or (zb_result[16], zb_result[15], xor_result[17]);
	or (zb_result[17], zb_result[16], xor_result[18]);
	or (zb_result[18], zb_result[17], xor_result[19]);
	or (zb_result[19], zb_result[18], xor_result[20]);
	or (zb_result[20], zb_result[19], xor_result[21]);
	or (zb_result[21], zb_result[20], xor_result[22]);
	or (zb_result[22], zb_result[21], xor_result[23]);
	or (zb_result[23], zb_result[22], xor_result[24]);
	or (zb_result[24], zb_result[23], xor_result[25]);
	or (zb_result[25], zb_result[24], xor_result[26]);
	or (zb_result[26], zb_result[25], xor_result[27]);
	or (zb_result[27], zb_result[26], xor_result[28]);
	or (zb_result[28], zb_result[27], xor_result[29]);
	or (zb_result[29], zb_result[28], xor_result[30]);
	or (zb_result[30],zb_result[29], xor_result[31]);

	or (zero_bit_or, zb_result[30], 1'b0);
	not (zero_bit, zero_bit_or);

endmodule
