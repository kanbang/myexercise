function printCellArrayList(E, cycle, prefix, needTranslate)
% ���ʹ��cell��ʾ�������֧���ϣ�ͨ�ø�ʽ���£�
% C1: --> { e1 e2 e3 }
% P2: --> {e5 e4 e0 e1}
% ...
% Ŀǰ��Ҫ��ӡ����3�ּ��ϣ�
%   ͨ·����P -- P{i}=[1 4 5] ,�÷�֧��ʾͨ·
%   �����·SC -- SC{i}=[2 3 45], �÷�֧��ʾ�����·
%   �ڽӱ�Adj_list -- Adj_list{i}=[1 4 5], �÷�֧��ʾ�����ڽӱ�
% ���У�Adj_list��Ҫ�Խڵ�ı�Ž��з��룬P��SC����Ҫ����

% ����ʹ��strcat�������÷������Զ���ɾ���ַ����еĶ���հ�
msg = [];
n = length(cycle);
for i=1:n
    msg = [msg sprintf('\t%s%d -> { ', prefix, i)];
    for e=cycle{i}
        msg = [msg sprintf('e%d ', translateEdge(E, e, needTranslate))];
    end
    msg = [msg sprintf('}\n')];
end

disp(msg);

  function te = translateEdge(E, e, needTranslate)
        te = e;
        if needTranslate
            te = E(e, 1);
        end
    end
end