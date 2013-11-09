function  data = QSort2(data, low, high, layer)
% �������� -- 2ά����
if(low<high)
    [data pivokey] = qPartition(data, low, high, layer);
    data = QSort2( data, low, pivokey - 1, layer);
    data = QSort2( data, pivokey + 1, high, layer);
end

% ������ֳ������֣�ǰһ���ֵ�ֵ���Ⱥ�һ����ֵС
% ���طֽ��
    function [data low] = qPartition(data, low, high, layer)
        lu = layer(data(low, 1));
        lv = layer(data(low, 2));
        pivokey1 = lv - lu;        
        pivokey2 = lv + lu;
        
        while low < high           
            while low < high
                lu2 = layer(data(high, 1));
                lv2 = layer(data(high, 2));
                gap = lv2 -lu2;
                s = lv2 + lu2;
                if ~(gap > pivokey1 | (gap == pivokey1 & s >= pivokey2))
                    break;
                end
                
                high = high - 1;
            end
            c = data(low, 1:end); d = data(high, 1:end);
            data(low, 1:end) = d; data(high, 1:end) = c;
            
            while low < high
                lu2 = layer(data(low, 1));
                lv2 = layer(data(low, 2));
                gap = lv2 -lu2;
                s = lv2 + lu2;

                if ~(gap < pivokey1 | (gap==pivokey1 & s <= pivokey2))
                    break;
                end
                
                low = low + 1;
            end
            c = data(low, 1:end); d = data(high, 1:end);
            data(low, 1:end) = d; data(high, 1:end) = c;
        end
    end
end