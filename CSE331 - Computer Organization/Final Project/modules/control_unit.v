module control_unit (
	output regDst,
	output branch,
	output memRead, // (This can also be used as memToReg)
	output memWrite,
	output [2:0] aluop, 
	output alusrc,
	output regWrite,
	
	output jump,
	output byteOperations, // (This is an optional signal for lb and sb operations)
	output move, // (This is an optional signal for MOVE operation)
	
	input [5:0] opcode
);

	wire rtype_opcode;
	

	
	
	
	// rtype opcode: 000000
	and_1bit and1(rt_out1, ~opcode[0], ~opcode[1]);
	and_1bit and2(rt_out2, rt_out1, ~opcode[2]);
	and_1bit and3(rt_out3, rt_out2, ~opcode[3]);
	and_1bit and4(rt_out4, rt_out3, ~opcode[4]);
	and_1bit and5(rtype_opcode, rt_out4, ~opcode[5]);
	
	
	// addi opcode: 000010
	and_1bit and6(addi_out1, ~opcode[0], opcode[1]);
	and_1bit and7(addi_out2, addi_out1, ~opcode[2]);
	and_1bit and8(addi_out3, addi_out2, ~opcode[3]);
	and_1bit and9(addi_out4, addi_out3, ~opcode[4]);
	and_1bit and10(addi_opcode, addi_out4, ~opcode[5]);
	
	
	// subi opcode: 000011
	and_1bit and11(subi_out1, opcode[0], opcode[1]);
	and_1bit and12(subi_out2, subi_out1, ~opcode[2]);
	and_1bit and13(subi_out3, subi_out2, ~opcode[3]);
	and_1bit and14(subi_out4, subi_out3, ~opcode[4]);
	and_1bit and15(subi_opcode, subi_out4, ~opcode[5]);
	
	// andi opcode: 000100
	and_1bit and16(andi_out1, ~opcode[0], ~opcode[1]);
	and_1bit and17(andi_out2, andi_out1, opcode[2]);
	and_1bit and18(andi_out3, andi_out2, ~opcode[3]);
	and_1bit and19(andi_out4, andi_out3, ~opcode[4]);
	and_1bit and20(andi_opcode, andi_out4, ~opcode[5]);
	
	
	// ori opcode: 000101
	and_1bit and21(ori_out1, opcode[0], ~opcode[1]);
	and_1bit and22(ori_out2, ori_out1, opcode[2]);
	and_1bit and23(ori_out3, ori_out2, ~opcode[3]);
	and_1bit and24(ori_out4, ori_out3, ~opcode[4]);
	and_1bit and25(ori_opcode, ori_out4, ~opcode[5]);
	
	
	// lw opcode: 001000
	and_1bit and26(lw_out1, ~opcode[0], ~opcode[1]);
	and_1bit and27(lw_out2, lw_out1, ~opcode[2]);
	and_1bit and28(lw_out3, lw_out2, opcode[3]);
	and_1bit and29(lw_out4, lw_out3, ~opcode[4]);
	and_1bit and30(lw_opcode, lw_out4, ~opcode[5]);
	
	
	// sw opcode: 010000
	and_1bit and31(sw_out1, ~opcode[0], ~opcode[1]);
	and_1bit and32(sw_out2, sw_out1, ~opcode[2]);
	and_1bit and33(sw_out3, sw_out2, ~opcode[3]);
	and_1bit and34(sw_out4, sw_out3, opcode[4]);
	and_1bit and35(sw_opcode, sw_out4, ~opcode[5]);
	
	
	// lb opcode: 001001
	and_1bit and36(lb_out1, opcode[0], ~opcode[1]);
	and_1bit and37(lb_out2, lb_out1, ~opcode[2]);
	and_1bit and38(lb_out3, lb_out2, opcode[3]);
	and_1bit and39(lb_out4, lb_out3, ~opcode[4]);
	and_1bit and40(lb_opcode, lb_out4, ~opcode[5]);
	
	// sb opcode: 010001
	and_1bit and41(sb_out1, opcode[0], ~opcode[1]);
	and_1bit and42(sb_out2, sb_out1, ~opcode[2]);
	and_1bit and43(sb_out3, sb_out2, ~opcode[3]);
	and_1bit and44(sb_out4, sb_out3, opcode[4]);
	and_1bit and45(sb_opcode, sb_out4, ~opcode[5]);
	
	// slti opcode: 000111
	and_1bit and46(slti_out1, opcode[0], opcode[1]);
	and_1bit and47(slti_out2, slti_out1, opcode[2]);
	and_1bit and48(slti_out3, slti_out2, ~opcode[3]);
	and_1bit and49(slti_out4, slti_out3, ~opcode[4]);
	and_1bit and50(slti_opcode, slti_out4, ~opcode[5]);
	
	
	// beq opcode: 100011
	and_1bit and51(beq_out1, opcode[0], opcode[1]);
	and_1bit and52(beq_out2, beq_out1, ~opcode[2]);
	and_1bit and53(beq_out3, beq_out2, ~opcode[3]);
	and_1bit and54(beq_out4, beq_out3, ~opcode[4]);
	and_1bit and55(beq_opcode, beq_out4, opcode[5]);
	
	
	// bne opcode:100111
	and_1bit and56(bne_out1, opcode[0], opcode[1]);
	and_1bit and57(bne_out2, bne_out1, opcode[2]);
	and_1bit and58(bne_out3, bne_out2, ~opcode[3]);
	and_1bit and59(bne_out4, bne_out3, ~opcode[4]);
	and_1bit and60(bne_opcode, bne_out4, opcode[5]);
	
	
	// jump opcode: 111000
	and_1bit and61(jump_out1, ~opcode[0], ~opcode[1]);
	and_1bit and62(jump_out2, jump_out1, ~opcode[2]);
	and_1bit and63(jump_out3, jump_out2, opcode[3]);
	and_1bit and64(jump_out4, jump_out3, opcode[4]);
	and_1bit and65(jump_opcode, jump_out4, opcode[5]);
	
	// jal opcode: 111001
	and_1bit and66(jal_out1, opcode[0], ~opcode[1]);
	and_1bit and67(jal_out2, jal_out1, ~opcode[2]);
	and_1bit and68(jal_out3, jal_out2, opcode[3]);
	and_1bit and69(jal_out4, jal_out3, opcode[4]);
	and_1bit and70(jal_opcode, jal_out4, opcode[5]);
	
	// move opcode: 100000
	and_1bit and71(move_out1, ~opcode[0], ~opcode[1]);
	and_1bit and72(move_out2, move_out1, ~opcode[2]);
	and_1bit and73(move_out3, move_out2, ~opcode[3]);
	and_1bit and74(move_out4, move_out3, ~opcode[4]);
	and_1bit and75(move_opcode, move_out4, opcode[5]);
	
	
	
	

	
	// if lw, memRead = 1
	or or1(memRead, lw_opcode, lb_opcode); // can also be memToReg
	
	// if sw, memWrite=1
	or_1bit or2(memWrite, sw_opcode, 1'b0);
	
	// if rtype, regDst=1
	or_1bit or3(regDst, rtype_opcode, 1'b0);
	
	or or4(alusrc, lw_opcode, sw_opcode, addi_opcode, andi_opcode, ori_opcode, slti_opcode, subi_opcode, lb_opcode, sb_opcode);
	
	or or5(regWrite, rtype_opcode, lw_opcode, lb_opcode, jal_opcode, addi_opcode, andi_opcode, ori_opcode, slti_opcode, subi_opcode, move_opcode);
	
	or or6(branch, beq_opcode, bne_opcode);
	
	or or7(jump, jump_opcode, 1'b0);
	
	or or8(move, move_opcode, 1'b0);
	
	or or9(aluop[2], slti_opcode, addi_opcode, lb_opcode, sb_opcode, lw_opcode, sw_opcode, subi_opcode, beq_opcode, bne_opcode, rtype_opcode);
	
	or or10(aluop[1], subi_opcode, beq_opcode, bne_opcode, rtype_opcode);
	
	or or11(aluop[0], ori_opcode,  addi_opcode, lb_opcode, sb_opcode, lw_opcode, sw_opcode, rtype_opcode);
	
	or or12(byteOperations, lb_opcode, sb_opcode);


endmodule
