function printLayer(V, layer)
% ��ӡlayer����

m = length(V);
msg = [];
for u=1:m
    msg = [msg sprintf('layer(v%d) = %d\n', V(u), layer(u))];
end

disp(msg);
end