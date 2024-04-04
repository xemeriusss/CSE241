// 1 bit full adder for cla
// There is no cout because it's calculated in full_adder4

module fa_1bit (
	output sum,
	input a, 
	input b, 
	input c_in
);
	
	
	//sum
	xor_1bit xor1 (a_xor_b, a, b);
	xor_1bit xor2 (sum, a_xor_b, c_in);
	
	//c_out
	//my_and and1 (and1_out, a_xor_b, c_in);
	//my_and and2 (and2_out, a, b);
	//my_or  or1  (c_out, and1_out, and2_out);
	
endmodule


