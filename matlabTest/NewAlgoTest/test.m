function test()
% ����
clc

[filename, pathname] = uigetfile('*.txt','��ѡ��һ�����������ļ�');
if isequal(filename,0)
     helpdlg('��ѡ��һ����������txt�ļ�!','��ʾ');
     return;
end

% �����������ļ��м�������
[V, E, from, to] = loadNetwork(fullfile(pathname, filename));

% ��Щ����û��Դ�㣨���������������һ������
% ��Ҫ�ų���Щ���
if isempty(from) | isempty(to)
    helpdlg('Դ�����Ƿ�!','��ʾ');
    return;
end

% �����Դ�������
if length(from)>1 | length(to)>1
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% ���������ʾ���������ݽṹ
% �ڵ��ڽӾ�������ͼ�ڵ��ڽӾ��󡢹������󡢳����ڽӱ�
[A, UA, B, Adj_list] = createNetworkPresentationDataStructure(V, E);

% ��ӡ������Ϣ
printNetworkInfo(V, E, from, to, A, UA, B, Adj_list, 1);

% ͨ�����󷽷�����Դ��֮�������ͨ·
P = findAllPaths(A, E, from, to);

end
