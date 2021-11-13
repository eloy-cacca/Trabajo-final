function [  ] = menu_dsp_events( help , settings )
    if (help == 1)
        fprintf('DSP events\n')
    else

        % infinite loop
        fprintf('\n');
        loop = 1; % set flag for exit
        while (loop)

            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
            fprintf('Ver. 1\n');
            fprintf('Main -> DSP Event\n');
            fprintf('Options:\n');
            % show options
            fprintf('(01) Start\n');
            fprintf('(other) Exit \n');
            in = input('Choose an option: '); % read input as num
            fprintf('\n');        
            
            % take action depending on selected option
            if (in==1)
                
                tic
                
                % send commands
                N = 1024;
                for i=[0:N+1]
                    
                    % show progress
                    if (mod(i,20)==0)
                        fprintf('%d of %d (t = %7.3fsec)\n',i,N,toc)
                    end
                    
                    % send command
                    tx.dest_id = settings.MSG_DSP_ID;
                    tx.msg_id = '78';
                    tx.payload = [];
                    i_HEX = dec2hex(floor(i),4);
                    tx.payload = [i_HEX(3:4);i_HEX(1:2)];
                    rx_payload = comm_routine( settings , tx , 20 );

                    % unir datos de 8 bits en 16 bits
                    for k=1:(length(rx_payload)/2)
                        data_p(k) = rx_payload(k*2-1) + rx_payload(k*2)*2^8;
                    end
                    
                    % response
                    if (i==0)
                        
                        % guardar datos con formato
                        k = 1;
                        event_info.start  = data_p(k);      k=k+1;
                        event_info.stop   = data_p(k);      k=k+1;
                        event_info.div    = data_p(k);      k=k+1;

                    elseif ( (i>0) & (i<(N+1)) )
                        data(i,:) = double(typecast(uint16(data_p'),'int16'));
                    elseif ( i == N+1)
                        fprintf('OK\n')                        
                    else
                        % none for now 
                    end
                
                end
                
                event_info
             %   data
                if (event_info.div == 1)
                    subplot(3,2,1); plot(data(1:512,1)); ylabel('counter')
                    subplot(3,2,2); plot(data(513:1024,1)); ylabel('counter')
                    subplot(3,2,3); plot(400/2^15*data(1:512,2:5)); ylabel('voltage [V]')
                    subplot(3,2,4); plot(400/2^15*data(513:1024,2:5)); ylabel('voltage [V]')
                    subplot(3,2,5); plot(200/2^15*data(1:512,6:9)); ylabel('current [A]')
                    subplot(3,2,6); plot(200/2^15*data(513:1024,6:9)); ylabel('current [A]')
                else                    
                    subplot(3,1,1); plot(data(:,1)); ylabel('counter')
                    subplot(3,1,2); plot(400/2^15*data(:,2:5)); ylabel('voltage [V]')
                    subplot(3,1,3); plot(200/2^15*data(:,6:9)); ylabel('current [A]')
                end
                %axis([0 1024 -2^15 2^15])
                

                
            else
                loop = 0;
            end 
        end
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [  ] = process_data_and_show_result ( data )

    data_i = 1;
    for i=1:size(data,1)
        for k=0:3
            data_p1(data_i,:) = data(i, 1+18*k : 18*(1+k));
            data_i = data_i+1;
        end
    end
    for i=1:size(data_p1,1)
        for k=1:9
            data_p2(i,k) = double(typecast(uint16(data_p1(i,k*2-1) + data_p1(i,k*2)*2^8),'int16'));
        end
    end

    subplot(3,1,1); plot(data_p2(:,1)); ylabel('counter')
    subplot(3,1,2); plot(400/2^15*data_p2(:,2:5)); ylabel('voltage [V]')
    subplot(3,1,3); plot(200/2^15*data_p2(:,6:9)); ylabel('current [A]')
    
end