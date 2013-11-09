function [V_V, V_E, V_from, V_to] = dealWithMultiSSNetwork(V, E, from, to)
% �����Դ�������
% ���������

    function flow = sumNodeOutFlow(E, u)
        % ����ڵ����������
        flow = 0;
        n = size(E,1);
        for e =1:n
            if E(e, 2) == u
                flow = flow + E(e, 4);
            end
        end
    end
    
    function flow = sumNodeInFlow(E, u)
        % ����ڵ����������
        flow = 0;
        for e =1:n
            if E(e, 3) == u
                flow = flow + E(e, 4);
            end
       end
    end

m = length(V); % �ڵ���
n = size(E,1); % ��֧��

V_V = V;
V_E = E;
V_from = from(1);
V_to = to(1);

% �������x��Դ�㣬�����һ�������Դ���x�������֧
if length(from)>1
    m = m+1;
    V_from = m;       % �����Դ��
    V_V(V_from) = -1; % ��Ӧ����ʵ�ڵ���Ϊ�����-1
    
    sum = 0;
    for u = from
%         disp(sprintf('����Դ��%d', u));
        flow = sumNodeOutFlow(E, u); % ��������
        sum = sum+flow;
        V_E(end+1, 1:end) = [-1 V_from u flow 0];        
    end
    
    disp(sprintf('\t==>��������:%d\n', sum));
end

% �������x����㣬�����һ������Ļ���x������ķ�֧
if length(to)>1
    m = m+1;
    V_to = m;        % ����Ļ��
    V_V(V_to) = -1;  % ��Ӧ����ʵ�ڵ���Ϊ�����-1
    
    % ��������ֱ�����������ӣ�
    % ����֪��Ϊʲô��ʱ��ʹ����ʱ�Ͳ���ʹ
    sum = 0;
    for u = to
%         disp(sprintf('������%d', u));
        flow = sumNodeInFlow(E, u); % ��������
        sum = sum+flow;
        V_E(end+1, 1:end) = [-1 u V_to flow 0];
    end
    
    disp(sprintf('\t==>��������:%d\n', sum));
end

end