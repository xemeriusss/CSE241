module mux2x1_32bit (
	output [31:0] y,
	input [31:0] a,
	input [31:0] b,
	input sel
);

	mux2x1_4bit mux1 (y[3:0], a[3:0], b[3:0], sel);
	mux2x1_4bit mux2 (y[7:4], a[7:4], b[7:4], sel);
	mux2x1_4bit mux3 (y[11:8], a[11:8], b[11:8], sel);
	mux2x1_4bit mux4 (y[15:12], a[15:12], b[15:12], sel);
	mux2x1_4bit mux5 (y[19:16], a[19:16], b[19:16], sel);
	mux2x1_4bit mux6 (y[23:20], a[23:20], b[23:20], sel);
	mux2x1_4bit mux7 (y[27:24], a[27:24], b[27:24], sel);
	mux2x1_4bit mux8 (y[31:28], a[31:28], b[31:28], sel);

endmodule
