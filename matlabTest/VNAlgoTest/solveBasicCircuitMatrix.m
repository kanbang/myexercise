function C = solveBasicCircuitMatrix(B, T)
% ͨ����ȫ��������B��������·����

% ȥ����1�У�����һ��������������
B = B(2:end, 1:end);

% 1��������T������B���ָ��BL��BT
BL=[]; % ��֦
BT=[]; % ��֦
color=zeros(size(B,2),1);
for e=T
    color(e)=1; % ���Ϊ���߷�֧
end

for e=1:size(B,2)
    if color(e)
        BT = [BT B(1:end, e)]; % ��֦����
    else
        BL = [BL B(1:end, e)]; % ��֦����
    end
end

C = [];  % ��ʼ����·����
if det(BT)~=0
    I_BT=inv(BT); % BT�������
    M = -1*BL'*I_BT';
    C = [eye(size(M,1)) M];
end

% ��ʱ��C���а�����֦BL����֦BT��˳�����У�
% ���Ҫ���·������Ҫ��һ���Ĵ���
end