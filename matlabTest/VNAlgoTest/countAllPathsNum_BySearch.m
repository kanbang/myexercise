function pathNum = countAllPathsNum_BySearch(Adj_list, E, from, to, hasCycle)
% ���������ͨ·����������dfs����
% ���������޵����·�����磬Ҳ�������е����·������
% �ο����ף�[1] ���������硷��3��--3.2.3С��: 44-45
%          [2] ��ͨ�����纬�е����·ʱ��ͨ·�㷨��--�������ֽ���,�ڱ�
% ���������
%   Adj_list -- �ڽӱ�
%   E -- ��֧����
%   from, to -- Դ��ڵ�
%   hasCycle -- �Ƿ��е����·
% ���������
%   pathNum -- ͨ·����

% ֱ�ӵ���dfs����ȫ��ͨ·
P = findAllPaths_BySearch(Adj_list, E, from, to, hasCycle);

% ������ͨ·����
pathNum = length(P);
end