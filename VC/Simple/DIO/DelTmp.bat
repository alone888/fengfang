@echo off
del *.exp
del *.bsc
del *.obj
del *.res
del *.sbr
del *.idb
del *.pch
del *.aps
del *.opt
del *.ilk
del *.ncb
del *.suo
del *.pdb

del x64\Debug\*.* /q
del x64\release\*.* /q
del x64
del Debug\*.* /q
del release\*.* /q

rd x64\Debug
rd x64\release
rd x64
rd Debug
rd release
