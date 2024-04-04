module mux2x1 (
	output y,
	input a,
	input b,
	input sel
);

	and_1bit and1 (a_and_notsel, a, !sel);
	and_1bit and2 (b_and_sel, b, sel);
	or_1bit or1 (y, a_and_notsel, b_and_sel);
	
endmodule
