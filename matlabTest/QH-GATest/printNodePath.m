function printNodePath(V, E, cycle, prefix)
msg = [];
n = length(cycle);
for i=1:n
    msg = [msg sprintf('\t%s%d -> { ', prefix, i)];
    fe = cycle{1}(1);
    msg = [msg sprintf('v%d ', V(E(fe, 2)))];
    for e=cycle{i}
        msg = [msg sprintf('v%d ', V(E(e, 3)))];
    end
    msg = [msg sprintf('}\n')];
end

disp(msg);

end