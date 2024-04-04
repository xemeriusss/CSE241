module mux4x1_32bit (
	output [31:0] y,
	input [31:0] a,
	input [31:0] b,
	input [31:0] c,
	input [31:0] d,
	input sel0,
	input sel1
);

	wire [31:0] out1;
	wire [31:0] out2;

	mux2x1_32bit mux1 (out1, a, b, sel0);
	mux2x1_32bit mux2 (out2, c, d, sel0);
	mux2x1_32bit mux3 (y, out1, out2, sel1);

endmodule
