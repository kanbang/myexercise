function layer = lpm(E, A, from)
% ����ڵ���·������
m = size(A,1); % �ڵ���

layer = zeros(1, m);

visited = zeros(1, m); % �ڵ��Ƿ񱻷�������(0-δ����; 1-�ѷ���)

parent(1:m)=-1;        % �ڵ��ǰ�ýڵ�����(-1��ʾδ֪����NULL)

q = [from];            % ����q
visited(from)=1;       % �����ڵ�����Ϊ�����ʹ�

while ~isempty(q)
    v = q(1);
    
    for e = A{v, 1} % �����ڵ�v�ĳ���
        w = E(e, 3);       % ���߷�֧��ĩ�ڵ�
        
        if visited(w)      % �ڵ�w�ѱ����ʹ���
            if layer(w) < layer(v)+1
                parent(w) = v;
                layer(w) = layer(v)+1;
                
                for ee = A{w, 1} % �����ڵ�w�ĳ���
                    u = E(ee, 3);
                    if parent(u) == w
                        layer(u) = layer(w)+1;
                    end
                end
            end
        else               % �ڵ�w��δ����
            q = [q w];
            visited(w)=1;
            parent(w)=v;
            layer(w)=layer(v)+1;
        end
    end
    
    visited(v)=0;
    q = q(2:end);
end

end