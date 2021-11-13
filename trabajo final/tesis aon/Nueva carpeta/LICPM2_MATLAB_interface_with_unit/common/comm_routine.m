function [ rx_payload ] = comm_routine( settings , tx , byte_per_line )

    if strcmp( settings.type , 'UART' )
        % write request
        tx_msg = comm_pack_msg ( settings , tx );
        serialport_write ( settings.serial_port , tx_msg );
        comm_print_msg( 'TX: ' , tx_msg , byte_per_line );

        % read response
        rx_msg = comm_wait_msg ( settings , byte_per_line);
 
        % save payload
        rx_payload = rx_msg(settings.MSG_HEADER_1ST_BYTE:end-2)';
        
    elseif strcmp( settings.type , 'IP' )
 
        % convert data
        N = size(tx.payload,1);
        msg = '';
        for k=1:N
           msg(k*2-1) = tx.payload(k,1);
           msg(k*2)   = tx.payload(k,2);
        end
        
        % send mdg
        msg_rx_temp = urlread(['http://',settings.ip,'/cmd'],'POST',{'dest',tx.dest_id,'msg',tx.msg_id,'payload',msg});
        comm_print_msg( 'TX: ' , hex2dec([tx.dest_id ; tx.msg_id ; tx.payload]) , byte_per_line );        
      
        % convert data
        N = length(msg_rx_temp);
        rx_src_id = hex2dec([msg_rx_temp(1),msg_rx_temp(2)]);
        rx_msg_id = hex2dec([msg_rx_temp(4),msg_rx_temp(5)]);
        for k=1:(N-6)/2 % fisrt 6 data are discarted
            rx_payload_temp(k,1:2) = [msg_rx_temp(6+k*2-1),msg_rx_temp(6+k*2)];
        end
        rx_payload = hex2dec(rx_payload_temp);
        comm_print_msg( 'RX: ' , [rx_src_id ; rx_msg_id ; rx_payload] , byte_per_line );
          
    else
        fprintf(2,'No suported device\n');
    end

end