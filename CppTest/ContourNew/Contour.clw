; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CContourView
LastTemplate=CMDIChildWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Contour.h"
LastPage=0

ClassCount=8
Class1=CContourApp
Class2=CContourDoc
Class3=CContourView
Class4=CMainFrame

ResourceCount=3
Resource1=IDR_MAINFRAME
Resource2=IDR_CONTOUTYPE
Class5=CChildFrame
Class6=CAboutDlg
Class7=CMyChildWnd
Class8=CMyChildFrame
Resource3=IDD_ABOUTBOX

[CLS:CContourApp]
Type=0
HeaderFile=Contour.h
ImplementationFile=Contour.cpp
Filter=N

[CLS:CContourDoc]
Type=0
HeaderFile=ContourDoc.h
ImplementationFile=ContourDoc.cpp
Filter=W
BaseClass=CDocument
VirtualFilter=DC
LastObject=CContourDoc

[CLS:CContourView]
Type=0
HeaderFile=ContourView.h
ImplementationFile=ContourView.cpp
Filter=C
LastObject=CContourView
BaseClass=CLogScrollView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
BaseClass=CMDIChildWnd
VirtualFilter=mfWC
LastObject=CChildFrame


[CLS:CAboutDlg]
Type=0
HeaderFile=Contour.cpp
ImplementationFile=Contour.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_CONTOUTYPE]
Type=1
Class=CContourView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
Command22=IDM_DRAWTRIANGLE
Command23=IDM_SAVEMEMBER
CommandCount=23

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=IDM_SAVEMEMBER
Command2=ID_EDIT_COPY
Command3=IDM_DRAWTRIANGLE
Command4=ID_FILE_NEW
Command5=ID_FILE_OPEN
Command6=ID_FILE_PRINT
Command7=ID_FILE_SAVE
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[CLS:CMyChildWnd]
Type=0
HeaderFile=mychildwnd.h
ImplementationFile=mychildwnd.cpp
BaseClass=CMDIChildWnd
Filter=M
LastObject=CMyChildWnd
VirtualFilter=mfWC

[CLS:CMyChildFrame]
Type=0
HeaderFile=mychildframe.h
ImplementationFile=mychildframe.cpp
BaseClass=CMDIChildWnd
Filter=M
LastObject=CMyChildFrame
VirtualFilter=mfWC

