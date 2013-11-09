@echo off
echo 删除所有的vc未清理的临时文件
del /S /Q *.exp *.lib *.exe
del /S /Q *.ilk *.htm *.pch *.ncb *.pdb *.idb *.obj *.manifest *.dep *.res