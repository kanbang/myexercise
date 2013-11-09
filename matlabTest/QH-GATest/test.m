function test()
% ����ga��ƽ��ͼ����

clc


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%�����ʼ��%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

[filename, pathname] = uigetfile('*.txt','��ѡ��һ����������txt�ļ�');
if isequal(filename,0)
   return;
end

% ��������
[V, E, from, to] = loadNetwork(fullfile(pathname, filename));

% disp(V)
% disp(E)
% ��Щ����û��Դ�㣨���������������һ������
% ��Ҫ�ų���Щ���
if isempty(from) | isempty(to)
    disp('Դ�����Ƿ�', '��ʾ');
    return;
end

% ���Դ��
isBalanced = checkBalance(V, E);
if ~isBalanced
%     return;
end

% �����Դ�������
if length(from)>1 | length(to)>1
    msg = sprintf('\n��������һ����Դ�������:\n\tԴ�㼯��:%s\n\t��㼯�ϣ�%s\n', mat2str(V(from)), mat2str(V(to)));
    disp(msg);
    [V, E, from, to] = dealWithMultiSSNetwork(V, E, from, to);
end

% ���������ʾ���������ݽṹ
A = createNetworkPresentationDataStructure(V, E);

% ���������Ϣ
printNetworkInfo(V, E, from, to, A, 1);

% ���ҵ����·
SC = findCycle_BySearch(E, A, from);

% Ŀǰ�в��ܴ������·
if ~isempty(SC)
    disp('��������ڵ����·')
    printCellArrayList(E, SC, 'C', 1);
    return;
end

% ����һ������ķ�֧�������������㷨
VE = E(1:end, 2:4);

useGA = false;
reply = input('��ѡ���Ƿ�ʹ��ga? Y/N [N]: ','s');
if isempty(reply)
    useGA = false;
elseif strcmpi(reply, 'y')
    useGA = true;
elseif strcmpi(reply, 'n')
    useGA = false;
end

if useGA
    disp(sprintf('\t�û�ѡ��ʹ��ga�Ż���������\n'));
    result_A = runGA(A, VE, from , to);
else
    disp(sprintf('\t�û�ѡ��ʹ��ga�Ż���������\n'));
    result_A = A;
end

[P, lastPath, lastNode, lastE]= standalone_dfs(result_A, VE, from, to);             % ��������ͨ·

disp(sprintf('\n����һ��ͨ·:%s\n', mat2str(E(lastPath))));
disp(sprintf('\n��󵽴�Ľڵ�:%d\n', V(lastNode)));
outE = [];
for i = 1:size(lastE,1)
    outE = [outE; [E(i,1) lastE(i, 3)]];
end
disp(outE)

P1 = removeAllVirtualEdges(E, P(1:end, 1));
disp('��ӡ����ͨ·:')
printCellArrayList(E, P1, 'P', 1);                      % ��Ҫ����
%printNodePath(V, E, P1, 'P');

% figure
splitedEdges = drawQHGraph(A, V, E, from, P, 1);        % �󱻷ָ�ķ�֧��(����)����ƽ��ͼ

% ��ӡ���ָ�ķ�֧
disp('���ָ�ķ�֧:')
disp(splitedEdges)

end