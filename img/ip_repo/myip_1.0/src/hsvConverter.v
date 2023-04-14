`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 24.03.2023 17:06:42
// Design Name: 
// Module Name: hsvConverter
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


module hsvConverter (
  input clk,
  input rst,
  input  [7:0] r,
  input  [7:0] g,
  input  [7:0] b,
  output  reg  [7:0] h,
  output  reg  [7:0] s,
  output  reg  [7:0] v
);

  reg [7:0] max_val, min_val, delta;
  /*reg [7:0] Hl; reg [7:0] Hh;
  reg [7:0] sl; reg [7:0] sh;
  reg [7:0] vl; reg [7:0] vh;*/

  always @(posedge clk or posedge rst) begin
    if (rst) begin
      // Reset values to 0 when reset is asserted
      h <= 0;
      s <= 0;
      v <= 0;
    end else begin
      // Find the maximum and minimum values among R, G, and B
      
      max_val <= (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
      min_val <= (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);

      // Calculate the value (V) component
      v <= max_val;
      
      /*if (v>vh || v < vl) begin
            v<=0;
      end
      else begin
        v <= max_val;
      end*/
      

      // Calculate the saturation (S) component
      if (max_val == 0) begin
        s <= 0;
      end else begin
        delta <= max_val - min_val;
        s <= (delta * 255) / max_val;
        /*if (s>sh || s < sl) begin
            s<=0;
        end 
        else begin
            s <= (delta * 255) / max_val;
        end*/
        
      end

      // Calculate the hue (H) component
      if (delta == 0) begin
        h <= 0;
      end else if (max_val == r) begin
      
        if(g>b) begin
            h <= (((g - b) * 60) / delta)%360;
            /*if (h>Hh || h < Hl) begin
                h<=0;
                end
            else begin
                h <= (((g - b) * 60) / delta)%360;
                end*/
            end
         else if (b>g) begin
            h <= 360 - (((b - g) * 60) / delta);
            /*if (h>Hh || h < Hl) begin
                h<=0;
                end
            else begin
                h <= 360 - (((b - g) * 60) / delta);
                end */ 
            end
        /*if (h < 0) begin
          h <= h + 360;
        end*/
      end else if (max_val == g) begin
        if (b>r) begin
            h <= (((b - r) * 60) / delta) + 120;
           
            /*if (h>Hh || h < Hl) begin
                h<=0;
                end
                
            else begin
                h <= (((b - r) * 60) / delta) + 120;
                end */  
            
        end
        else if (r>b) begin
            h <= 120 - (((r - b) * 60) / delta);
           /*if (h>Hh || h < Hl) begin
                h<=0;
                end 
           else  begin
                h <= 120 - (((r - b) * 60) / delta);
                end */
        end
      end else if (max_val == b) begin
        if (r>g) begin
            h <= (((r - g) * 60) / delta) + 240;
            /*if (h>Hh || h < Hl) begin
                h<=0;
                end
            else begin
                h <= (((r - g) * 60) / delta) + 240;
                end*/
        end
        else if (g>r) begin
            h <= 240 - (((g - r) * 60) / delta);
            /*if (h>Hh || h < Hl) begin
                h<=0;
                end
            else begin
                h <= 240 - (((g - r) * 60) / delta);
                end*/
          end

        end
      end
    end
  
endmodule

