function [ msg ] = comm_pack_msg( settings , tx )

    % pack message
    msg = [ hex2dec(settings.MSG_START_BYTE) , ...
            hex2dec(settings.MSG_PROJECT_ID) , ...
            hex2dec(settings.MSG_RPI_ID) , ...
            hex2dec(tx.dest_id) , ...
            hex2dec(tx.msg_id) , ...
            length(tx.payload) , ...
            hex2dec(tx.payload)' ];
    msg = crc_add( msg );
    
end