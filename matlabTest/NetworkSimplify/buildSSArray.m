function SS = buildSSArray(E, A, layer)
% 构建一条源(source)汇(sink)的2维数组
% 该数组按照层次间距和层次较小（层次和或平均值)来排序

SS = [];

m = size(A, 1);

% 构建源汇链表
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

% 按照层次大小排序
% source = InsertionSort1(source, layer);
% sink = InsertionSort1(sink, layer);

% 使用快速排序速度明显变快
source = QSort1(source, 1, length(source), layer);
sink = QSort1(sink, 1, length(sink), layer);

disp(sprintf('源点个数:%d\n源点数组:%s\n', length(source), mat2str(source)));
disp(sprintf('汇点个数:%d\n汇点数组:%s\n', length(sink), mat2str(sink)));

for u = source
    for v = sink
        if layer(v)>layer(u) % 汇点的层次必须要大于源点的层次
            SS = [SS; [u v]];
        end
    end
end

disp(sprintf('源汇数组对的个数:%d\n', size(SS, 1)));
% SS = InsertionSort2(SS, layer);
SS = QSort2(SS, 1, size(SS,1), layer);

end