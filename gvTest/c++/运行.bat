@echo off
rem 清空屏幕
cls

setlocal

rem 数据存放目录
set DATA_DIR=.\data

rem png图形存放目录
set PICTURE_DIR=.\picture

rem Graphviz安装目录
set DOT_EXE = D:\Program\Graphviz2.26.3\bin\dot.exe

echo ****************************************************************************
echo 使用graphviz软件生成图形
echo 前提准备：
echo     1) 安装Graphviz软件
echo     2) 目录下包含了gvFileGen.exe
echo     3) 安装了Microsoft Visual C++ 2008 SP1 Redistributable Package (x86)
echo     4) 目录下包含该批处理文件
echo     5) 目录下包含文件夹data和picture
echo     6) 其中data目录存放拓扑数据文件；picture目录存放生成的png图形
echo ****************************************************************************

for /f %%i in ('dir /b "%DATA_DIR%\*.txt"') do (
echo .
echo "正在处理拓扑数据文件%%i"

rem 添加分支编号
gvFileGen.exe "%DATA_DIR%\%%i"  -e

rem 不添加分支编号
rem gvFileGen.exe "%DATA_DIR%\%%i"

rem 生成png图形
dot -Tpng gd.gv -o  "%PICTURE_DIR%\%%~ni.png"

rem 生成ps文件
rem dot -Tps gd.gv -o  "%PICTURE_DIR%\%%~ni.ps"
)

echo 删除*.gv临时文件
rem del *.gv

echo ****************************************************************************
echo **************************** 生成完毕^_^_^ ******************************
echo ****************************************************************************

endlocal

pause