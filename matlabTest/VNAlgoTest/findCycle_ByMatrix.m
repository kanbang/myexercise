function SC = findCycle_ByMatrix(A, E, Adj_list)
% ���ýڵ��ڽӾ�����㵥���·
% (***)�þ����㷨������ȱ�ݣ������·�ᱻ������
% �ο����ף���A new matrix-based mathematical model for determining
%            unidirectional circuits in a ventilation network�� -- �ֽ���
%            �������·��ѧģ��.doc�� -- Ӣ�������ص㲿�ֵ�����˵��
% ���������
%   A -- �ڵ��ڽӾ���
%   E -- ��֧����
%   Adj_list -- �ڽӱ�
% ���������
%   SC -- �����·����
% celldisp(SC)

m = size(A,1);
SC={};
TA=1;
for i=1:m-1
    TA=TA*A;
    nodesOfCycle = findNodesOfCycle(TA);
    if isempty(nodesOfCycle)
        continue;
    end

    % �����·�Ľڵ㲻һ���ǰ���˳�����е�
    % �þ����㷨������ȱ�ݣ������·�ᱻ������
    edgesOfCycle = findEdgesOfCycle(E, Adj_list, nodesOfCycle);
    cycleNum = length(edgesOfCycle);
    for i=1:cycleNum
        SC{end+1} = edgesOfCycle{i};
    end
end


% �Ӻ���
    function nodesOfCycle = findNodesOfCycle(A_k)
        % ���ҵ����·�ϵĽڵ�
        % �ýڵ��ʾ�����·
        nodesOfCycle =[];
        n = size(A_k,1);
        for j=1:n
            if A_k(j,j)~=0
                nodesOfCycle = [nodesOfCycle j];
            end
        end
    end

    function edgesOfCycle = findEdgesOfCycle(E, Adj_list, nodesOfCycle)
        % ���ݽڵ㵥���·���ҷ�֧�����·
        % ����һ���޸ĵ�dfs�㷨�������ɽڵ�nodesOfCycle���ɵĵ����·
        % �ο����ף����������硷��3��-3.2.4С�ڣ�44-45
        edgesOfCycle = {};
        color = zeros(1, size(Adj_list,1));
        visited = zeros(1, size(E,1)); % �Ƿ���ʹ�
        color(nodesOfCycle) = 1; % ��ǵ����·�еĽڵ�
        nodesOfPath = [];
        path=[];

        cycleEdges = [];
        u = nodesOfCycle(1);
        currentStartNode = u;
        otherCycleNodes = [];

        while true
            outEdges = findOutEdgesOfCycle_unVisited(E, Adj_list, color, visited, nodesOfPath, u);
            if isempty(outEdges)
                if isempty(path)
                    return;
                else
                    outEdges = findOutEdgesOfCycle_visited(E, Adj_list, color, visited, u);
                    for e = outEdges
                        visited(e)=0;
                    end
                    u = E(path(end), 2);
                    path = path(1:end-1);
                    nodesOfPath = nodesOfPath(1:end-1);
                end
            else
                for e = outEdges
                    u = E(e, 3);
                    visited(e)= 1;
                    path = [path e];
                    nodesOfPath = [nodesOfPath u];

                    if u == currentStartNode % �ҵ�һ�������·
                        edgesOfCycle{end+1} = path;
                        cycleEdges = [cycleEdges path]; % ��¼�Ѳ鵽�ĵ����·

                        if isempty(nodesOfCycle)
                            return;
                        end
                        currentStartNode = findStartNode(E, nodesOfCycle, cycleEdges);
                        if isempty(currentStartNode)
                            return;
                        end
                        u = currentStartNode;
                        path = [];
                        nodesOfPath = [];
                        color = zeros(1, size(Adj_list,1));
                        visited = zeros(1, size(E,1)); % �Ƿ���ʹ�
                        color(nodesOfCycle) = 1; % ��ǵ����·�еĽڵ�
                    end
                    break;
                end
            end
        end
    end

    function startNode = findStartNode(E, nodesOfCycle, cycleEdges)
        tnodes = [];
        for e = cycleEdges
            tnodes = union(tnodes, E(e,2:3));
        end

        startNode = [];
        nodes = [];
        for v = nodesOfCycle
            if(isempty(find(tnodes==v)))
                startNode = v;
                break;
            end
        end
    end

    function outEdges = findOutEdgesOfCycle_unVisited(E, Adj_list, color, visited, nodesOfPath, u)
        % ���ҽڵ�u���ڵ����·����δ���ʹ��Ľڵ�֮��ķ�֧
        outEdges = [];
        for e = Adj_list{u}
            v = E(e, 3);
            if ~isempty(find(nodesOfPath==v))
                continue;
            end

            if color(v) & ~visited(e)
                outEdges = [outEdges e];
            end
        end
    end

    function outEdges = findOutEdgesOfCycle_visited(E, Adj_list, color, visited, u)
        % ���ҽڵ�u���ڵ����·�����Ѿ����ʹ��Ľڵ�֮��ķ�֧
        outEdges = [];
        for e = Adj_list{u}
            v = E(e, 3);
            if color(v) & visited(e)
                outEdges = [outEdges e];
            end
        end
    end

end