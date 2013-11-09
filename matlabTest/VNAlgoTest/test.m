function varargout = test(varargin)
% TEST M-file for test.fig
%      TEST, by itself, creates a new TEST or raises the existing
%      singleton*.
%
%      H = TEST returns the handle to a new TEST or the handle to
%      the existing singleton*.
%
%      TEST('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TEST.M with the given input arguments.
%
%      TEST('Property','Value',...) creates a new TEST or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before test_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to test_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Copyright 2002-2003 The MathWorks, Inc.

% Edit the above text to modify the response to help test

% Last Modified by GUIDE v2.5 06-Aug-2010 00:25:53

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
    'gui_Singleton',  gui_Singleton, ...
    'gui_OpeningFcn', @test_OpeningFcn, ...
    'gui_OutputFcn',  @test_OutputFcn, ...
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


% --- Executes just before test is made visible.
function test_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to test (see VARARGIN)

% Choose default command line output for test
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes test wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = test_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in sel1.
function sel1_Callback(hObject, eventdata, handles)
% hObject    handle to sel1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
[filename, pathname] = uigetfile('*.txt','请选择一个拓扑数据文件');
if ~isequal(filename,0)
    set(handles.graphFile, 'String', fullfile(pathname, filename));
end

% --- Executes on button press in run1.
function run1_Callback(hObject, eventdata, handles)
% hObject    handle to run1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% clc§command window
clc

filepath = get(handles.graphFile, 'String');
if isempty(filepath)
    helpdlg('请选择一个拓扑数据txt文件!','提示');
    return;
end

% 从拓扑数据文件中加载网络
[V, E, from, to] = loadNetwork(filepath);

% 有些网络没有源汇（例如整个网络就是一个环）
% 需要排除这些情况
if isempty(from) | isempty(to)
    helpdlg('源点或汇点非法!','提示');
    return;
end

% 处理多源多汇网络
if length(from)>1 | length(to)>1
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% 创建网络表示方法的数据结构
[A, UA, B, Adj_list] = createNetworkPresentationDataStructure(V, E);

% 输出网络信息
% printNetworkInfo(V, E, from, to, A, UA, B, Adj_list, 0); % 不需要翻译

% 用户选择的测试
index = get(handles.testList, 'Value');

% 开始计时

msg=[];

if index == 1
    disp(sprintf('begin ============= 查找单向回路测试 ============ begin\n'));

%     t=0;
%     tic
%     SC=findCycle_ByMatrix(A, E, Adj_list);
%     t = toc;
%     msg = [msg sprintf('矩阵方法:%.3f', t)];
% 
%     SC = removeAllVirtualEdges(E, SC);
%     disp( '采用节点邻接矩阵计算单向回路:')
%     printCellArrayList(E, SC, 'C', 1); % 需要翻译

    t=0;
    tic
    SC=findCycle_BySearch(E, Adj_list, from);
    t=toc;
    msg=[msg sprintf('  搜索方法:%.3f', t)];

    SC = removeAllVirtualEdges(E, SC);
    disp('采用dfs搜索单向回路: ')
    printCellArrayList(E, SC, 'C', 1); % 需要翻译

    disp(sprintf('end ============= 查找单向回路测试 ============ end\n'));

elseif index == 2
    disp(sprintf('begin ============= 连通性判定 =============== begin\n'));

%     t=0;
%     tic
%     isConnectedGraph = checkGraphConnection_ByMatrix(V, E);
%     t = toc;
%     msg = [msg sprintf('矩阵方法:%.3f', t)];
% 
%     disp('采用无向图节点邻接计算图的连通性:')
%     if isConnectedGraph
%         disp(sprintf('\t===>图是连通的'))
%     else
%         disp(sprintf('\t===>图是不连通的'))
%     end

    t=0;
    tic
    isConnectedGraph = checkGraphConnection_BySearch(E);
    t=toc;
    msg=[msg sprintf('  搜索方法:%.3f', t)];

    disp('采用搜索的方法计算图的连通性:')
    if isConnectedGraph
        disp(sprintf('\t===>图是连通的'))
    else
        disp(sprintf('\t===>图是不连通的'))
    end

    disp(sprintf('end ============= 连通性判定 =============== end\n'));

elseif index == 3
    disp(sprintf('begin ============= 查找全部通路测试 ============ begin\n'));

%     hasCycle = ~isempty(findCycle_BySearch(E, Adj_list, from));
%     if hasCycle
%         disp(sprintf('[warning]网络中存在单向回路,因此只能使用搜索方法来搜索所有通路\n'));
%     end
% 
%     % 矩阵算法只适用于无单向回路的网络
%     if ~hasCycle
%         t=0;
%         tic
%         P = findAllPaths_NoCycle_ByMatrix2(A, E, from, to);
%         t = toc;
%         msg = [msg sprintf('矩阵方法:%.3f', t)];
% 
%         P = removeAllVirtualEdges(E, P);
%         disp('采用矩阵方法计算无单向回路网络的通路:')
%         printCellArrayList(E, P, 'P', 1); % 需要翻译
%     end

    t=0;
    tic
    P = findAllPaths_BySearch(Adj_list, E, from, to, hasCycle);
    t=toc;
    msg=[msg sprintf('  搜索方法:%.3f', t)];

    P = removeAllVirtualEdges(E, P);
    disp('采用深度优先搜索(有/无)单向回路网络的通路:')
    printCellArrayList(E, P, 'P', 1);

    disp(sprintf('end ============= 查找全部通路测试 ============ end\n'));

else
    disp(sprintf('begin ============= 计算全部通路数测试 ============ begin\n'));

%     hasCycle = ~isempty(findCycle_BySearch(E, Adj_list, from));
%     if hasCycle
%         disp(sprintf('[warning]网络中存在单向回路,因此只能使用搜索方法来计算通路总数\n'));
%         disp('');
%     end
% 
%     % 矩阵算法只适用于无单向回路的网络
%     if ~hasCycle
%         t=0;
%         tic
%         pathNum = countAllPathsNum_NoCycle_ByMatrix(A, from, to);
%         t = toc;
%         msg = [msg sprintf('矩阵方法:%.3f', t)];
% 
%         disp('采用矩阵方法计算无单向回路通路总数:')
%         disp(pathNum)
%     end

    t=0;
    tic
    pathNum = countAllPathsNum_BySearch(Adj_list, E, from, to, hasCycle);
    t=toc;
    msg=[msg sprintf('  搜索方法:%.3f', t)];

    disp('采用深度优先搜索计算(有/无)单向回路通路总数:')
    disp(pathNum)

    disp(sprintf('end ============= 计算全部通路数测试 ============ end\n'));
end

% 写入到static text中
%set(handles.tt, 'String', sprintf('%.3fs',t));
set(handles.tt, 'String', msg);

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


% --- Executes on button press in sel2.
function sel2_Callback(hObject, eventdata, handles)
% hObject    handle to sel2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
directory_name = uigetdir('','请选择存放拓扑数据文件的目录');
if ~isequal(directory_name, 0)
    set(handles.graphDir, 'String', directory_name);
end

% --- Executes on button press in run2.
function run2_Callback(hObject, eventdata, handles)
% hObject    handle to run2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on selection change in testList.
function testList_Callback(hObject, eventdata, handles)
% hObject    handle to testList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = get(hObject,'String') returns testList contents as cell array
%        contents{get(hObject,'Value')} returns selected item from testList


% --- Executes during object creation, after setting all properties.
function testList_CreateFcn(hObject, eventdata, handles)
% hObject    handle to testList (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


