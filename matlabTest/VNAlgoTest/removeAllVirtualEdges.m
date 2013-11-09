function out_list_r = removeAllVirtualEdges(E, out_list)
% 删除采用cell数组表示的邻接链表集合中的所有虚拟分支

    function V_edges = removeVirtualEdges(E, edges)
        % 删除虚拟分支，目前的算法中使用分支集合表示通路、树、单向回路
        V_edges = [];

        for e = edges
            if E(e,1) == -1 % 表示该分支是一条虚拟的分支
                continue;
            end
            V_edges = [V_edges e];
        end
    end

out_list_r = {};
n = length(out_list);
for i = 1:n
    out_list_r{i} = removeVirtualEdges(E, out_list{i});
end

end