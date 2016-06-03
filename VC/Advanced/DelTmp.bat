@echo off 
del /s *.~h
del /s *.~cpp
del /s *.~bpr
del /s *.dof
del /s *.ddp
del /s *.~dfm
del /s *.~ddp
del /s *.log
del /s *.~dpr
del /s *.dcu
del /s *.~pas
del /s *.exp
del /s *.obj
del /s *.sbr
del /s *.idb
del /s *.pch
del /s *.aps
del /s *.bsc
del /s *.obj
del /s *.pdb
del /s *.ncb
del /s *.opt
del /s debug\*.exp
del /s debug\*.bsc
del /s debug\*.obj
del /s debug\*.sbr
del /s debug\*.idb
del /s debug\*.pch
del /s debug\*.ilk
del /s debug\*.pdb
del /s release\*.exp
del /s release\*.bsc
del /s release\*.obj
del /s release\*.sbr
del /s release\*.idb
del /s release\*.pch
del /s release\*.ilk
del /s release\*.pdb
del /s *.suo
del /s *.user
del debug\*.* /q

del release\*.* /q

rd debug

rd X86 /s /q
rd x64 /s /q

rd release
