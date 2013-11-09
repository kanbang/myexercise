function P = dfs_visit(Adj_list, E, u, v, color)
path=[];
P={};
color= zeros(1, size(E, 1));  % �ڵ���ɫ����(��ʼ��Ϊ0��Ĭ�Ͻڵ㶼δ��ɫ)

while true
    outEdges = findOutEdges(Adj_list, color, u, 0); % Ѱ�ҽڵ�u��δ��ɫ���߷�֧
    if isempty(outEdges)
        if isempty(path)
            return;
        else
            outEdges = findOutEdges(Adj_list, color, u, 1); % Ѱ�ҽڵ�u����ɫ��֧����
            color(outEdges(1))=0; % ��ѡһ����֧��ɫ������ѡ������˳��ĵ�һ����ɫ��֧��
            u = E(path(end), 1); % ͨ·�����һ����֧��ʼ�ڵ���ΪѰ��ʼ�ڵ�
            path = path(1:end-1);
        end
    else
        for e = outEdges
            if color(e)
                continue;
            end

            color(e) = 1;
            path = [path, e];       % ����֧��ӵ�·����
            u = E(e, 2);            % ��ѯ��֧��ĩ�ڵ�

            if u == v
                P{length(P)+1} = path;
                u = E(path(end), 1); % ͨ·�����һ����֧��ʼ�ڵ���ΪѰ��ʼ�ڵ�
                path = path(1:end-1);
            end
            break;
        end
    end
end
end