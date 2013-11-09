function SC = findCycle_BySearch(E, Adj_list, from)
% ͨ�������ķ������㵥���·

m = size(Adj_list,1);
dfn = zeros(1,m); % �ѷ��ʽڵ��˳��
color = zeros(1,m); % �ڵ��Ƿ��ѱ����ʹ�

parent(1:m)=-1;   % ��¼�ڵ�ĸ��ڵ��δ֪

k = 1; % ������
SC={};
for u=1:m
    if dfn(u)==0
        [SC, dfn, color, k, parent]=dfs(Adj_list, E, from, dfn, color, k, parent, SC);
    end
end

%SC = dfs_visit(Adj_list, E, color, parent, from, {});
%SC = dfs(Adj_list, E, from, dfn, color, k, parent, {});


    function [outSC, outDfn, outColor, outK, outParent]=dfs(Adj_list, E, u, dfn, color, k, parent, SC)
        dfn(u)=k;
        k = k+1;
        color(u)=1;

        for e = Adj_list{u}
            w = E(e, 3);
            if color(w)
                % �ҵ�һ�������·
                nodesOfCycle = findNodesOfCycle(parent, u, w);
                edgesOfCycle = findEdgesOfCycle(E, Adj_list, nodesOfCycle);
                if ~isempty(edgesOfCycle)
                    SC{end+1} = edgesOfCycle;
                end
            elseif dfn(w)==0
                parent(w)=u;
                [outSC, outDfn, outColor, outK, outParent]=dfs(Adj_list, E, w, dfn, color, k, parent, SC);
                dfn=outDfn;
                color=outColor;
                k=outK;
                parent=outParent;
                SC=outSC;
            end
        end
        color(u)=0;

        outDfn=dfn;
        outColor=color;
        outK=k;
        outParent=parent;
        outSC=SC;
    end

    function nodesOfCycle = findNodesOfCycle(parent, u, v)
        % ���ҵ����·u->v->u�ϵĽڵ�
        % �ýڵ��ʾ�����·
        nodesOfCycle =[];
        while u ~= v
            nodesOfCycle = [u nodesOfCycle]; % ����
            u = parent(u);
        end
        nodesOfCycle = [nodesOfCycle v];
    end

    function edgesOfCycle = findEdgesOfCycle(E, Adj_list, nodesOfCycle)
        % ���ݽڵ㵥���·���ҷ�֧�����·
        % �÷�֧��ʾ�����·
        edgesOfCycle = [];
        if length(nodesOfCycle) <= 1 % �������ٴ���2���ڵ�
            return;
        end

        % ��ⵥ���·�ķ�֧·��
        u = nodesOfCycle(1); % ȡ����1���ڵ���Ϊ��·�Ŀ�ʼ��
        nodesOfCycle = [nodesOfCycle u]; % ����һ����
        for v = nodesOfCycle
            if u == v
                continue;
            end

            for e = Adj_list{u}
                if E(e, 3) == v
                    edgesOfCycle = [edgesOfCycle e];
                end
            end
            u = v;
        end
    end


end