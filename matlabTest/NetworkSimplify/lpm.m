function layer = lpm(E, A, from)
% 计算节点的最长路径长度
m = size(A,1); % 节点数

layer = zeros(1, m);

visited = zeros(1, m); % 节点是否被访问属性(0-未访问; 1-已访问)

parent(1:m)=-1;        % 节点的前置节点属性(-1表示未知或者NULL)

q = [from];            % 队列q
visited(from)=1;       % 出发节点设置为被访问过

while ~isempty(q)
    v = q(1);
    
    for e = A{v, 1} % 遍历节点v的出边
        w = E(e, 3);       % 出边分支的末节点
        
        if visited(w)      % 节点w已被访问过了
            if layer(w) < layer(v)+1
                parent(w) = v;
                layer(w) = layer(v)+1;
                
                for ee = A{w, 1} % 遍历节点w的出边
                    u = E(ee, 3);
                    if parent(u) == w
                        layer(u) = layer(w)+1;
                    end
                end
            end
        else               % 节点w尚未访问
            q = [q w];
            visited(w)=1;
            parent(w)=v;
            layer(w)=layer(v)+1;
        end
    end
    
    visited(v)=0;
    q = q(2:end);
end

end