function printNetwork(E, networkList)
% ��ӡ����

count = size(networkList, 1);

msg = [];
for i=1:count
    uv = networkList{i, 1}; % Դ��
    type = networkList{i, 2}; % ��������
    parent = networkList{i,3}; % ������
    ve = networkList{i,4}; % �����֧���
    edges = networkList{i,5}; % ��֧����
    
    typestr = [];
    if type == 1
        typestr = '����';
    elseif type == 2
        typestr = '����';
    else
        typestr = '����';
    end
    
    msg = [msg sprintf('������:%d  Դ��:%d  ���:%d  ��������:%d  �����֧���:%d  ����ṹ����:%s\n', i, uv(1), uv(2), parent, ve, typestr)];    
    msg = [msg sprintf('\t\t--> ������֧: { ')];
    for e = edges
        msg = [msg sprintf('e%d ',e)]; 
    end
    msg = [msg sprintf('}\n\n')];
end

disp(msg);
end