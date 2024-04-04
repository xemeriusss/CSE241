module sub_32bit (
	output [31:0] result,
	output c_out, 
	input [31:0] a, 
	input [31:0] b, 
	input c_in
);

	wire [31:0] one_co; 
	wire [31:0] two_co;

	not_32bit ones_complement (one_co, b); // not b
	
	fa_claunit_32bit res (result, c_out, a, one_co, 1'b1); // a + (notb +1)
	
	// Instead of c_in, I gave 1
	
endmodule
