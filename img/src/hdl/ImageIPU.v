`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.03.2023 00:50:56
// Design Name: 
// Module Name: ImageIPU
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module ImageIPU(

    input clk,
    input rst,
    input  [7:0] r,
    input  [7:0] g,
    input  [7:0] b,
    output  [7:0] h_mask,
    output  [7:0] s_mask,
    output  [7:0] v_mask
    );
    
    wire [7:0] h_inter; wire [7:0]s_inter; wire [7:0] v_inter;
    
    
 hsvConverter rgb2hsv (
  .clk(clk),
  .rst(rst),
  .r(r),
  .g(g),
  .b(b),
  .h(h_inter),
  .s(s_inter),
  .v(v_inter));
  
  reg [7:0] Hl; reg [7:0] Hh;
  reg [7:0] sl; reg [7:0] sh;
  reg [7:0] vl; reg [7:0] vh;
  
  assign h_mask = (h_inter>Hh || h_inter < Hl) ? 0 : h_inter;
  assign s_mask = (s_inter>sh || s_inter < sl) ? 0 : s_inter;
  assign v_mask = (v_inter>vh || v_inter < vl) ? 0 : v_inter;
  
  
endmodule
