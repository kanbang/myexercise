function A = createNetworkPresentationDataStructure(V, E)
% ���������ʾ�������ݽṹ
% ���������
%   V -- �ڵ㼯��
%   E -- ��֧���ϣ���¼��֧��ʵ��š�ʼĩ�ڵ㣩
% ���������
%   A -- �ڽӱ�

m = length(V);
n = size(E,1);

% �����ڽӱ�A
A=cell(m, 1);
for j=1:m
    for k=1:n
        v = E(k, 2);          % ��֧��ʼ�ڵ�
        if v == j             % ƥ��ʵ�ʵĽڵ���
            A{v}=[A{v}, k];   % ��k����֧�ǽڵ�v�ĳ��߷�֧
        end
    end
end

end