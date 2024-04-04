module xor_4bit (
	output [3:0] y, 
	input [3:0] a,
	input [3:0] b
);
	
	xor_1bit xor1 (y[0], a[0], b[0]);
	xor_1bit xor2 (y[1], a[1], b[1]);
	xor_1bit xor3 (y[2], a[2], b[2]);
	xor_1bit xor4 (y[3], a[3], b[3]);

endmodule
