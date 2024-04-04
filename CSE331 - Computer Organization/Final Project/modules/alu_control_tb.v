module alu_control_tb ();

	wire [2:0] alu_ctr;
	reg [5:0] function_code;
	reg [2:0] aluop;
	
	reg CLK;

	alu_control ac (
    alu_ctr,
	 function_code,
	 aluop
  );
	
	
	initial begin
		
		#5  aluop = 3'b101; // addi...
		#100 $display("aluop: %d,  function_code: %b,  alu_ctr: %b",aluop, function_code, alu_ctr);
		
		#5 aluop = 3'b001; // ori
		#100 $display("aluop: %d,  function_code: %b,  alu_ctr: %b",aluop, function_code, alu_ctr);
		
		// add 101
		#5 function_code = 6'b000010; aluop = 111; 
		#100 $display("aluop: %d,  function_code: %b,  alu_ctr: %b",aluop, function_code, alu_ctr);
		
		//sub 110
		#5 function_code = 6'b000011; aluop = 111; 
		#100 $display("aluop: %d,  function_code: %b,  alu_ctr: %b",aluop, function_code, alu_ctr);
		
		// and 000
		#5 function_code = 6'b000100; aluop = 111; 
		#100 $display("aluop: %d,  function_code: %b,  alu_ctr: %b",aluop, function_code, alu_ctr);
		
		// or 001
		#5 function_code = 6'b000101; aluop = 111; 
		#100 $display("aluop: %d,  function_code: %b,  alu_ctr: %b",aluop, function_code, alu_ctr);
		
		// slt 100
		#5 function_code = 6'b000111; aluop = 111; 
		#100 $display("aluop: %d,  function_code: %b,  alu_ctr: %b",aluop, function_code, alu_ctr);
    

    #10 $finish; // Finish simulation

  end
	
	always
		begin
		#1
		CLK = ~CLK;
		end
		
endmodule
