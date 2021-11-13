function [  ] = menu_dsp_calibration( help , settings)
    if (help == 1)
        fprintf('DSP Calibration\n')
    else
        % clear screen
        clc;
        
        % infinite loop
        loop = 1;
        while (loop)

            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB01\n');
            fprintf('Ver. 1\n');
            fprintf('Main -> DSP Calibration\n');
            fprintf('Options: \n');

            % show options
            fprintf('Telemetry = 00\n');
            fprintf('Gain   = 01:V1 , 02:V2 , 03:V3 , 04:VE , 05:I1 , 06:I2 , 07:I3 , 08:IN \n');
            fprintf('Offset = 09:V1 , 10:V2 , 11:V3 , 12:VE , 13:I1 , 14:I2 , 15:I3 , 16:IN \n');
            fprintf('(other) Exit \n');
            in = input('Choose an option: '); % read input as num
            fprintf('\n');

            % take action depending on selected option
            if ((in > 0) && (in < 17))

                % wait user
                ticks = input('Set ticks: ');

                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % Get unit calibration
                % request
                tx.dest_id = settings.MSG_ARM_ID;
                tx.msg_id = '28';
                tx.payload = [];
                rx_payload = comm_routine( settings , tx , 20 );
                % unir datos de 8 bits en 32 bits
                for i=1:(length(rx_payload)/4)
                    unit_cal(i) = rx_payload(i*4-3) + rx_payload(i*4-2)*2^8 + rx_payload(i*4-1)*2^16 + rx_payload(i*4)*2^24;
                end

                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % set new paramater
                unit_cal(in) = unit_cal(in) + ticks;
                  
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % write new unit calibration
                tx.dest_id = settings.MSG_ARM_ID;
                tx.msg_id = '26';
                payload = dec2hex(unit_cal,8);
                for i=1:length(payload)
                    tx.payload(4*i-3,:) = payload(i,7:8);
                    tx.payload(4*i-2,:) = payload(i,5:6);
                    tx.payload(4*i-1,:) = payload(i,3:4);
                    tx.payload(4*i  ,:) = payload(i,1:2);
                end
                rx_payload = comm_routine( settings , tx , 0 );
                % responde
                if (rx_payload(1) == 0) fprintf('OK\n')
                else                    fprintf('ERROR = %d\n',rx_payload(1))
                end
                fprintf('\n')
            
            elseif (in == 0)
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % Showcalibration
                % request
                tx.dest_id = settings.MSG_ARM_ID;
                tx.msg_id = '28';
                tx.payload = [];
                rx_payload = comm_routine( settings , tx , 0 );        
                % responde
                fprintf('\n')
                % unir datos de 8 bits en 32 bits
                for i=1:(length(rx_payload)/4)
                    data_p(i) = rx_payload(i*4-3) + rx_payload(i*4-2)*2^8 + rx_payload(i*4-1)*2^16 + rx_payload(i*4)*2^24;
                end
                data_p2 = typecast(uint32(data_p),'int32');
                % show
                fprintf('Unit Calibration\n')
                fprintf('ADC gain = %d | %d | %d | %d | %d | %d | %d | %d \n',data_p2(1:8))
                fprintf('ADC offset = %d | %d | %d | %d | %d | %d | %d | %d \n',data_p2(9:16))

                % telemetry
                % request
                tx.dest_id = settings.MSG_DSP_ID;
                tx.msg_id = '74';
                tx.payload = [];
                rx_payload = comm_routine( settings , tx , 0 );
                % responde
                fprintf('\n')       
                % unir datos de 8 bits en 16 bits
                for i=1:length(rx_payload)/2
                    data_p(i) = double(typecast(uint16(rx_payload(i*2-1) + rx_payload(i*2)*2^8),'int16'));
                end
                % data
                fprintf('DSP Telemetria\n')
                fprintf('       Gain            | Offset(real)      | Offset (imag)\n')
                fprintf('V1 = %8.4f (%6d) | %8.4f (%6d) | %8.4f (%6d)\n', [(400/2^15) 1]*data_p(1) , [(400/2^15) 1]*data_p( 9) , [0 0]);
                fprintf('V2 = %8.4f (%6d) | %8.4f (%6d) | %8.4f (%6d)\n', [(400/2^15) 1]*data_p(2) , [(400/2^15) 1]*data_p(10) , [0 0]);
                fprintf('V3 = %8.4f (%6d) | %8.4f (%6d) | %8.4f (%6d)\n', [(400/2^15) 1]*data_p(3) , [(400/2^15) 1]*data_p(11) , [0 0]);
                fprintf('VE = %8.4f (%6d) | %8.4f (%6d) | %8.4f (%6d)\n', [(400/2^15) 1]*data_p(4) , [(400/2^15) 1]*data_p(12) , [0 0]);
                fprintf('I1 = %8.4f (%6d) | %8.4f (%6d) | %8.4f (%6d)\n', [(1/2^15)   1]*data_p(5) , [(1/2^15)   1]*data_p(13) , [0 0]);
                fprintf('I2 = %8.4f (%6d) | %8.4f (%6d) | %8.4f (%6d)\n', [(1/2^15)   1]*data_p(6) , [(1/2^15)   1]*data_p(14) , [0 0]);
                fprintf('I3 = %8.4f (%6d) | %8.4f (%6d) | %8.4f (%6d)\n', [(1/2^15)   1]*data_p(7) , [(1/2^15)   1]*data_p(15) , [0 0]);
                fprintf('IN = %8.4f (%6d) | %8.4f (%6d) | %8.4f (%6d)\n', [(1/2^15)   1]*data_p(8) , [(1/2^15)   1]*data_p(16) , [0 0]);      
                fprintf('\n');
            else
                loop = 0;
            end    

        end
    end
end
