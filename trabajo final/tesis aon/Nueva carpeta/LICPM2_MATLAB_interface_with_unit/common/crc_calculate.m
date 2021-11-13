% Add 16-bit checksum to message. If done for a packet with checksum 
% appended, it should return 0.
function [ result ] = crc_calculate ( msg )

    CRCPoly = hex2dec('A001');
    result = 0;
    for i=1:length(msg)
        data = msg(i);
        for k=1:8
            result_bit = bitand(result,1);
            result = floor(result/2);
            data_bit = bitand(data,1);
            data = floor(data/2);
            if (result_bit ~= data_bit)
                result = bitxor(result,CRCPoly);
            end
         %[i result msg(i) k result_bit data_bit data]    
        end
%    [i result]
    end

end