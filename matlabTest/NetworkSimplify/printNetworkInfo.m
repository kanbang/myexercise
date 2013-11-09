function printNetworkInfo(V, E, from, to, Adj_list, needTranslate)
% 输出网络的相关信息

disp('输出网络的相关信息:')

m = length(V);
n = size(E,1);
disp(sprintf('节点数：%d\t\t分支数：%d\n', m, n))

disp('节点集合:')
disp(V)

disp('分支集合:')
disp(E)

disp('源点:')
disp(from)
disp('汇点:')
disp(to)

disp('邻接表:')
printAdjList(V, E, Adj_list, needTranslate);
end