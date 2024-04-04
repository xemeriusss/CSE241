module xor_32bit (
	output [31:0] result,
	input [31:0] a, 
	input [31:0] b
);

	xor_4bit xor1 (result[3:0], a[3:0], b[3:0]);
	xor_4bit xor2 (result[7:4], a[7:4], b[7:4]);
	xor_4bit xor3 (result[11:8], a[11:8], b[11:8]);
	xor_4bit xor4 (result[15:12], a[15:12], b[15:12]);
	xor_4bit xor5 (result[19:16], a[19:16], b[19:16]);
	xor_4bit xor6 (result[23:20], a[23:20], b[23:20]);
	xor_4bit xor7 (result[27:24], a[27:24], b[27:24]);
	xor_4bit xor8 (result[31:28], a[31:28], b[31:28]);
	
endmodule
