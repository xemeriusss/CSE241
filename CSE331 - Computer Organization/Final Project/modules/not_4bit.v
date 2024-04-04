module not_4bit (
	output [3:0] y, 
	input [3:0] a
);

	not_1bit not1 (y[0], a[0]);
	not_1bit not2 (y[1], a[1]);
	not_1bit not3 (y[2], a[2]);
	not_1bit not4 (y[3], a[3]);
	
endmodule
	