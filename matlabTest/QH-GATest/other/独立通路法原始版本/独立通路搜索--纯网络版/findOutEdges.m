function outEdges = findOutEdges(A, E, color, v, isColored)
% �������ܣ�
% ���ݵ�ǰ�����˽ṹ�����ҵ�ǰ�ڵ�v�ĳ��߷�֧
% 1���ų���0������֧��2��������ɫ��֧
% ���������
%   A -- �ڵ��ڽӾ���
%   v -- Ҫ���ҳ��ߵĽڵ�
% ��������
%   outEdges -- �ڵ�v�ĳ��߷�֧

outEdges = A{v}; % ��ȡ����A�ĵ�v�У��ȼ��ڻ�ȡ�ڵ�v�ĳ�����Ϣ��

% �޸�!!!!!!!!!!!!!!!!!1
% �����ų�0������֧
outEdges = outEdges(find(E(outEdges, 3)>10^-3));

% ������ɫ��֧
outEdges = outEdges(find(color(outEdges)==isColored));

end