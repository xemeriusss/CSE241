module register_block_tb ();

	reg [31:0] write_data;
	reg [4:0] read_reg1, read_reg2, write_reg;
	reg regWrite;
	wire [31:0] read_data1, read_data2;
	reg CLK;

	register_block rb (
    .read_data1(read_data1),
    .read_data2(read_data2),
    .write_data(write_data),
    .read_reg1(read_reg1),
    .read_reg2(read_reg2),
    .write_reg(write_reg),
    .regWrite(regWrite)
  );
	
	
	
	initial begin
		read_reg1 = 0; read_reg2 = 1; write_reg = 20;regWrite = 1;write_data = 32'b00100000000000000000000000000011;
		#100 $display("reg1: %d,  reg2: %d,  reg1 content: %32b,  reg2 content: %32b, regwrite: %d",read_reg1, read_reg2, read_data1,read_data2,regWrite);
		
		read_reg1 = 0; read_reg2 = 1; write_reg = 25;regWrite = 1;write_data = 32'b00110000000000000000000000001111;
		#100 $display("reg1: %d,  reg2: %d,  reg1 content: %32b,  reg2 content: %32b, regwrite: %d",read_reg1, read_reg2, read_data1,read_data2,regWrite);
		/*
		read_reg1 = 2; read_reg2 = 10; write_reg = 0;regWrite = 0; write_data = 32'b00110000000000000000000000000000;
		#100 $display("reg1: %d,  reg2: %d,  reg1 content: %32b,  reg2 content: %32b, regwrite: %d",read_reg1, read_reg2, read_data1,read_data2,regWrite);

		read_reg1 = 0; read_reg2 = 4; write_reg = 7;regWrite = 1;write_data = 32'b00111000000000000000000000000001;
		#100 $display("reg1: %d,  reg2: %d,  reg1 content: %32b,  reg2 content: %32b, regwrite: %d",read_reg1, read_reg2, read_data1,read_data2,regWrite);
		
		read_reg1 = 7; read_reg2 = 3; write_reg = 6;regWrite = 0; write_data = 32'b11110000000000000000000000000000;
		#100 $display("reg1: %d,  reg2: %d,  reg1 content: %32b,  reg2 content: %32b, regwrite: %d",read_reg1, read_reg2, read_data1,read_data2,regWrite);
    */

    #10 $finish; // Finish simulation

  end
	
	always
		begin
		#1
		CLK = ~CLK;
		end
		
endmodule
