function  data = QSort1(data, low, high, layer)
% 快速排序 -- 1维数组
if(low<high)
    [data pivokey] = qPartition(data, low, high, layer);
    data = QSort1( data, low, pivokey - 1, layer);
    data = QSort1( data, pivokey + 1, high, layer);
end

% 将数组分成两部分，前一部分的值均比后一部分值小
% 返回分界点
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