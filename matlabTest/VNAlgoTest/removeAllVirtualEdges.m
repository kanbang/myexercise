function out_list_r = removeAllVirtualEdges(E, out_list)
% ɾ������cell�����ʾ���ڽ��������е����������֧

    function V_edges = removeVirtualEdges(E, edges)
        % ɾ�������֧��Ŀǰ���㷨��ʹ�÷�֧���ϱ�ʾͨ·�����������·
        V_edges = [];

        for e = edges
            if E(e,1) == -1 % ��ʾ�÷�֧��һ������ķ�֧
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