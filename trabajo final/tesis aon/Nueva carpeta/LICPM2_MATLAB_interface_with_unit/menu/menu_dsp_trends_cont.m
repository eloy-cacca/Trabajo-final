function [  ] = menu_dsp_trends_cont( help , settings )
    if (help == 1)
        fprintf('DSP trends continuo (temporal para debug)\n')
    else

        % infinite loop
        fprintf('\n');
        loop = 1; % set flag for exit
        N_buffer = 100;
        while (loop)

            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
            fprintf('Ver. 1\n');
            fprintf('Main -> DSP Trend\n');
            fprintf('Options: \n');
            % show options
            fprintf('(01) Trend 150 cycles\n');
            fprintf('(02) Trend 5 minutes\n');    
            fprintf('(other) Exit \n');
            in = input('Choose an option: '); % read input as num
            fprintf('\n');
            
            buffer_rms = zeros(3,N_buffer);
            buffer_THD = zeros(3,N_buffer);
            buffer_PWR = zeros(3,N_buffer);
            t = [1:1:N_buffer];
            while(1)
                tic

                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % take action depending on selected option
                if (in~=0)     

                    if (in==1)
                        id = '70';
                    elseif (in==2)
                        id = '72';
                    end

                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    % send all request and process response
                    N = 1 + 9 + 1 + 1;
                    for i=0:(N-1)

                        % send request                        
                        tx.dest_id = settings.MSG_DSP_ID;
                        tx.msg_id = id;
                        i_HEX = dec2hex(floor(i),4);
                        tx.payload = [i_HEX(3:4);i_HEX(1:2)];
                        rx_payload = comm_routine( settings , tx , 20 );
                        
                        % depending on message, add data into buffer.
                        if (i==1)
                            data = rx_payload';
                        elseif ((i>1)&(i<(N-1)))
                            data = [data ; rx_payload' ];
                        end

                    end

                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    % communication complete, generate trend
                    trend = func_trend_generate(data);

                    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                    % show result
                    %func_trend_show(trend);
                    fprintf(2,'Tiempo requerido para la lectura = %6.3f s\n\n',toc)
                    
                    buffer_rms(1,:) = [buffer_rms(1,2:end) , trend.ch(1).rms_AVG];
                    buffer_rms(2,:) = [buffer_rms(2,2:end) , trend.ch(2).rms_AVG];
                    buffer_rms(3,:) = [buffer_rms(3,2:end) , trend.ch(3).rms_AVG];
                    subplot(3,1,1);
                    plot(t,buffer_rms(1,:)','r',t,buffer_rms(2,:)','b',t,buffer_rms(3,:)','g','LineWidth',1.5)
                    axis([1 N_buffer 210 240]); grid on

                    buffer_THD(1,:) = [buffer_THD(1,2:end) , trend.ch(1).TWD_AVG];
                    buffer_THD(2,:) = [buffer_THD(2,2:end) , trend.ch(2).TWD_AVG];
                    buffer_THD(3,:) = [buffer_THD(3,2:end) , trend.ch(3).TWD_AVG];
                    subplot(3,1,2);
                    plot(t,buffer_THD(1,:)','r',t,buffer_THD(2,:)','b',t,buffer_THD(3,:)','g','LineWidth',1.5)
                    axis([1 N_buffer 0 6]); grid on
                    
                    buffer_PWR(1,:) = [buffer_PWR(1,2:end) , trend.ch_power(1).RealPower_AVG ];
                    buffer_PWR(2,:) = [buffer_PWR(2,2:end) , trend.ch_power(2).RealPower_AVG];
                    buffer_PWR(3,:) = [buffer_PWR(3,2:end) , trend.ch_power(3).RealPower_AVG];
                    subplot(3,1,3);
                    plot(t,buffer_PWR(1,:)','r',t,buffer_PWR(2,:)','b',t,buffer_PWR(3,:)','g','LineWidth',1.5)
                    axis([1 N_buffer 0 18000]); grid on
                    
                    drawnow;
                    
                    while (toc<3)
                    end

                else
                    loop = 0;
                end   
            end

        end      
    end
end