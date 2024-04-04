// adder with cla 32 bit (level 2)

module fa_claunit_32bit (
	output [31:0] sum,
	output c_out, 
	input [31:0] a, 
	input [31:0] b, 
	input c_in
);

	wire [31:0] p;
	wire [31:0] g;
	wire [31:0] c;
	

	// calculate necessary p and g's to send cla as input
	and_32bit and1 (g, a, b);
	
	or_32bit or1 (p, a, b);
	
	
	// cla will be done here, to find c'S
	fa_claunit_4bit my_cla1 (c[3:0], p[3:0], g[3:0], c_in);
	fa_claunit_4bit my_cla2 (c[7:4], p[7:4], g[7:4], c[3]);
	fa_claunit_4bit my_cla3 (c[11:8], p[11:8], g[11:8], c[7]);
	fa_claunit_4bit my_cla4 (c[15:12], p[15:12], g[15:12], c[11]);
	fa_claunit_4bit my_cla5 (c[19:16], p[19:16], g[19:16], c[15]);
	fa_claunit_4bit my_cla6 (c[23:20], p[23:20], g[23:20], c[19]);
	fa_claunit_4bit my_cla7 (c[27:24], p[27:24], g[27:24], c[23]);
	fa_claunit_4bit my_cla8 (c[31:28], p[31:28], g[31:28], c[27]);
	
	
	fa_4bit fa1 (sum[3:0], a[3:0], b[3:0], c_in);
	fa_4bit fa2 (sum[7:4], a[7:4], b[7:4], c[3]); // last c goes other cla
	fa_4bit fa3 (sum[11:8], a[11:8], b[11:8], c[7]);
	fa_4bit fa4 (sum[15:12], a[15:12], b[15:12], c[11]);
	fa_4bit fa5 (sum[19:16], a[19:16], b[19:16], c[15]);
	fa_4bit fa6 (sum[23:20], a[23:20], b[23:20], c[19]);
	fa_4bit fa7 (sum[27:24], a[27:24], b[27:24], c[23]);
	fa_4bit fa8 (sum[31:28], a[31:28], b[31:28], c[27]);
	
	
	//assign c_out = c[31];
	
	or_32bit res (c_out, c[31], 1'b0);


endmodule	
	
	