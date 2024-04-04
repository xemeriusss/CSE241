module sign_extend_tb;

  reg [15:0] imm;
  wire [31:0] sign_ext_imm;

  // Instantiate the sign_extend module
  sign_extend uut (
    .sign_ext_imm(sign_ext_imm),
    .imm(imm)
  );

  // Clock generation
  reg clk;
  always #5 clk = ~clk;

  // Initial values
  initial begin
    clk = 0;
    imm = 16'b0000000000000001; // You can change the input value as needed

    // Apply stimulus
    #10 imm = 16'b0000000000000011; // Change input value for the next test case
    #10 imm = 16'b1100000000000001; // Negative value test case
    #10 imm = 16'b0000000000000000; // Zero value test case
    #10 imm = 16'b1111111111111111; // Negative value test case

    #100 $finish;
  end

  // Clock driver
  always #5 clk = ~clk;

  // Display statements
  always @(posedge clk) begin
    $display("Time=%0t imm=%b sign_ext_imm=%b", $time, imm, sign_ext_imm);
  end

endmodule
