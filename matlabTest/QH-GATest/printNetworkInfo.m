function printNetworkInfo(V, E, from, to, Adj_list, needTranslate)
% �������������Ϣ

disp('�������������Ϣ:')

m = length(V);
n = size(E,1);
disp(sprintf('�ڵ�����%d\t\t��֧����%d\n', m, n))

disp('�ڵ㼯��:')
disp(V)

disp('��֧����:')
disp(E)

disp('Դ��:')
disp(from)
disp('���:')
disp(to)

disp('�ڽӱ�:')
printAdjList(V, E, Adj_list, needTranslate);
end