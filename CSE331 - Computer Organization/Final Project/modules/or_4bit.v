module or_4bit (
	output [3:0] y, 
	input [3:0] a,
	input [3:0] b
);

	or_1bit or1 (y[0], a[0], b[0]);
	or_1bit or2 (y[1], a[1], b[1]);
	or_1bit or3 (y[2], a[2], b[2]);
	or_1bit or4 (y[3], a[3], b[3]);
	
endmodule
