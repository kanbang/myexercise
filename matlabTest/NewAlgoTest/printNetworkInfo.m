function printNetworkInfo(V, E, from, to, A, UA, B, Adj_list, needTranslate)
% �������������Ϣ

disp('�������������Ϣ:')

m = length(V);
n = size(E,1);
disp(sprintf('�ڵ�����%d\t\t��֧����%d\n', m, n))

disp('�ڵ���Ϣ:')
disp(V)

disp('��֧��Ϣ:')
disp(E)

disp('Դ��:')
disp(from)
disp('���:')
disp(to)

disp('����ͼ���ڽӾ���:')
disp(A)

disp('����ͼ���ڽӾ���:')
disp(UA)

disp('��������:')
disp(B)

disp('�ڽӱ�:')
printAdjList(V, E, Adj_list, needTranslate);
end