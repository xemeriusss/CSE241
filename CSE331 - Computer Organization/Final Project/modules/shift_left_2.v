module shift_left_2 (
	output [31:0] shifted_address,
	input [31:0] address
);

	or_32bit or1 (shifted_address, {address[29:0], 2'b00}, 32'b00); // delete msb 2 bit, add 00 at the end
	
endmodule


	