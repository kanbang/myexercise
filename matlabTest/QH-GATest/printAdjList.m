function printAdjList(V, E, Adj_list, needTranslate)
% 输出邻接表
% 输入参数：
%   V               --   节点集合
%   E               --   分支集合
%   Adj_list        --   邻接表
%   needTranslate   --   是否需要翻译(输出正确的分支编号)

% 不能使用strcat方法，该方法会自动的删除字符串中的多余空白
msg = [];
n = length(Adj_list);
for i=1:n    
    msg = [msg sprintf('\tv%d -> { ', translateNode(V, i, needTranslate))];
    for e=Adj_list{i}
        msg = [msg sprintf('e%d ', translateEdge(E, e, needTranslate))];
    end
    msg = [msg sprintf('}\n')];
end

disp(msg);

    function tv = translateNode(V, v, needTranslate)
        tv = v;
        if needTranslate
            tv = V(v);
        end
    end

    function te = translateEdge(E, e, needTranslate)
        te = e;
        if needTranslate
            te = E(e, 1);
        end
    end
end