function test_g()
% ����

% �����������ļ��м�������
[V, E, from, to]=loadNetwork('graph_topology.txt');

% ��Щ����û��Դ�㣨���������������һ������
% ��Ҫ�ų���Щ���
if isempty(from) | isempty(to)
    disp('Դ�����Ƿ�')
    return;
end

% �����Դ�������
if length(from)>1 | length(to)>1
    disp('==================================================================')
    disp('��������һ����Դ�������:')
    disp('Դ�㼯��:')
    disp(from)
    disp('��㼯��:')
    disp(to)
    disp('==================================================================')
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% ���������ʾ���������ݽṹ
[A, UA, B, Adj_list] = createNetworkPresentationDataStructure(V, E);

% ���������Ϣ
printNetworkInfo(V, E, from, to, A, UA, B, Adj_list);

% �д��󣨲��������е����·��ͼ��
% % ����һ��������
% ST = findSpanTree(Adj_list, E, from);
% % ɾ�������֧��ͬʱ����֧ת������ʵ�ķ�֧���
% ST = removeVirtualEdges(E, ST);
% disp('��֦:')
% disp(ST)
% 
% C = solveBasicCircuitMatrix(B, ST);
% disp('��·����:')
% disp(C)

SC=findCycle_ByMatrix(A, E, Adj_list);
SC = removeAllVirtualEdges(E, SC);  % ɾ�������֧��ͬʱ����֧ת������ʵ�ķ�֧���
disp('���ýڵ��ڽӾ�����㵥���·: ')
printCellArrayList(E, SC, 'C');

SC=findCycle_BySearch(E, Adj_list, from);
SC = removeAllVirtualEdges(E, SC);  % ɾ�������֧��ͬʱ����֧ת������ʵ�ķ�֧���
disp('����dfs���������·: ')
printCellArrayList(E, SC, 'C');

isConnectedGraph = checkGraphConnection_ByMatrix(UA);
disp('��������ͼ�ڵ��ڽӼ���ͼ����ͨ��:')
if isConnectedGraph
    disp(sprintf('\t===>ͼ����ͨ��'))
else
    disp(sprintf('\t===>ͼ�ǲ���ͨ��'))
end

isConnectedGraph = checkGraphConnection_BySearch(E);
disp('���������ķ�������ͼ����ͨ��:')
if isConnectedGraph
    disp(sprintf('\t===>ͼ����ͨ��'))
else
    disp(sprintf('\t===>ͼ�ǲ���ͨ��'))
end

% �ж������Ƿ��е����·
hasCycle = 0;
if isempty(SC)
    hasCycle = 1;
end

if ~hasCycle
    P = findAllPaths_NoCycle_ByMatrix(A, E, from, to);
    P = removeAllVirtualEdges(E, P);
    disp('���þ��󷽷������޵����·�����ͨ·:')
    printCellArrayList(E, P, 'P');
end

P = findAllPaths_BySearch(Adj_list, E, from, to, hasCycle); % ͬʱ���������޵����·�����
P = removeAllVirtualEdges(E, P);
disp('���������������(��/��)�����·�����ͨ·:')
printCellArrayList(E, P, 'P');

if ~hasCycle
    pathNum = countAllPathsNum_NoCycle_ByMatrix(A, from, to);
    disp('���þ��󷽷������޵����·ͨ·����:')
    disp(pathNum)
end

pathNum = countAllPathsNum_BySearch(Adj_list, E, from, to, hasCycle); % ͬʱ���������޵����·�����
disp('�������������������(��/��)�����·ͨ·����:')
disp(pathNum)

end
