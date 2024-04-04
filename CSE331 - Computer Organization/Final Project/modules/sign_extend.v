module sign_extend (
	output [31:0] sign_ext_imm,
	input [15:0] imm
);

	wire msb;
	wire [15:0] out1;
	
	or_1bit or1(msb, imm[15], 1'b0);
	
	or_16bit or2(out1, {16{msb}}, 16'b0000000000000000);
	
	or_32bit or3(sign_ext_imm, {out1, imm}, 32'b00000000000000000000000000000000);

endmodule
