// ADDER WITH 4 BIT CLA (level 1)

module fa_4bit (
	output [3:0] sum,
	input [3:0] a, 
	input [3:0] b, 
	input c_in
);

	wire [3:0] p;
	wire [3:0] g;
	wire [3:0] c;
	
	
	// calculate necessary p and g's to send cla as input
	and_1bit and1 (g[0], a[0], b[0]);
	and_1bit and2 (g[1], a[1], b[1]);
	and_1bit and3 (g[2], a[2], b[2]);
	and_1bit and4 (g[3], a[3], b[3]);
	
	or_1bit or1 (p[0], a[0], b[0]);
	or_1bit or2 (p[1], a[1], b[1]);
	or_1bit or3 (p[2], a[2], b[2]);
	or_1bit or4 (p[3], a[3], b[3]);
	
	// cla will be done here, to find c's
	fa_claunit_4bit my_cla (c, p, g, c_in);
	

	fa_1bit fa1 (sum[0], a[0], b[0], c_in);
	fa_1bit fa2 (sum[1], a[1], b[1], c[0]); // c1 goes fa2
	fa_1bit fa3 (sum[2], a[2], b[2], c[1]); // c2 goes fa3
	fa_1bit fa4 (sum[3], a[3], b[3], c[2]); // c3 goes fa4
	
	
	// c_out = c[3]
	//assign c_out = c[3];
	
endmodule
