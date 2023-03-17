-- Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
-- Date        : Wed Mar  8 22:15:46 2023
-- Host        : LAPTOP-2FCFNK7J running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               D:/PROJECTS/FPGAImageProcessing/DMASys_2018/DMASys_2018.srcs/sources_1/bd/design_1/ip/design_1_clk_wiz_1_4/design_1_clk_wiz_1_4_stub.vhdl
-- Design      : design_1_clk_wiz_1_4
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7a200tsbg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity design_1_clk_wiz_1_4 is
  Port ( 
    clk_out1 : out STD_LOGIC;
    resetn : in STD_LOGIC;
    locked : out STD_LOGIC;
    clk_in1 : in STD_LOGIC
  );

end design_1_clk_wiz_1_4;

architecture stub of design_1_clk_wiz_1_4 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clk_out1,resetn,locked,clk_in1";
begin
end;
