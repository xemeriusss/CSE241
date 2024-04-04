module xor_1bit (output result, input a,b);
	and_1bit and1 (and1_out, a, b);
	and_1bit and2 (and2_out, a, !and1_out);
	and_1bit and3 (and3_out, b, !and1_out);
	and_1bit and4 (and4_out, !and2_out, !and3_out);
	not not1 (result, and4_out);
endmodule
