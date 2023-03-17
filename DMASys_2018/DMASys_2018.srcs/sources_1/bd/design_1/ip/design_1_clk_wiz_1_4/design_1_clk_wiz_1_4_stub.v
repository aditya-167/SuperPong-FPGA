// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
// Date        : Wed Mar  8 22:15:46 2023
// Host        : LAPTOP-2FCFNK7J running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               D:/PROJECTS/FPGAImageProcessing/DMASys_2018/DMASys_2018.srcs/sources_1/bd/design_1/ip/design_1_clk_wiz_1_4/design_1_clk_wiz_1_4_stub.v
// Design      : design_1_clk_wiz_1_4
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a200tsbg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module design_1_clk_wiz_1_4(clk_out1, resetn, locked, clk_in1)
/* synthesis syn_black_box black_box_pad_pin="clk_out1,resetn,locked,clk_in1" */;
  output clk_out1;
  input resetn;
  output locked;
  input clk_in1;
endmodule
