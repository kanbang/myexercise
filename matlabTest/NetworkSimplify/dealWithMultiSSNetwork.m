function [V_V, V_E, V_from, V_to] = dealWithMultiSSNetwork(V, E, from, to)
% �����Դ�������
% ���������   

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
    V_V(end+1) = -1; % ��Ӧ����ʵ�ڵ���Ϊ�����-1
    
    for u = from
        V_E(end+1, 1:end) = [-1 V_from u];  % ������֧
    end
end

% �������x����㣬�����һ������Ļ���x������ķ�֧
if length(to)>1
    m = m+1;
    V_to = m;        % ����Ļ��
    V_V(end+1) = -1;  % ��Ӧ����ʵ�ڵ���Ϊ�����-1
        
    for u = to
        V_E(end+1, 1:end) = [-1 u V_to];  % ������֧
    end
end

end