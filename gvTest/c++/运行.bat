@echo off
rem �����Ļ
cls

setlocal

rem ���ݴ��Ŀ¼
set DATA_DIR=.\data

rem pngͼ�δ��Ŀ¼
set PICTURE_DIR=.\picture

rem Graphviz��װĿ¼
set DOT_EXE = D:\Program\Graphviz2.26.3\bin\dot.exe

echo ****************************************************************************
echo ʹ��graphviz�������ͼ��
echo ǰ��׼����
echo     1) ��װGraphviz���
echo     2) Ŀ¼�°�����gvFileGen.exe
echo     3) ��װ��Microsoft Visual C++ 2008 SP1 Redistributable Package (x86)
echo     4) Ŀ¼�°������������ļ�
echo     5) Ŀ¼�°����ļ���data��picture
echo     6) ����dataĿ¼������������ļ���pictureĿ¼������ɵ�pngͼ��
echo ****************************************************************************

for /f %%i in ('dir /b "%DATA_DIR%\*.txt"') do (
echo .
echo "���ڴ������������ļ�%%i"

rem ��ӷ�֧���
gvFileGen.exe "%DATA_DIR%\%%i"  -e

rem ����ӷ�֧���
rem gvFileGen.exe "%DATA_DIR%\%%i"

rem ����pngͼ��
dot -Tpng gd.gv -o  "%PICTURE_DIR%\%%~ni.png"

rem ����ps�ļ�
rem dot -Tps gd.gv -o  "%PICTURE_DIR%\%%~ni.ps"
)

echo ɾ��*.gv��ʱ�ļ�
rem del *.gv

echo ****************************************************************************
echo **************************** �������^_^_^ ******************************
echo ****************************************************************************

endlocal

pause