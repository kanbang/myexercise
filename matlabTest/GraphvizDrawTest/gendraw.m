function varargout = gendraw(varargin)
% GENDRAW M-file for gendraw.fig
%      GENDRAW, by itself, creates a new GENDRAW or raises the existing
%      singleton*.
%
%      H = GENDRAW returns the handle to a new GENDRAW or the handle to
%      the existing singleton*.
%
%      GENDRAW('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GENDRAW.M with the given input arguments.
%
%      GENDRAW('Property','Value',...) creates a new GENDRAW or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before gendraw_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to gendraw_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help gendraw

% Last Modified by GUIDE v2.5 12-Aug-2010 21:27:13

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @gendraw_OpeningFcn, ...
                   'gui_OutputFcn',  @gendraw_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before gendraw is made visible.
function gendraw_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to gendraw (see VARARGIN)

% Choose default command line output for gendraw
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes gendraw wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = gendraw_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;



function graphDir_Callback(hObject, eventdata, handles)
% hObject    handle to graphDir (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of graphDir as text
%        str2double(get(hObject,'String')) returns contents of graphDir as a double


% --- Executes during object creation, after setting all properties.
function graphDir_CreateFcn(hObject, eventdata, handles)
% hObject    handle to graphDir (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in sel3_btn.
function sel3_btn_Callback(hObject, eventdata, handles)
% hObject    handle to sel3_btn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
graphFiledirectory = uigetdir('','请选择存放拓扑数据文件的目录');
if ~isequal(graphFiledirectory, 0)
    set(handles.graphDir, 'String', graphFiledirectory);
end


function pngDir_Callback(hObject, eventdata, handles)
% hObject    handle to pngDir (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pngDir as text
%        str2double(get(hObject,'String')) returns contents of pngDir as a double


% --- Executes during object creation, after setting all properties.
function pngDir_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pngDir (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in sel4_btn.
function sel4_btn_Callback(hObject, eventdata, handles)
% hObject    handle to sel4_btn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
index = get(handles.graphFormat, 'Value');
gf='png';
if index==2
    gf='ps';
end

pngdirectory = uigetdir('',['请选择保存' gf '图形的目录']);
if ~isequal(pngdirectory, 0)
    set(handles.pngDir, 'String', pngdirectory);
end


function graphFile_Callback(hObject, eventdata, handles)
% hObject    handle to graphFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of graphFile as text
%        str2double(get(hObject,'String')) returns contents of graphFile as a double


% --- Executes during object creation, after setting all properties.
function graphFile_CreateFcn(hObject, eventdata, handles)
% hObject    handle to graphFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in sel1_btn.
function sel1_btn_Callback(hObject, eventdata, handles)
% hObject    handle to sel1_btn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[filename, pathname] = uigetfile('*.txt','');
if ~isequal(filename,0)
   set(handles.graphFile, 'String', fullfile(pathname, filename));
end


function pngFile_Callback(hObject, eventdata, handles)
% hObject    handle to pngFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of pngFile as text
%        str2double(get(hObject,'String')) returns contents of pngFile as a double


% --- Executes during object creation, after setting all properties.
function pngFile_CreateFcn(hObject, eventdata, handles)
% hObject    handle to pngFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in sel2_btn.
function sel2_btn_Callback(hObject, eventdata, handles)
% hObject    handle to sel2_btn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
index = get(handles.graphFormat, 'Value');
gf='png';
if index==2
    gf='ps';
end

[filename, pathname] = uiputfile(['*.' gf], ['请选择保存' gf '文件的路径'], 'test');
if ~isequal(filename,0)
   set(handles.pngFile, 'String', fullfile(pathname, filename));
end



% --- Executes on button press in run1.
function run1_Callback(hObject, eventdata, handles)
% hObject    handle to run1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clc

dotDir = get(handles.gv, 'String');
if isempty(dotDir)
    helpdlg('请指定Graphviz的安装目录');
    return;
end

dotExe = fullfile(dotDir, 'bin\dot.exe');

graphFile = get(handles.graphFile, 'String');
if isempty(graphFile)
    helpdlg('请选择一个拓扑数据文件!');
    return;
end

pngFile = get(handles.pngFile, 'String');
if isempty(pngFile)
    helpdlg('请选择保存图形文件的路径!');
    return;
end

index = get(handles.graphFormat, 'Value');
gf='png';
if index==2
    gf='ps';
end

% 生成图形
genOnce(dotExe, graphFile, gf, pngFile);

msgbox(sprintf('图形生成完毕! 请查看图形!'));

% --- Executes on button press in run2.
function run2_Callback(hObject, eventdata, handles)
% hObject    handle to run2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
clc

dotDir = get(handles.gv, 'String');
if isempty(dotDir)
    helpdlg('请指定Graphviz的安装目录');
    return;
end

dotExe = fullfile(dotDir, 'bin\dot.exe');

graphDir = get(handles.graphDir, 'String');
if isempty(graphDir)
    helpdlg('请选择存放拓扑数据的路径!');
    return;
end

pngDir = get(handles.pngDir, 'String');
if isempty(pngDir)
    helpdlg('请选择保存图形的目录!');
    return;
end

index = get(handles.graphFormat, 'Value');
gf='png';
if index==2
    gf='ps';
end
% 批量生成图形
genMore(dotExe, graphDir, gf, pngDir);

msgbox(sprintf('图形生成完毕!\n请打开目录%s\n查看图形!', pngDir));




function gv_Callback(hObject, eventdata, handles)
% hObject    handle to gv (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of gv as text
%        str2double(get(hObject,'String')) returns contents of gv as a double


% --- Executes during object creation, after setting all properties.
function gv_CreateFcn(hObject, eventdata, handles)
% hObject    handle to gv (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in sel0_btn.
function sel0_btn_Callback(hObject, eventdata, handles)
% hObject    handle to sel0_btn (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
dotRoot = uigetdir('','Graphviz');
if ~isequal(dotRoot, 0)
    set(handles.gv, 'String', dotRoot);
end



% --- Executes on selection change in graphFormat.
function graphFormat_Callback(hObject, eventdata, handles)
% hObject    handle to graphFormat (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns graphFormat contents as cell array
%        contents{get(hObject,'Value')} returns selected item from graphFormat


% --- Executes during object creation, after setting all properties.
function graphFormat_CreateFcn(hObject, eventdata, handles)
% hObject    handle to graphFormat (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on selection change in graphFormat.
function popupmenu1_Callback(hObject, eventdata, handles)
% hObject    handle to graphFormat (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns graphFormat contents as cell array
%        contents{get(hObject,'Value')} returns selected item from graphFormat


% --- Executes during object creation, after setting all properties.
function popupmenu1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to graphFormat (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


