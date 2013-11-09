@echo off
rem 清空屏幕
cls

setlocal

rem 输入数据存放目录
set IN_DATA_DIR=.\inData

rem  输出数据存放目录
set OUT_DATA_DIR=.\outData

echo ****************************************************************************
echo 使用newNum小程序修改编号生成图形
echo     1) 目录下包含该批处理文件
echo     2) 目录下包含newNum.exe程序
echo     3) 目录下包含文件夹inData和outData
echo     4) 其中inData目录存放原始的拓扑数据文件
echo         注1：只包含始末节点，不包含分支
echo     5) 输出文件存放在outData目录下
echo ****************************************************************************

for /f %%i in ('dir /b "%IN_DATA_DIR%\*.txt"') do (
echo.
echo "正在处理拓扑数据文件%%i"

rem 重新生成编号
newNum.exe "%IN_DATA_DIR%\%%i"  "%OUT_DATA_DIR%\%%i"
)

echo.
echo ***************************************************************************
echo **************************** 生成完毕^_^_^ ******************************
echo ***************************************************************************

endlocal

pause