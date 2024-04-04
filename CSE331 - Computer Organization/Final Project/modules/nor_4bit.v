module nor_4bit (
	output [3:0] y, 
	input [3:0] a,
	input [3:0] b
);
	
	nor_1bit nor1 (y[0], a[0], b[0]);
	nor_1bit nor2 (y[1], a[1], b[1]);
	nor_1bit nor3 (y[2], a[2], b[2]);
	nor_1bit nor4 (y[3], a[3], b[3]);

endmodule
