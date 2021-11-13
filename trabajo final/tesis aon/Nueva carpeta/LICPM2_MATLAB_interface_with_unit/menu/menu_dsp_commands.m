function [  ] = menu_dsp_commands( help , settings )

    if (help == 1)
        fprintf('DSP commands\n')
    else
        
        % clear screem
        clc
        
        % start loop
        loop = 1;
        while (loop)

            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
            fprintf('Ver. 2\n');
            fprintf('Main - DSP commands\n');
            fprintf('Options:\n');

            % number of options availables (read options)
            option_N = 0;
            option_exist = 1;
            while (option_exist)
                option_N = option_N+1;
                option_exist = exist(['option',num2str(option_N+1)]);
            end
            
            % show options
            for k=1:option_N
                fprintf(['(',num2str(k,'%1.2d'),') '])        % show number
                eval(['option',num2str(k),'(1,0);']);      % show description
            end
            fprintf('(other) Return to menu \n');
            in = input('Choose an option: '); % read input as num
            
            % take action depending on selected option
            fprintf('\n');
            if ( (in>0) && (in<=option_N))
                eval(['option',num2str(in),'(0,settings);']);
            else
                loop = 0;
            end
            fprintf('\n-------------------------------------------\n');
               
        end
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% LOCAL FUNCTION

function [ ] = option1( help , settings)
    
    if (help == 1)
        fprintf('DSP - Request firmware version\n');
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '10';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );

        % response
        fprintf('\n')
        fprintf('uC firmware\n')
        fprintf('Firmware version    = %d.%d\n',rx_payload(1),rx_payload(2))
    end
    
end
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option2( help , settings)
    
    if (help == 1)
        fprintf('DSP - Send echo\n');
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '12';
        M = input('Enter length =');
        vector = randi(255,M,1);
        tx.payload = dec2hex(vector,2);
        rx_payload = comm_routine( settings , tx , 20 );
    end
    
end
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option3( help , settings)
    
    if (help == 1)
        fprintf('DSP - Send an unknow message\n');
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '5E';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );

        fprintf(['UNKNOW COMMAND, ID =',dec2hex(rx_payload),'\n'])
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option4( help , settings)
    
    if (help == 1)
        fprintf('DSP - Unit Config - Set with custom parameters\n')
    else
        
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '30';
        uc(1) = bin2dec(['000000000000000', ...
                                        '1', ... % SD-Card discart oldest months
                                        '1', ... % Event enable
                                        '0', ... % Trend 5 minute enable 
                                        '0', ... % Trend 3 second enable
                                    '00001', ... % Controller
                                     '0010', ... % 0000 = Grid / 0001 = low / 0010 = Emu
                                     '0001']);   % 0000 = VSPT / 0001 = FSPT
        uc(2) = bin2dec(['000000000000000000000', ...
                                           '000', ... % over current enable (I1,I2,I3)
                                             '0', ... % under frequency enable
                                             '0', ... % over frequency enable
                                           '111', ... % under voltage enable (V1,V2,V3)
                                           '111']);   % over voltage enable (V1,V2,V3)
        uc(3) = 750000;
        uc(4:6) = floor([+1.730432838898867e-05  -1.726021465392558e-05 0]*150e6*2^10);
        uc(7:8) = floor([+20e-6 -20e-6]*150e6*2^5);
        uc(9) = 400;
        uc(10) = 400;
        uc(11) = 400;
        uc(12) = 8*1024;
        uc(13) = (250 * 2^15 / 400)^2;
        uc(14) = (200 * 2^15 / 400)^2;
        uc(15) = (150 * 2^15 / 400)^2;
        uc(16) = 52;
        uc(17) = 48;
        uc_1 = dec2hex(floor(typecast(int32(uc),'uint32')),8);
        for i=1:length(uc_1)
            tx.payload(4*i-3,:) = uc_1(i,7:8);
            tx.payload(4*i-2,:) = uc_1(i,5:6);
            tx.payload(4*i-1,:) = uc_1(i,3:4);
            tx.payload(4*i  ,:) = uc_1(i,1:2);
        end
        rx_payload = comm_routine( settings , tx , 20 ); 

        % check proper response
        if isempty(rx_payload)
            return;
        end
        
        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option5( help , settings)
    
    if (help == 1)
        fprintf('DSP - Unit Config - Set to default\n')
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '34';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 ); 
        
        % check proper response
        if isempty(rx_payload)
            return;
        end
        
        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option6( help , settings)
    
    if (help == 1)
        fprintf('DSP - Unit Config - Get parameters\n')
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '32';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 ); 

        % check proper response
        if isempty(rx_payload)
            return;
        end
        
        % responde
        fprintf('\n')
        % unir datos de 8 bits en 32 bits
        for i=1:(length(rx_payload)/4)
            data_p(i) = rx_payload(i*4-3) + rx_payload(i*4-2)*2^8 + rx_payload(i*4-1)*2^16 + rx_payload(i*4)*2^24;
        end
        % show
        fprintf('Unit Configuration\n')
        fprintf('mode flag = %s\n',dec2bin(data_p(1),32))
        fprintf('event flag = %s\n',dec2bin(data_p(2),32))
        fprintf('Ts ticks Q5 = %d (%6.2f us)\n',data_p(3),data_p(3)/150/2^5)
        fprintf('Controller Q10 = %d | %d | %d\n',data_p(4),data_p(5),data_p(6))
        fprintf('Ts ticks Q5 MAX | MIN = %d | %d\n',data_p(7),data_p(8))
        fprintf('Gain Voltage Channel = %d\n',data_p(9))
        fprintf('Gain Current Channel = %d\n',data_p(10))
        fprintf('SD-Card - Mrmory capacity = %d MB\n',data_p(11))
        fprintf('event thr RMS2 over voltage = %d\n',data_p(12))
        fprintf('event thr RMS2 under voltage = %d\n',data_p(13))
        fprintf('event thr RMS2 over current = %d\n',data_p(14))
        fprintf('event thr RMS2 over freq = %d Hz\n',data_p(15))
        fprintf('event thr RMS2 under freq = %d Hz\n',data_p(16))
    
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option7( help , settings)
    
    if (help == 1)
        fprintf('DSP - Unit Calibration - Set with custom parameters\n');
    else
        
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '36';
        uc(1:12)  = 32768 * [-1 -1 -1 -1 +1 +1 +1 +1 +1 +1 +1 +1];
        uc(13:24) = 32768 * [-1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1]; 
        uc_1 = dec2hex(floor(typecast(int32(uc),'uint32')),8);
        for i=1:length(uc_1)
            tx.payload(4*i-3,:) = uc_1(i,7:8);
            tx.payload(4*i-2,:) = uc_1(i,5:6);
            tx.payload(4*i-1,:) = uc_1(i,3:4);
            tx.payload(4*i  ,:) = uc_1(i,1:2);
        end
        rx_payload = comm_routine( settings , tx , 20 );

        % check proper response
        if isempty(rx_payload)
            return;
        end
        
        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option8( help , settings)
    
    if (help == 1)
        fprintf('DSP - Unit Calibration - Set to default\n');
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '3A';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option9( help , settings)
    
    if (help == 1)
        fprintf('DSP - Unit Calibration - Get parameters\n');
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '38';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % check proper response
        if isempty(rx_payload)
            return;
        end
        
        % responde
        fprintf('\n')
        % unir datos de 8 bits en 32 bits
        for i=1:(length(rx_payload)/4)
            data_p(i) = rx_payload(i*4-3) + rx_payload(i*4-2)*2^8 + rx_payload(i*4-1)*2^16 + rx_payload(i*4)*2^24;
        end
        % show
        fprintf('Unit Calibration\n')
        fprintf('Signal | gain    | offset |\n')
        fprintf('V1_N   | %7d |%7d |\n',typecast(uint32([data_p(1) ,data_p(13)]),'int32'))
        fprintf('V2_N   | %7d |%7d |\n',typecast(uint32([data_p(2) ,data_p(14)]),'int32'))
        fprintf('V3_N   | %7d |%7d |\n',typecast(uint32([data_p(3) ,data_p(15)]),'int32'))
        fprintf('VE_N   | %7d |%7d |\n',typecast(uint32([data_p(4) ,data_p(16)]),'int32'))
        fprintf('I1A    | %7d |%7d |\n',typecast(uint32([data_p(5) ,data_p(17)]),'int32'))
        fprintf('I2A    | %7d |%7d |\n',typecast(uint32([data_p(6) ,data_p(18)]),'int32'))
        fprintf('I3A    | %7d |%7d |\n',typecast(uint32([data_p(7) ,data_p(19)]),'int32'))
        fprintf('IN     | %7d |%7d |\n',typecast(uint32([data_p(8) ,data_p(20)]),'int32'))
        fprintf('I1B    | %7d |%7d |\n',typecast(uint32([data_p(9) ,data_p(21)]),'int32'))
        fprintf('I2B    | %7d |%7d |\n',typecast(uint32([data_p(10),data_p(22)]),'int32'))
        fprintf('I3B    | %7d |%7d |\n',typecast(uint32([data_p(11),data_p(23)]),'int32'))
        fprintf('IE     | %7d |%7d |\n',typecast(uint32([data_p(12),data_p(24)]),'int32'))
    
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option10( help , settings)
    
    if (help == 1)
        fprintf('DSP - RTC - Get time and date\n');
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '52';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % responde
        fprintf('\n')
        % unir datos de 8 bits en 32 bits
        for i=1:(length(rx_payload)/2)
            data_p(i) = rx_payload(i*2-1) + rx_payload(i*2)*2^8;
        end
        % guardar datos con formato
        k = 1;
        rtc.msecond = data_p(k);      k=k+1;
        rtc.second  = data_p(k);      k=k+1;
        rtc.minute  = data_p(k);      k=k+1;
        rtc.hour    = data_p(k);      k=k+1;
        % show
        fprintf('RTC\n');
        fprintf('time %2d:%2d:%2d %4d\n', rtc.hour , rtc.minute , rtc.second , rtc.msecond );
    
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option11( help , settings)
    
    if (help == 1)
        fprintf('DSP - RTC - Set with MatLab time\n');
    else
        
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '50';
        matlab_t = clock;
        time_1 = floor([0 , floor(matlab_t(6:-1:4))]);
        time_2 = dec2hex(time_1,4);
        for i=1:size(time_2,1)
            tx.payload(2*i-1,:) = time_2(i,3:4);
            tx.payload(2*i  ,:) = time_2(i,1:2);
        end
        rx_payload = comm_routine( settings , tx , 20 );

        % check proper response
        if isempty(rx_payload)
            return;
        end         
        
        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option12( help , settings)
    
    if (help == 1)
        fprintf('DSP - Request communication flags\n');
    else
        
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '14';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );

        % responde
        data_p = dec2bin( rx_payload(1) + rx_payload(2)*2^8 , 16)
        fprintf('DSP communication flags\n')
        fprintf(['Send RTC                 ',data_p(14),'\n']);
        fprintf(['Send Unit Config         ',data_p(13),'\n']);
        fprintf(['Send Unit Calibration    ',data_p(12),'\n']);
        fprintf(['Request Trend 150 cycles ',data_p(11),'\n']);
        fprintf(['Request Trend 5 minutes  ',data_p(10),'\n']);
        fprintf(['Request Telemetry        ',data_p(9),'\n']);
        fprintf(['Request event buffer     ',data_p(8),'\n']);
        fprintf(['Request event det info   ',data_p(7),'\n']);

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option13( help , settings)
    
    if (help == 1)
        fprintf('DSP - Request telemetry (used in calibration)\n');
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '74';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % check proper response
        if isempty(rx_payload)
            return;
        end       
        
        % responde
        fprintf('\n')       
        % unir datos de 8 bits en 16 bits
        for i=1:(length(rx_payload)/2)
            data_p(i) = rx_payload(i*2-1) + rx_payload(i*2)*2^8;
        end
        % data
        fprintf('DSP Telemetria\n')
        fprintf('V1 = %6.2f | %8.4f\n',(400/2^15)*data_p(1),(400/2^15)*double(typecast(uint16(data_p( 9)),'int16')));
        fprintf('V2 = %6.2f | %8.4f\n',(400/2^15)*data_p(2),(400/2^15)*double(typecast(uint16(data_p(10)),'int16')));
        fprintf('V3 = %6.2f | %8.4f\n',(400/2^15)*data_p(3),(400/2^15)*double(typecast(uint16(data_p(11)),'int16')));
        fprintf('VE = %6.2f | %8.4f\n',(400/2^15)*data_p(4),(400/2^15)*double(typecast(uint16(data_p(12)),'int16')));
        fprintf('I1 = %6.2f | %8.4f\n',(400/2^15)*data_p(5),(400/2^15)*double(typecast(uint16(data_p(13)),'int16')));
        fprintf('I2 = %6.2f | %8.4f\n',(400/2^15)*data_p(6),(400/2^15)*double(typecast(uint16(data_p(14)),'int16')));
        fprintf('I3 = %6.2f | %8.4f\n',(400/2^15)*data_p(7),(400/2^15)*double(typecast(uint16(data_p(15)),'int16')));
        fprintf('IN = %6.2f | %8.4f\n',(400/2^15)*data_p(8),(400/2^15)*double(typecast(uint16(data_p(16)),'int16')));
    
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option14( help , settings)
    
    if (help == 1)
        fprintf('DSP - Request event detection info (DSP waiting)\n');
    else
        % request
        tx.dest_id = settings.MSG_DSP_ID;
        tx.msg_id = '7A';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % check proper response
        if isempty(rx_payload)
            return;
        end  
        
        % responde
        fprintf('\n')       
        % unir datos de 8 bits en 16 bits
        k = 3;
        info.type                = rx_payload(k) + rx_payload(k+1)*2^8; k=k+2;
        info.duration_dsp_ticks  = rx_payload(k) + rx_payload(k+1)*2^8 + rx_payload(k+2)*2^16 + rx_payload(k+3)*2^24; k=k+4;
        info.magnitud            = rx_payload(k) + rx_payload(k+1)*2^8; k=k+2;
        info.msecond             = rx_payload(k) + rx_payload(k+1)*2^8; k=k+2;
        info.second              = rx_payload(k) + rx_payload(k+1)*2^8; k=k+2;
        info.minute              = rx_payload(k) + rx_payload(k+1)*2^8; k=k+2;
        info.hour                = rx_payload(k) + rx_payload(k+1)*2^8; k=k+2;
        % show
        fprintf('Event detection\n');
        fprintf('type = %d\n', info.type );
        fprintf('duration = %f s\n', info.duration_dsp_ticks/150e6 );
        fprintf('magnitud = %d\n', info.magnitud );
        fprintf('time %02d:%02d:%02d %4d\n', info.hour , info.minute , info.second , info.msecond );
        
    end
    
end
