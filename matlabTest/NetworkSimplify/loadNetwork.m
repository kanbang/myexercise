function [V, E, from, to] = loadNetwork(filepath)
% �������ܣ��������ļ�graph_topology.txt�м�������
% ���������
%   V        --     �ڵ㼯��
%   E        --     ��֧����
%   from     --     Դ�㼯��
%   to       --     ��㼯��

% �����֧���ϣ�ʾ����
% E = [
%     % ��ʽ
%     % ��֧��� -- ʼ�ڵ��ţ�ĩ�ڵ��� -- ��֧���ԣ�����������ϵ����
%     1     1    2  % ��1����֧
%     2     2    4  % ��2����֧
%     3     4    6  % ��3����֧
%     4     3    2  % ��4����֧
%     5     2    5  % ��5����֧
%     6     5    4  % ��6����֧
%     7     1    3  % ��7����֧
%     8     3    5  % ��8����֧
%     9     5    6  % ��9����֧
%     ];

InE = load(filepath); % ��������

n = size(InE,1); % ��֧��

% ����ڵ���Ϣӳ���
V=[];
for i=1:n
    V = union(V, InE(i, 2:3)); % ��ȡ��֧��ʼĩ�ڵ㣬�ϲ��ظ��Ľڵ�
end

m = length(V); % �ڵ���

% �����֧ӳ��
E = zeros(n, 3); % ��¼��֧��š������֧��ʼĩ�ڵ��������
for i=1:n
    E(i,1) = InE(i,1);           % ��¼��֧����ʵ���
    E(i,2) = find(V==InE(i,2));  % ʼ�ڵ�ı����V�е�����λ��
    E(i,3) = find(V==InE(i,3));  % ĩ�ڵ�ı����V�е�����λ��
end

% �ڵ�ȵ���Ϣ�������ڵ�ĳ���outDegree�������ϢinDegree
VertexDegree=cell(m, 1);
for i=1:n 
    u = E(i, 2);  % ʼ�ڵ㣨�ڵ�ӳ����V�еı�ţ�
    v = E(i, 3);  % ĩ�ڵ㣨�ڵ�ӳ����V�еı�ţ�
    if isempty(VertexDegree{u})
        VertexDegree{u} = [VertexDegree{u}; [1 0]]; % ���ȳ�ʼ��Ϊ1����ʼ��Ϊ0
    else
        VertexDegree{u}(1) = VertexDegree{u}(1)+1; % ����+1
    end
    
    if isempty(VertexDegree{v})
        VertexDegree{v} = [VertexDegree{v}; [0 1]]; % ���ȳ�ʼ��Ϊ0����ʼ��Ϊ1
    else
        VertexDegree{v}(2) = VertexDegree{v}(2)+1; % ���+1
    end
end

% ����Դ��㣨�ڵ���V�е�ӳ��������������ʵ�Ľڵ��ţ�
from=[];
to=[];
for u=1:m
    outDegree = VertexDegree{u}(1);  % ��ȡ�ڵ�ĳ��Ⱥ����
    inDegree = VertexDegree{u}(2);
    if outDegree>0 & inDegree==0
        from = [from u];
    elseif outDegree==0 & inDegree>0
        to = [to u];
    end
end

end