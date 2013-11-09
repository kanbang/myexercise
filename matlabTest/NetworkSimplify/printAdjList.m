function printAdjList(V, E, Adj_list, needTranslate)
% ����ڽӱ�
% ���������
%   V               --   �ڵ㼯��
%   E               --   ��֧����
%   Adj_list        --   �ڽӱ�
%   needTranslate   --   �Ƿ���Ҫ����(�����ȷ�ķ�֧���)

% ����ʹ��strcat�������÷������Զ���ɾ���ַ����еĶ���հ�
msg = [];
m = size(Adj_list,1); % �ڵ���
for i=1:m
    msg = [msg sprintf('\tv%d -> ', translateNode(V, i, needTranslate))];
    msg = [msg '����: { '];
    for e=Adj_list{i,1}
        msg = [msg sprintf('e%d ', translateEdge(E, e, needTranslate))];
    end
    msg = [msg sprintf('}')];
    
    msg = [msg sprintf('\t���: { ')];
    for e = Adj_list{i, 2}
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