function test_Press
% 加载网络
[A, V, VE, E, vertexPosInTopology, from, to] = loadNetwork();

% 搜索独立通路
P = standalone_dfs(A, VE, from, to);

path = P{1};

totalPress=500; % 单位Pa
y = [totalPress];

for e = path
y = [y totalPress-E(e, 3)];
end

plot(1:1:length(path)+1, y);
axis([0 8 300 600]);
daspect([1 60 1]);
end