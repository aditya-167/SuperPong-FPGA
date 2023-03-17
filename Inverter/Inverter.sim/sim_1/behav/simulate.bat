@echo off
set xv_path=D:\\Vivado\\Vivado_2016\\Vivado\\2016.4\\bin
call %xv_path%/xsim inverter_behav -key {Behavioral:sim_1:Functional:inverter} -tclbatch inverter.tcl -log simulate.log
if "%errorlevel%"=="0" goto SUCCESS
if "%errorlevel%"=="1" goto END
:END
exit 1
:SUCCESS
exit 0
