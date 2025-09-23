@echo off



if exist "MainBoard*.hex" del /s/q "MainBoard*.hex"

if exist "2GenerationNbInterrupt*.hex" del /s/q "2GenerationNbInterrupt*.hex"

copy "..\User\output\2GenerationNbInterrupt*.hex"

cd .\srec_cat\

srec_cat.exe ..\BootloaderForGD32F103.hex -Intel ..\2GenerationNbInterrupt*.hex -Intel -o ..\MainBoard_V0_90.hex -Intel

exit