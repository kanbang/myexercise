function A = createNetworkPresentationDataStructure(V, E)
% ���������ʾ�������ݽṹ
% ���������
%   V -- �ڵ㼯��
%   E -- ��֧���ϣ���¼��֧��ʵ��š�ʼĩ�ڵ㣩
% ���������
%   A -- �ڽӱ�(���ߺ�����ڵ�˫���ڽӱ�)

m = length(V);
n = size(E,1);

% �����ڽӱ�A
A=cell(m, 2);
for u=1:m
    for e=1:n
        if u == E(e, 2)
            A{u,1} = [A{u,1} e]; % ��¼���߷�֧
        end
        
        if u == E(e, 3)
            A{u,2} = [A{u,2} e]; % ��¼��߷�֧
        end
    end
end

end