function [  ] = menu_dsp_trends( help , settings )
    if (help == 1)
        fprintf('DSP trends\n')
    else

        % infinite loop
        fprintf('\n');
        loop = 1; % set flag for exit
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
                        data = rx_payload;
                    elseif ((i>1)&(i<(N-1)))
%                        size(rx_payload)
%                        size(data)
                        data = [data ; rx_payload ];
                    end

                end

                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % communication complete, generate trend
                trend = func_trend_generate(data);

                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % show result
                func_trend_show(trend);
                fprintf(2,'Tiempo requerido para la lectura = %6.3f s\n\n',toc)

            else
                loop = 0;
            end    

        end      
    end
end