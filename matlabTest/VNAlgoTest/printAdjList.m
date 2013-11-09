function printAdjList(V, E, Adj_list, needTranslate)
% ����ڽӱ�
% ���������
%   V               --   �ڵ㼯��
%   E               --   ��֧����
%   Adj_list        --   �ڽӱ�
%   needTranslate   --   �Ƿ���Ҫ����(�����ȷ�ķ�֧���)

% ����ʹ��strcat�������÷������Զ���ɾ���ַ����еĶ���հ�
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