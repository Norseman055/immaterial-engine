REM --------- GO TO DEV FOLDER, CLEAN ALL, BUILD ALL

call cd ..\dev\
call CleanAll.Bat
call BuildAll.Bat
call cd ..\ReleaseLibs

REM Clean old folders

call rmdir /s/q .\File
call rmdir /s/q .\Math
call rmdir /s/q .\PCSTree
call rmdir /s/q .\Time

REM update Math libs

call mkdir Math\lib

call copy ..\dev\Math\Debug\*.lib .\Math\lib\
call copy ..\dev\Math\Debug\*.pdb .\Math\lib\
call copy ..\dev\Math\Release\*.lib .\Math\lib\
call copy ..\dev\Math\Release\*.pdb .\Math\lib\

call mkdir Math\include
call copy ..\dev\Math\MathEngine\*.h .\Math\include\

REM update File libs

call mkdir File\lib
call copy ..\dev\File\Debug\*.lib .\File\lib\
call copy ..\dev\File\Debug\*.pdb .\File\lib\
call copy ..\dev\File\Release\*.lib .\File\lib\
call copy ..\dev\File\Release\*.pdb .\File\lib\

call mkdir File\include
call copy ..\dev\File\PA3\*.h .\File\include\

REM update PCSTree libs

call mkdir PCSTree\lib
call copy ..\dev\PCSTree\Debug\*.lib .\PCSTree\lib\
call copy ..\dev\PCSTree\Debug\*.pdb .\PCSTree\lib\
call copy ..\dev\PCSTree\Release\*.lib .\PCSTree\lib\
call copy ..\dev\PCSTree\Release\*.pdb .\PCSTree\lib\

call mkdir PCSTree\include
call copy ..\dev\PCSTree\PA1\*.h .\PCSTree\include\

REM update Time libs

call mkdir Time\lib
call copy ..\dev\Time\Debug\*.lib .\Time\lib\
call copy ..\dev\Time\Debug\*.pdb .\Time\lib\
call copy ..\dev\Time\Release\*.lib .\Time\lib\
call copy ..\dev\Time\Release\*.pdb .\Time\lib\

call mkdir Time\include
call copy ..\dev\Time\*.h .\Time\include\

call cd ..\dev\
call CleanAll.Bat