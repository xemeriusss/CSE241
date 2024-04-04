module alu_control (
	output [2:0] alu_ctr,
	input [5:0] function_code,
	input [2:0] aluop
);

	wire is_aluop_111; // rtype
	wire [2:0] itype_aluctr;
	wire [2:0] rtype_aluctr;
	
	
	and_1bit and1(rtype_out1, aluop[0], aluop[1]);
	and_1bit and2(is_aluop_111, rtype_out1, aluop[2]);
	
	// if i-type
	or_1bit or1(itype_aluctr[0], aluop[0], 1'b0);
	or_1bit or2(itype_aluctr[1], aluop[1], 1'b0);
	or_1bit or3(itype_aluctr[2], aluop[2], 1'b0);
	
	
	// if r-type, calculate aluctr with function code:
	
	//and_1bit and6(c2_out1, function_code[0], function_code[1]);
	//and and7(c2_out2, function_code[1], ~function_code[2], ~function_code[0]);
	//or_1bit or1(c2_out3, c2_out1, c2_out2);
	
	//aluctr[2]
	and and6(c2_out1, ~function_code[2], function_code[1], ~function_code[0]); 
	and and7(c2_out2, ~function_code[2], function_code[1], function_code[0]);
	and and8(c2_out3, function_code[2], function_code[1], function_code[0]);
	
	or or4(c2_out4, c2_out1, c2_out2, c2_out3);
	
	and and20(rtype_aluctr[2], ~function_code[5], ~function_code[4], ~function_code[3], c2_out4); 
	
	
	//aluctr[1]
	and_1bit and9(c1_out1, function_code[0], function_code[1]);
	and_1bit and10(c1_out2, c1_out1, ~function_code[2]);
	and_1bit and11(c1_out3, c1_out2, ~function_code[3]);
	and_1bit and12(c1_out4, c1_out3, ~function_code[4]);
	and_1bit and13(rtype_aluctr[1], c1_out4, ~function_code[5]);

	
	//aluctr[0]
	
	and and14(c0_out1, ~function_code[2], function_code[1], ~function_code[0]); 
	and and15(c0_out2, function_code[2], ~function_code[1], function_code[0]);
	
	or or5(c0_out3, c0_out1, c0_out2);
	
	and and16(rtype_aluctr[0], ~function_code[5], ~function_code[4], ~function_code[3], c0_out3);
	
	
	
	// if aluop = 111 (rtype) it will choose rtype_aluctr
	mux2x1_3bit r_or_i_type(alu_ctr, itype_aluctr, rtype_aluctr, is_aluop_111);
	

endmodule
