module and_4bit (
	output [3:0] y, 
	input [3:0] a,
	input [3:0] b
);

	and_1bit and1 (y[0], a[0], b[0]);
	and_1bit and2 (y[1], a[1], b[1]);
	and_1bit and3 (y[2], a[2], b[2]);
	and_1bit and4 (y[3], a[3], b[3]);
	
endmodule
	
	