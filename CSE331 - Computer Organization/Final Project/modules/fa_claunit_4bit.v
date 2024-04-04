// cla logic unit 4 bit

module fa_claunit_4bit (
	output [3:0] c_out,
	input [3:0] p,
	input [3:0] g,
	input c_in
);

	and_1bit and1 (p0_and_cin, p[0], c_in); 			// p*c_in
	or_1bit or1 (c_out[0], p0_and_cin, g[0]); 		// c1
	
	
	and_1bit and2 (p1_and_c1, p[1], c_out[0]);
	or_1bit or2 (c_out[1], p1_and_c1, g[1]);  		// c2
	
	
	and_1bit and3 (p2_and_c2, p[2], c_out[1]);
	or_1bit or3 (c_out[2], p2_and_c2, g[2]);  	// c3
	
	
	and_1bit and4 (p3_and_c3, p[3], c_out[2]);
	or_1bit or4 (c_out[3], p3_and_c3, g[3]); 	// c4
	

endmodule
