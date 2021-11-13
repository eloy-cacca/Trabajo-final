
function [ ] = menu_rtc_test( help , settings )

    if (help == 1)
        fprintf('RTC test\n')
    else
        
        % clear screen
        clc;
        
        % close port
        if strcmp( settings.type , 'UART')
            serialport_close(settings.serial_port)
        end

        % show name
        fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
        fprintf('Ver. 2\n');
        fprintf('Main - RTC test\n');
        fprintf('Options:\n');
        
        % infinite loop
        time_second_old = 0;
        
        while(1)
        
            fprintf('\n')
            
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % wait time
            wait = 1;
            while (wait)
                matlab_t = clock;
                time_second = floor( matlab_t(6) ) / 10;                       % cada 10 segundos
                %time_second = floor( matlab_t(5)*60 + matlab_t(6) ) / 60;      % cada 1 minuto
                %time_second = floor( matlab_t(5)*60 + matlab_t(6) ) / (60*10);  % cada 10 minuto
                if (time_second ~= time_second_old)
                    if ( time_second == floor(time_second))
                        wait = 0;
                        time_second_old = time_second;
                    end
                end
            end

            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
            % init serial port
            if strcmp( settings.type , 'UART')
                settings.serial_port = serialport_open( settings );
            end

            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % print matlab time
            matlab_t = clock;
            matlab_second = floor(matlab_t(6));
            fprintf('MATLAB');
            fprintf('  %02d/%02d/%4d', matlab_t(3) , matlab_t(2) , matlab_t(1) );
            fprintf(' %02d:%02d:%02d\n', matlab_t(4) , matlab_t(5) , matlab_second );

            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%        
            % request ARM RTC
            tx.dest_id = settings.MSG_ARM_ID;
            tx.msg_id = '42';
            tx.payload = [];
            rx_payload = comm_routine( settings , tx , 0 );

            % responde
            % unir datos de 8 bits en 32 bits
            for i=1:(length(rx_payload)/4)
                data_p(i) = rx_payload(i*4-3) + rx_payload(i*4-2)*2^8 + rx_payload(i*4-1)*2^16 + rx_payload(i*4)*2^24;
            end
            % guardar datos con formato
            k = 1;
            armrtc.year   = data_p(k);      k=k+1;
            armrtc.month  = data_p(k);      k=k+1;
            armrtc.day    = data_p(k);      k=k+1;
            armrtc.week   = data_p(k);      k=k+1;
            armrtc.hour   = data_p(k);      k=k+1;
            armrtc.minute = data_p(k);      k=k+1;
            armrtc.second = data_p(k);      k=k+1;

            % show datas
            fprintf('ARM RTC');
            fprintf(' %02d/%02d/%4d', armrtc.day , armrtc.month , armrtc.year );        
            fprintf(' %02d:%02d:%02d\n', armrtc.hour , armrtc.minute , armrtc.second );

            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % request DSP RTC
            tx.dest_id = settings.MSG_DSP_ID;
            tx.msg_id = '52';
            tx.payload = [];
            rx_payload = comm_routine( settings , tx , 0 );
                     
            % response
            % unir datos de 8 bits en 32 bits
            for i=1:(length(rx_payload)/2)
                data_p(i) = rx_payload(i*2-1) + rx_payload(i*2)*2^8;
            end
            % guardar datos con formato
            k = 1;
            dsprtc.msecond = data_p(k);      k=k+1;
            dsprtc.second  = data_p(k);      k=k+1;
            dsprtc.minute  = data_p(k);      k=k+1;
            dsprtc.hour    = data_p(k);      k=k+1;
            % show datas
            fprintf('DSP RTC');
            fprintf(' %02d/%02d/%4d', armrtc.day , armrtc.month , armrtc.year );
            fprintf(' %02d:%02d:%02d\n', dsprtc.hour , dsprtc.minute , dsprtc.second);

            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % close port
            if strcmp( settings.type , 'UART')
                serialport_close(settings.serial_port)
            end
        end
    end  
end