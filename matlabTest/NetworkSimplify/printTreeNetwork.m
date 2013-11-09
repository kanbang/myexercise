function printTreeNetwork(V, E, networkList)
% ��ӡ��״�ṹ������

% �����������"Դ���֧"
vEdges = [networkList{1:end, 4}];

% ��������ĩ�˵�������Ƕ�������!!!
msg = print(V, E, vEdges, networkList, size(networkList, 1), 0);

disp(msg);
  
    function msg = print(V, E, vEdges, networkList, networkNum, level)
        msg = [];
        
        uv = networkList{networkNum, 1}; % Դ��
        type = networkList{networkNum,2}; % ��������
        parent = networkList{networkNum,3}; % ��������
        ve = networkList{networkNum,4}; % �����֧���
        edges = networkList{networkNum,5}; % ��֧����

        % ��ӡͷ
        msg = [msg printHead(V(uv(1)), V(uv(2)), type, level)];

        for e = edges
            if ~isempty(find(vEdges==e)) % ��ʾ�÷�֧��һ�������"Դ���֧"
                msg = [msg print(V, E, vEdges, networkList, findNetworkByVEdge(networkList, e), level+1)];
            else
                msg = [msg printEdge(V, E, e, level+1)];
            end
        end
    end

    function index = findNetworkByVEdge(networkList, ve)
        % ���������֧��Ų���������
        index = -1;
        num = size(networkList, 1);
        for i=1:num
            if networkList{i,4}==ve
                index = i;
                break;
            end
        end
    end

    function msg = printTAB(level)
        msg = [];
        for i=1:level
            msg = [msg sprintf('|\t')];
        end
    end

    function msg = printHead(u, v, type, level)
        msg = printTAB(level);
        msg = [msg sprintf('|--->%s\n', getNetworkInfo(u, v, type))];
    end

    function msg = printEdge(V, E, e, level)
        msg = printTAB(level);
        u = E(e, 2);
        v = E(e, 3);
        msg = [msg sprintf('|--->e%d(v%d,v%d)\n',E(e,1), V(u), V(v))];
    end

    function str = getNetworkInfo(u, v, type)
        ss = [];
        if type == 1
            ss = 'S';
        elseif type == 2
            ss = 'P';
        else
            ss = 'D';
        end

        str = sprintf('%s(%d,%d)', ss, u, v);
    end

    function cn = findChild(networkList, parent)
        % ����parent����������
        cn = [];
        num = size(networkList, 1);
        for i=1:num
            if networkList{i,3}==parent
                cn = [cn i];
            end
        end
    end
end