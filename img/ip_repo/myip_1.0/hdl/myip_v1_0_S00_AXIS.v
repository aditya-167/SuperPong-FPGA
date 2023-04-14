`timescale 1 ns / 1 ps

	module myip_v1_0_S00_AXIS #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line

		// AXI4Stream sink: Data Width
		parameter integer C_S_AXIS_TDATA_WIDTH	= 32
	)
	(
            // Users to add ports here
            output reg valid,
            output reg [7:0] red,
            output reg [7:0] green,
            output reg [7:0] blue,
            // User ports ends
            
            // Do not modify the ports beyond this line
            // AXI4Stream sink: Clock
            input wire  S_AXIS_ACLK,
            // AXI4Stream sink: Reset
            input wire  S_AXIS_ARESETN,
            // Ready to accept data in
            output wire  S_AXIS_TREADY,
            // Data in
            input wire [C_S_AXIS_TDATA_WIDTH-1 : 0] S_AXIS_TDATA,
            // Indicates boundary of last packet
            input wire  S_AXIS_TLAST,
            // Data is in valid
            input wire  S_AXIS_TVALID
        );
        
        // User defined regs
        reg [7:0] packet_1, packet_2;
        reg flag;
        
        // Define the states of state machine
        // The control state machine oversees the writing of input streaming data to the FIFO,
        // and outputs the streaming data from the FIFO
        parameter [1:0] IDLE  = 1'b0,        // This is the initial/idle state 
                        WRITE = 1'b1;        // In this state we deserialize the data
                                        
        wire      axis_tready;
        // State variable
        reg mst_exec_state;
        
        // I/O Connections assignments
        assign S_AXIS_TREADY    = axis_tready;
        
        // Control state machine implementation
        always @(posedge S_AXIS_ACLK) begin  
           if (!S_AXIS_ARESETN) 
           // Synchronous reset (active low)
               mst_exec_state <= IDLE;
          else
               case (mst_exec_state)
                   IDLE: 
                   // The sink starts accepting tdata when there tvalid is asserted to mark the presence of valid streaming data 
                   if (S_AXIS_TVALID)
                       mst_exec_state <= WRITE;
                   else
                       mst_exec_state <= IDLE;
                   WRITE: 
                   // Keep writing till you recieve valid data
                   if (S_AXIS_TVALID)
                       mst_exec_state <= IDLE;
                   else
                       mst_exec_state <= WRITE;
               endcase
        end
        // AXI Streaming Sink 
        // The Sink is always ready to accept data if there is valid data on the stream input
        assign axis_tready = (mst_exec_state == WRITE);
    
        always@(posedge S_AXIS_ACLK) begin
           if(!S_AXIS_ARESETN) begin
               valid <= 1'b0;
               packet_1 <= 8'b0000_0000;
               packet_2 <= 8'b0000_0000;
               flag <= 1'b0;
           end  
           else if (flag == 1'b0) begin
               packet_1 <= S_AXIS_TDATA;
               valid <= 1'b0;
               flag <= 1'b1;
           end
           else if (flag == 1'b1) begin
               packet_2 <= S_AXIS_TDATA;
               valid <= 1'b1;
               flag <= 1'b0;
           end
        end
    
        always@(posedge valid) begin
            if(valid) begin
                red = (packet_1[7:3] * 255)/31;
                green = ((packet_1[2:0] * 8 + packet_2[7:5]) * 255)/31;
                blue = (packet_2[4:0] * 255)/31;
             end
        end
    
        endmodule