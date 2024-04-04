module mux2x1_4bit (
	output [3:0] y,
	input [3:0] a,
	input [3:0] b,
	input sel
);

	mux2x1 mux1 (y[0], a[0], b[0], sel);
	mux2x1 mux2 (y[1], a[1], b[1], sel);
	mux2x1 mux3 (y[2], a[2], b[2], sel);
	mux2x1 mux4 (y[3], a[3], b[3], sel);

endmodule
