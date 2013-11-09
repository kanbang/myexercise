function isConnectedGraph = checkGraphConnection_ByMatrix(V, E)
% �ж�ͼ�Ƿ���ͨ�����þ������ķ���
% �μ�����:�������ڽӾ���ͼ����ͨ���ж�׼�� -- �ֽ��£�����������ɭ
% ���������
%   UA -- ����ͼ�Ľڵ��ڽӾ���
% ���������
%   isConnectedGraph -- �Ƿ���ͨ

UA = createUA(V, E);
m = size(UA,1);

TA = UA; % ����۳�
S = UA; % ����ۼ�
for i=2:m-1
    TA = TA*UA;
    S = S + TA;
end

% ���S���Ƿ���ڲ�Ϊ0��Ԫ��
isConnectedGraph = true; % Ĭ��ͼ����ͨ��
for i = 1:m
    for j = 1:m
        if S(i,j) == 0
            isConnectedGraph = false;
            break;
        end
    end
    if ~isConnectedGraph
        break
    end
end


    function UA = createUA(V, E)
        % ������ʵ�Ľڵ���
        m = length(find(V~=-1));

        % �����ڽӾ���A�͹�������B
        A=zeros(m, m);
        for i=1:size(E,1)
            if E(i,1) ~= -1
                u = E(i,2); % ��֧��ʼ�ڵ�
                v = E(i,3); % ��֧��ĩ�ڵ�
                A(u,v)=1;
            end
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
end