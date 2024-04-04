module and_32bit (
	output [31:0] result,
	input [31:0] a, 
	input [31:0] b
);

	and_4bit and1 (result[3:0], a[3:0], b[3:0]);
	and_4bit and2 (result[7:4], a[7:4], b[7:4]);
	and_4bit and3 (result[11:8], a[11:8], b[11:8]);
	and_4bit and4 (result[15:12], a[15:12], b[15:12]);
	and_4bit and5 (result[19:16], a[19:16], b[19:16]);
	and_4bit and6 (result[23:20], a[23:20], b[23:20]);
	and_4bit and7 (result[27:24], a[27:24], b[27:24]);
	and_4bit and8 (result[31:28], a[31:28], b[31:28]);
endmodule
	