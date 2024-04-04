module not_32bit (
	output [31:0] result,
	input [31:0] a
);

	not_4bit not1 (result[3:0], a[3:0]);
	not_4bit not2 (result[7:4], a[7:4]);
	not_4bit not3 (result[11:8], a[11:8]);
	not_4bit not4 (result[15:12], a[15:12]);
	not_4bit not5 (result[19:16], a[19:16]);
	not_4bit not6 (result[23:20], a[23:20]);
	not_4bit not7 (result[27:24], a[27:24]);
	not_4bit not8 (result[31:28], a[31:28]);

endmodule
