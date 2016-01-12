rmdir /S /Q Debug
rmdir /S /Q ipch
rmdir /S /Q Release
del /F /S *.sdf
del /F /S /A:H *.suo
del /F /S /A:H *.nativecodeanalysis.xml

rmdir /S /Q ImmaterialEngine\Debug
rmdir /S /Q ImmaterialEngine\Release
