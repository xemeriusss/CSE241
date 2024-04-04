module mux8x1_32bit (
	output [31:0] y,
	input [31:0] a,
	input [31:0] b,
	input [31:0] c,
	input [31:0] d,
	input [31:0] e,
	input [31:0] f,
	input [31:0] g,
	input [31:0] h,
	input sel0,
	input sel1,
	input sel2
);

	wire [31:0] out1;
	wire [31:0] out2;

	mux4x1_32bit mux1 (out1, a, b, c, d, sel0, sel1);
	mux4x1_32bit mux2 (out2, e, f, g, h, sel0, sel1);
	mux2x1_32bit mux3 (y, out1, out2, sel2);
	
endmodule
