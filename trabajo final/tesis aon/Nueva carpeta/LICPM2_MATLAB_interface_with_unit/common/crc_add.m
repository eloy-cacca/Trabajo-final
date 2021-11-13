% Add 16-bit checksum to message. If done for a packet with checksum 
% appended, it should return 0.
function [ msg_out ] = crc_add ( msg_in )
    
    result = crc_calculate( msg_in );
    crc = dec2hex(result,4);
    msg_out = [ msg_in , hex2dec(crc(3:4)) , hex2dec(crc(1:2)) ];

end