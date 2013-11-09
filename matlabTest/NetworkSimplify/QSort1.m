function  data = QSort1(data, low, high, layer)
% �������� -- 1ά����
if(low<high)
    [data pivokey] = qPartition(data, low, high, layer);
    data = QSort1( data, low, pivokey - 1, layer);
    data = QSort1( data, pivokey + 1, high, layer);
end

% ������ֳ������֣�ǰһ���ֵ�ֵ���Ⱥ�һ����ֵС
% ���طֽ��
    function [data low] = qPartition(data, low, high, layer)        
        pivokey= layer(data(low));
        while low < high
            while low < high & layer(data(high)) >= pivokey
                high = high - 1;
            end
            c = data(low); d = data(high);
            data(low) = d; data(high) = c;
            
            while low < high & layer(data(low)) <= pivokey
                low = low + 1;
            end
            c = data(low); d = data(high);
            data(low) = d; data(high) = c;
        end
    end
end