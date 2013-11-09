function SC = findCycle_BySearch(E, Adj_list, from)
% ͨ�������ķ������㵥���·

m = size(Adj_list,1);
dfn = zeros(1,m); % �ѷ��ʽڵ��˳��
color = zeros(1,size(E, 1)); % �ڷ�֧�Ƿ��ѱ����ʹ�

parent(1:m)=-1;   % ��¼�ڵ�ĸ��ڵ��δ֪

k = 1; % ������
SC={};
for u=1:m
    if dfn(u)==0
        [SC, dfn, color, k, parent]=dfs(Adj_list, E, from, dfn, color, k, parent, SC);
    end
end

    function outSC=dfs(Adj_list, E, u, SC)
         w = u;
         color = zeros(1, size(E, 1));                    % ��֧�Ƿ���ɫ
         visited = zeros(1,size(Adj_list,1));             % �ڵ��Ƿ��ѷ���
         path=[];
        while true
            outEdges = findOutEdges(A, E, color, w);
            if isempty(outEdges)
                if isempty(path)
                    return;
                else
                    w = E(path(end), 1); % ����
                    path = path(1:end-1);
                    continue;
                end
            end

            for e = outEdges
                w = E(e, 3);
                if color(e)
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
    end

    function outEdges = findOutEdges(A, E, color, v)
        outEdges = A{v}; % ��ȡ����A�ĵ�v�У��ȼ��ڻ�ȡ�ڵ�v�ĳ�����Ϣ��

        % �ų���ɫ��֧(��ǰ���ڴ���ķ�֧��Ҳ����path�еķ�֧)
        outEdges = outEdges(find(color(outEdges)==0));
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
