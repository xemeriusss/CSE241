module slt_32bit (
	output [31:0] slt_result,
	input [31:0] a,
	input [31:0] b
);

	wire c_out;
	wire [31:0] subt;
	
	sub_32bit my_sub (subt, c_out, a, b, 1'b0);	
	
	or_32bit res (slt_result, {31'b0, subt[31]}, 32'b0); // the slt result is msb of sub result
	
	
endmodule


	