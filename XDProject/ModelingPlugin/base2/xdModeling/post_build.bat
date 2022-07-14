@echo off
echo ------ Copying header files to ..\..\xdp\include\base2 ...

copy *.h ..\..\xdp\include\base2

if exist ..\..\xdp\lib\win64d\xdModeling.exp (
	del ..\..\xdp\lib\win64d\xdModeling.exp
) 

if exist ..\..\xdp\lib\win64r\xdModeling.exp (
	del ..\..\xdp\lib\win64r\xdModeling.exp
)

@echo off
echo ------ Copying copy xdModeling library to ..\..\x64\Debug ...

copy ..\..\xdp\lib\win64d\*.lib ..\..\x64\Debug
copy ..\..\xdp\product\win64d\bin\xdModeling* ..\..\x64\Debug