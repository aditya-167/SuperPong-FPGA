@echo off
set xv_path=D:\\Vivado\\Vivado_2016\\Vivado\\2016.4\\bin
call %xv_path%/xelab  -wto 2144bc9b36394b149327f0a6c03cf47c -m64 --debug typical --relax --mt 2 -L xil_defaultlib -L unisims_ver -L unimacro_ver -L secureip --snapshot inverter_behav xil_defaultlib.inverter xil_defaultlib.glbl -log elaborate.log
if "%errorlevel%"=="0" goto SUCCESS
if "%errorlevel%"=="1" goto END
:END
exit 1
:SUCCESS
exit 0
