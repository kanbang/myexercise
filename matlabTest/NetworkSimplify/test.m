function test()
clc

[filename, pathname] = uigetfile('*.txt','��ѡ��һ����������txt�ļ�');
if isequal(filename,0)
   return;
end

% ��������
[V, E, from, to] = loadNetwork(fullfile(pathname, filename));

% �ų�û��Դ�����磨�����������һ������
if isempty(from) | isempty(to)
    disp('Դ�����Ƿ�', '��ʾ');
    return;
end

% �����Դ�������
if length(from)>1 | length(to)>1   
    msg = sprintf('\n��������һ����Դ�������:\n\tԴ�㼯��:%s\n\t��㼯�ϣ�%s\n', mat2str(from), mat2str(to));
    disp(msg);
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% ���������ʾ���������ݽṹ
A = createNetworkPresentationDataStructure(V, E); % ˫���ڽӱ�

% ���������Ϣ
printNetworkInfo(V, E, from, to, A, 0);

% ���ҵ����·
SC = findCycle_BySearch(E, A, from);

% Ŀǰ�в��ܴ������·
if ~isempty(SC)
    disp('��������ڵ����·')
    printCellArrayList(E, SC, 'C', 1);
    return;
end

% ��������Ľڵ���(�����·����)
tic
layer = lpm(E, A, from);
t1 = toc;
printLayer(V, layer);

tic
% �ָ�����
networkList = spliteNetwork(V, E, A, layer, from, to);
t2 = toc;

disp('��������:');
printNetwork(E, networkList); % ��ӡ��������

disp('�������״�ṹ:');
printTreeNetwork(V, E, networkList); % ��ӡ��״�ṹ������

disp(sprintf('\n�·���㷨ִ��ʱ��:%.3fs\t������㷨ִ��ʱ��:%.3fs',t1, t2));
end