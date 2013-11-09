function P = findAllPaths_BySearch(Adj_list, E, from, to, hasCycle)
% ���������ȫ��ͨ·������dfs����
% ���������޵����·�����磬Ҳ�������е����·������
% �ο����ף�[1] ���������硷��3��--3.2.3С��: 44-45
%          [2] ��ͨ�����纬�е����·ʱ��ͨ·�㷨��--�������ֽ���,�ڱ�
% �������:
%   Adj_list -- �ڽӱ�
%   E -- ��֧����
%   from, to -- Դ��ڵ�
%   hasCycle -- �Ƿ��е����·
% ���������
%   P -- ͨ·����
% celldisp(P)

% ����ȫ��ͨ·
P = dfs_visit(Adj_list, E, from, to, hasCycle);

    function nodes = getNodesOfColoredEdges(E, color)
        nodes = [];
        for ee=1:size(E,1)
            if color(ee)
                 % ֻ�����ɫ��֧��ʼ�ڵ�
                 % ����2û��˵���(!!!!)
                nodes = union(nodes, E(ee,2));
            end
        end
    end

    function outEdges = findColoredOutEdges(Adj_list, color, u)
        % �������ܣ�
        % ���ݵ�ǰ�����˽ṹ�����ҵ�ǰ�ڵ�u�ĳ��߷�֧
        % 1����������ɫ��֧
        % ���������
        %   Adj_list -- �ڽӱ�
        %   u -- Ҫ���ҳ��ߵĽڵ�
        % ��������
        %   outEdges -- �ڵ�u�ĳ��߷�֧

        outEdges = Adj_list{u}; % ��ȡ����A�ĵ�u�У��ȼ��ڻ�ȡ�ڵ�v�ĳ�����Ϣ��

        % ������ɫ��֧
        outEdges = outEdges(find(color(outEdges)==1));

    end

    function outEdges = findUnColoredOutEdges_NoCycle(Adj_list, color, u)
        % �������ܣ�
        % ���ݵ�ǰ�����˽ṹ�����ҵ�ǰ�ڵ�u�ĳ��߷�֧
        % 1����������ɫ��֧-- �޵����·�����

        outEdges = Adj_list{u};
        outEdges = outEdges(find(color(outEdges)==0));
    end

    function outEdges = findUnColoredOutEdges(Adj_list, E, color, u)
        % �������ܣ�
        % ���ݵ�ǰ�����˽ṹ�����ҵ�ǰ�ڵ�u�ĳ��߷�֧
        % 1����������ɫ��֧-- �е����·�����

        outEdges = [];

        nodes = getNodesOfColoredEdges(E, color);
        % ������ɫ��֧
        for ee = Adj_list{u}
            if color(ee) | ~isempty(find(nodes==E(ee, 3)))
                continue;
            end

            outEdges = [outEdges ee];
        end
    end

    function P = dfs_visit(Adj_list, E, u, v, hasCycle)
        path=[];
        P={};
        color= zeros(1, size(E, 1));  % �ڵ���ɫ����(��ʼ��Ϊ0��Ĭ�Ͻڵ㶼δ��ɫ)

        while true
            outEdges = [];
            % �޸ĺ��dfs����ͬʱ���������޵����·�����
            % findUnColoredOutEdges��ȫ���Դ���findUnColoredOutEdges_NoCycle
            % ��Ч�ʵĽǶȿ��ǣ�������if/else����ʽ(!!!!!!!!!!!!)
            if hasCycle
                outEdges = findUnColoredOutEdges(Adj_list, E, color, u);       % Ѱ���е����·����Ľڵ�u��δ��ɫ���߷�֧
            else
                outEdges = findUnColoredOutEdges_NoCycle(Adj_list, color, u);   % Ѱ���޵����·����Ľڵ�u��δ��ɫ���߷�֧
            end

            if isempty(outEdges)
                if isempty(path)
                    return;
                else
                    outEdges = findColoredOutEdges(Adj_list, color, u);    % Ѱ�ҽڵ�u����ɫ��֧����
                    color(outEdges) = 0; % ȫ����ɫ
                    u = E(path(end), 2); % ͨ·�����һ����֧��ʼ�ڵ���ΪѰ��ʼ�ڵ�
                    path = path(1:end-1);
                end
            else
                for e = outEdges
                    if color(e)
                        continue;
                    end

                    color(e) = 1;
                    path = [path, e];       % ����֧��ӵ�·����
                    u = E(e, 3);            % ��ѯ��֧��ĩ�ڵ�

                    if u == v
                        P{length(P)+1} = path;
                        u = E(path(end), 2); % ͨ·�����һ����֧��ʼ�ڵ���ΪѰ��ʼ�ڵ�
                        path = path(1:end-1);
                    end
                    break;
                end
            end
        end
    end

end