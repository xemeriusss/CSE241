module or_16bit (
	output [15:0] result,
	input [15:0] a, 
	input [15:0] b
);

	or_4bit or1 (result[3:0], a[3:0], b[3:0]);
	or_4bit or2 (result[7:4], a[7:4], b[7:4]);
	or_4bit or3 (result[11:8], a[11:8], b[11:8]);
	or_4bit or4 (result[15:12], a[15:12], b[15:12]);
endmodule
	