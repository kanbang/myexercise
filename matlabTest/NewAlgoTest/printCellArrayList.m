function printCellArrayList(E, cycle, prefix, needTranslate)
% 输出使用cell表示的链表分支集合，通用格式如下：
% C1: --> { e1 e2 e3 }
% P2: --> {e5 e4 e0 e1}
% ...
% 目前需要打印的有3种集合：
%   通路集合P -- P{i}=[1 4 5] ,用分支表示通路
%   单向回路SC -- SC{i}=[2 3 45], 用分支表示单向回路
%   邻接表Adj_list -- Adj_list{i}=[1 4 5], 用分支表示出边邻接表
% 其中，Adj_list需要对节点的编号进行翻译，P和SC则不需要翻译

% 不能使用strcat方法，该方法会自动的删除字符串中的多余空白
msg = [];
n = length(cycle);
for i=1:n
    msg = [msg sprintf('\t%s%d -> { ', prefix, i)];
    for e=cycle{i}
        msg = [msg sprintf('e%d ', translateEdge(E, e, needTranslate))];
    end
    msg = [msg sprintf('}\n')];
end

disp(msg);

  function te = translateEdge(E, e, needTranslate)
        te = e;
        if needTranslate
            te = E(e, 1);
        end
    end
end