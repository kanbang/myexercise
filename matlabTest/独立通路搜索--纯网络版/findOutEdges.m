function outEdges = findOutEdges(Adj_list, E, color, v, isColored)
% �������ܣ�
% ���ݵ�ǰ�����˽ṹ�����ҵ�ǰ�ڵ�v�ĳ��߷�֧
% 1��������ɫ��֧
% ���������
%   Adj_list -- �ڽӱ�
%   v -- Ҫ���ҳ��ߵĽڵ�
% ��������
%   outEdges -- �ڵ�v�ĳ��߷�֧

outEdges = Adj_list{v}; % ��ȡ����A�ĵ�v�У��ȼ��ڻ�ȡ�ڵ�v�ĳ�����Ϣ��

% ������ɫ��֧
outEdges = outEdges(find(color(outEdges)==isColored));

end