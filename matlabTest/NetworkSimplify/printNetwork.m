function printNetwork(E, networkList)
% 打印网络

count = size(networkList, 1);

msg = [];
for i=1:count
    uv = networkList{i, 1}; % 源汇
    type = networkList{i, 2}; % 网络类型
    parent = networkList{i,3}; % 父网络
    ve = networkList{i,4}; % 虚拟分支编号
    edges = networkList{i,5}; % 分支集合
    
    typestr = [];
    if type == 1
        typestr = '串联';
    elseif type == 2
        typestr = '并联';
    else
        typestr = '角联';
    end
    
    msg = [msg sprintf('网络编号:%d  源点:%d  汇点:%d  父网络编号:%d  虚拟分支编号:%d  网络结构类型:%s\n', i, uv(1), uv(2), parent, ve, typestr)];    
    msg = [msg sprintf('\t\t--> 包含分支: { ')];
    for e = edges
        msg = [msg sprintf('e%d ',e)]; 
    end
    msg = [msg sprintf('}\n\n')];
end

disp(msg);
end