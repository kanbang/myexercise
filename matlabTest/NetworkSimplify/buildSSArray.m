function SS = buildSSArray(E, A, layer)
% ����һ��Դ(source)��(sink)��2ά����
% �����鰴�ղ�μ��Ͳ�ν�С����κͻ�ƽ��ֵ)������

SS = [];

m = size(A, 1);

% ����Դ������
source = [];
sink = [];
for u=1:m
    if length(A{u,1})>1
        source = [source u];
    end

    if length(A{u,2})>1
        sink = [sink u];
    end
end

% ���ղ�δ�С����
% source = InsertionSort1(source, layer);
% sink = InsertionSort1(sink, layer);

% ʹ�ÿ��������ٶ����Ա��
source = QSort1(source, 1, length(source), layer);
sink = QSort1(sink, 1, length(sink), layer);

disp(sprintf('Դ�����:%d\nԴ������:%s\n', length(source), mat2str(source)));
disp(sprintf('������:%d\n�������:%s\n', length(sink), mat2str(sink)));

for u = source
    for v = sink
        if layer(v)>layer(u) % ���Ĳ�α���Ҫ����Դ��Ĳ��
            SS = [SS; [u v]];
        end
    end
end

disp(sprintf('Դ������Եĸ���:%d\n', size(SS, 1)));
% SS = InsertionSort2(SS, layer);
SS = QSort2(SS, 1, size(SS,1), layer);

end