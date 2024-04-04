module nor_32bit (
	output [31:0] result,
	input [31:0] a, 
	input [31:0] b
);

	nor_4bit nor1 (result[3:0], a[3:0], b[3:0]);
	nor_4bit nor2 (result[7:4], a[7:4], b[7:4]);
	nor_4bit nor3 (result[11:8], a[11:8], b[11:8]);
	nor_4bit nor4 (result[15:12], a[15:12], b[15:12]);
	nor_4bit nor5 (result[19:16], a[19:16], b[19:16]);
	nor_4bit nor6 (result[23:20], a[23:20], b[23:20]);
	nor_4bit nor7 (result[27:24], a[27:24], b[27:24]);
	nor_4bit nor8 (result[31:28], a[31:28], b[31:28]);
endmodule
	