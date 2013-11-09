function [V_V, V_E, V_from, V_to] = dealWithMultiSSNetwork(V, E, from, to)
% 处理多源多汇网络
% 输出参数：

    function flow = sumNodeOutFlow(E, u)
        % 计算节点的流出流量
        flow = 0;
        n = size(E,1);
        for e =1:n
            if E(e, 2) == u
                flow = flow + E(e, 4);
            end
        end
    end
    
    function flow = sumNodeInFlow(E, u)
        % 计算节点的流入流量
        flow = 0;
        for e =1:n
            if E(e, 3) == u
                flow = flow + E(e, 4);
            end
       end
    end

m = length(V); % 节点数
n = size(E,1); % 分支数

V_V = V;
V_E = E;
V_from = from(1);
V_to = to(1);

% 如果存在x个源点，则添加一个虚拟的源点和x个虚拟分支
if length(from)>1
    m = m+1;
    V_from = m;       % 虚拟的源点
    V_V(V_from) = -1; % 对应的真实节点编号为虚拟的-1
    
    sum = 0;
    for u = from
%         disp(sprintf('计算源点%d', u));
        flow = sumNodeOutFlow(E, u); % 流出流量
        sum = sum+flow;
        V_E(end+1, 1:end) = [-1 V_from u flow 0];        
    end
    
    disp(sprintf('\t==>流出总量:%d\n', sum));
end

% 如果存在x个汇点，则添加一个虚拟的汇点和x个虚拟的分支
if length(to)>1
    m = m+1;
    V_to = m;        % 虚拟的汇点
    V_V(V_to) = -1;  % 对应的真实节点编号为虚拟的-1
    
    % 向量可以直接用数字增加，
    % 矩阵不知道为什么有时好使，有时就不好使
    sum = 0;
    for u = to
%         disp(sprintf('计算汇点%d', u));
        flow = sumNodeInFlow(E, u); % 流入流量
        sum = sum+flow;
        V_E(end+1, 1:end) = [-1 u V_to flow 0];
    end
    
    disp(sprintf('\t==>流入总量:%d\n', sum));
end

end