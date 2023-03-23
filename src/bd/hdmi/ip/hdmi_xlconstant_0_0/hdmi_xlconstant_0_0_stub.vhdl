-- Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2016.4 (win64) Build 1756540 Mon Jan 23 19:11:23 MST 2017
-- Date        : Sun Feb 19 15:51:07 2023
-- Host        : DESKTOP-RMQ09J8 running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub
--               D:/Desktop/Angad/532/Video/Nexys-Video-HDMI-2016.4-2/Nexys-Video-HDMI/src/bd/hdmi/ip/hdmi_xlconstant_0_0/hdmi_xlconstant_0_0_stub.vhdl
-- Design      : hdmi_xlconstant_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7a200tsbg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity hdmi_xlconstant_0_0 is
  Port ( 
    dout : out STD_LOGIC_VECTOR ( 0 to 0 )
  );

end hdmi_xlconstant_0_0;

architecture stub of hdmi_xlconstant_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "dout[0:0]";
begin
end;
