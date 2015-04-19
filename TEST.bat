@echo off
for %%* in (.) do set ProjectName=%%~n*
echo %ProjectName%
make
%ProjectName%.3dsx