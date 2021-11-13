% Read message from port. Set port and number of byte per line to be showed
% (if 0 no data is showed).
function [msg] = comm_wait_msg ( settings , byte_per_line)

    % start loop
    stop = 0;
    N = 0;
    while (~stop) 

        % wait byte
        temp = serialport_read(settings.serial_port,1);
        if isempty(temp) % if it is empty (timeout), go out
            fprintf('Byte received before timeout:\n')
            if exist('msg') % print received bytes
                comm_print_msg('RX: ',msg,byte_per_line);  
            else
                fprintf('RX:\n')
            end
            msg = temp;
            break
        end
        msg(N+1) = temp;
        N=N+1;

        % Check proper start byte
        if ((N>0) && (msg(settings.MSG_HEADER_START) ~= hex2dec(settings.MSG_START_BYTE)))
            N = 0;
        end

        % Check proper project id
        if ((N>1) && (msg(settings.MSG_HEADER_PORJ_ID) ~= hex2dec(settings.MSG_PROJECT_ID)))
            N = 0;
        end        
        
        % Check proper source id
        if ((N>2) && (~( (msg(settings.MSG_HEADER_SRC_ID) ~= hex2dec(settings.MSG_DSP_ID)) || ...
                         (msg(settings.MSG_HEADER_SRC_ID) ~= hex2dec(settings.MSG_ARM_ID)))))
            N = 0;
        end

         % Check proper destination id
         if ((N>3) && (~( (msg(settings.MSG_HEADER_DES_ID) ~= hex2dec(settings.MSG_RPI_ID)) || ...
                          (msg(settings.MSG_HEADER_DES_ID) ~= hex2dec(settings.MSG_OTHERS_ID)))))
             N = 0;
         end

         % if the packet is complete, check CRC and continue
         if ((N>=settings.MSG_LENGHT_HEADER) && ((msg(settings.MSG_HEADER_PAYLOAD_LEN)+settings.MSG_LENGHT_HEADER) == N))
             comm_print_msg('RX: ',msg,byte_per_line);
             if (crc_calculate(msg) ~= 0)
                 N = 0;
             else
                 break
             end
         end

    end
    
end
