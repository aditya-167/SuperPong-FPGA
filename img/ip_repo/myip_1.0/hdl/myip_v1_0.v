`timescale 1 ns / 1 ps

	module myip_v1_0 #
	(
		// Users to add parameters here
        parameter integer H_HIGH = 157,
        parameter integer S_HIGH = 255,
        parameter integer V_HIGH = 255,
        parameter integer H_LOW = 110,
        parameter integer S_LOW = 47,
        parameter integer V_LOW = 111,        
		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface S00_AXIS
		parameter integer C_S00_AXIS_TDATA_WIDTH	= 8,

		// Parameters of Axi Master Bus Interface M00_AXIS
		parameter integer C_M00_AXIS_TDATA_WIDTH	= 32,
		parameter integer C_M00_AXIS_START_COUNT	= 32
	)
	(
		// Users to add ports here

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface S00_AXIS
		input wire  s00_axis_aclk,
		input wire  s00_axis_aresetn,
		output wire  s00_axis_tready,
		input wire [C_S00_AXIS_TDATA_WIDTH-1 : 0] s00_axis_tdata,
		input wire  s00_axis_tlast,
		input wire  s00_axis_tvalid,

		// Ports of Axi Master Bus Interface M00_AXIS
		input wire  m00_axis_aclk,
		input wire  m00_axis_aresetn,
		output wire  m00_axis_tvalid,
		output wire [C_M00_AXIS_TDATA_WIDTH-1 : 0] m00_axis_tdata,
		output wire [(C_M00_AXIS_TDATA_WIDTH/8)-1 : 0] m00_axis_tstrb,
		output wire  m00_axis_tlast,
		input wire  m00_axis_tready
	);
	
	// Interconnects
	wire valid;
	wire [7:0] red, green, blue;
	wire [7:0] h_mask, s_mask, v_mask;
	
    // Instantiation of Axi Bus Interface S00_AXIS
	myip_v1_0_S00_AXIS # ( 
		.C_S_AXIS_TDATA_WIDTH(C_S00_AXIS_TDATA_WIDTH)
	) myip_v1_0_S00_AXIS_inst (
		.S_AXIS_ACLK(s00_axis_aclk),
		.S_AXIS_ARESETN(s00_axis_aresetn),
		.S_AXIS_TREADY(s00_axis_tready),
		.S_AXIS_TDATA(s00_axis_tdata),
		.S_AXIS_TLAST(s00_axis_tlast),
		.S_AXIS_TVALID(s00_axis_tvalid),
		.valid(valid),
		.red(red),
		.green(green),
		.blue(blue)
	);

    // Instantiation of Axi Bus Interface M00_AXIS
	myip_v1_0_M00_AXIS # ( 
		.C_M_AXIS_TDATA_WIDTH(C_M00_AXIS_TDATA_WIDTH),
		.C_M_START_COUNT(C_M00_AXIS_START_COUNT)
	) myip_v1_0_M00_AXIS_inst (
		.M_AXIS_ACLK(m00_axis_aclk),
		.M_AXIS_ARESETN(m00_axis_aresetn),
		.M_AXIS_TVALID(m00_axis_tvalid),
		.M_AXIS_TDATA(m00_axis_tdata),
		.M_AXIS_TSTRB(m00_axis_tstrb),
		.M_AXIS_TLAST(m00_axis_tlast),
		.M_AXIS_TREADY(m00_axis_tready)
	);

    ImageIPU #(.H_HIGH(H_HIGH), .S_HIGH(S_HIGH), .V_HIGH(V_HIGH), .H_LOW(H_LOW), .S_LOW(S_LOW), .V_LOW(V_LOW))
        I_DUT (.clk(valid), .rst(s00_axis_aresetn), .r(red), .g(green), .b(blue), .h_mask(h_mask), .s_mask(s_mask), .v_mask(v_mask));

	endmodule