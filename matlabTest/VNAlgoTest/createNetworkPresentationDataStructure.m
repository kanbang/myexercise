function [A, UA, B, Adj_list] = createNetworkPresentationDataStructure(V, E)
% ���������ʾ���������ݽṹ
% ���������
%   V        --     �ڵ㼯��
%   E        --     ��֧����
% �������:
%   A        --     ����ͼ�ڵ��ڽӾ���
%   UA       --     ����ͼ�ڵ��ڽӾ��󣨶Գƾ���
%   B        --     ��ȫ��������
%   Adj_list --     �����ڽӱ�

m = length(V); % �ڵ���
n = size(E,1); % ��֧��

% �����ڽӱ�Adj_list
Adj_list=cell(m, 1);
for i=1:m
    for j=1:n
        v = E(j,2);          % ��֧��ʼ�ڵ�
        if v == i
            Adj_list{v}=[Adj_list{v}, j];   % ��j����֧�ǽڵ�v�ĳ��߷�֧
        end
    end
end

% �����ڽӾ���A�͹�������B
A=zeros(m, m);
B=zeros(m, n);
for i=1:n
    u = E(i,2); % ��֧��ʼ�ڵ�
    v = E(i,3); % ��֧��ĩ�ڵ�
    A(u,v)=1;
    B(u,i)=1;
    B(v,i)=-1;
end

% ��������ͼ�ڵ��ڽӾ���(�Գƾ���)
UA = A;
for i=1:m
    for j = 1:i
        if UA(i,j) == UA(j,i) % ͬʱ����0��ͬʱ����1
            continue;
        end
        UA(i,j)=1;
        UA(j,i)=1;
    end
end

end