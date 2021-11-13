function [  ] = menu_arm_commands( help , settings )

    if (help == 1)
        fprintf('ARM commands\n')
    else
        
        % clear screem
        clc
        
        % start loop
        loop = 1;
        while (loop)

            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
            fprintf('Ver. 2\n');
            fprintf('Main - ARM commands\n');
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
        fprintf('ARM - Request firmware version\n');
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '02';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
          
        % check proper response
        if isempty(rx_payload)
            return;
        end
        
        % response
        fprintf('\n')
        fprintf('uC firmware\n')
        fprintf('Firmware version = %d.%d\n',rx_payload(1),rx_payload(2))
        fprintf('Compilate date   = %s\n',rx_payload(3:14))
        fprintf('Compilate time   = %s\n',rx_payload(15:23))
        
    end
    
end
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option2( help , settings)
    
    if (help == 1)
        fprintf('ARM - Send echo\n');
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '04';
        M = input('Enter length =');
        vector = randi(255,M,1);
        tx.payload = dec2hex(vector,2);
        rx_payload = comm_routine( settings , tx , 20 );
        
    end
    
end
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option3( help , settings)
    
    if (help == 1)
        fprintf('ARM - Send an unknow message\n');
    else
        % request      
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '5E';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        fprintf(['UNKNOW COMMAND, ID =',dec2hex(rx_payload),'\n'])
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option4( help , settings)
    
    if (help == 1)
        fprintf('ARM - Unit Config - Set with custom parameters\n')
    else
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '20';
        uc(1) = bin2dec(['000000000000000', ...
                                        '0', ... % SD-Card discart oldest months
                                        '0', ... % Event enable
                                        '0', ... % Trend 5 minute enable 
                                        '0', ... % Trend 3 second enable
                                    '00001', ... % Controller
                                     '0010', ... % 0000 = Grid / 0001 = low / 0010 = Emu
                                     '0001']);   % 0000 = VSPT / 0001 = FSPT
        uc(2) = bin2dec(['000000000000000000000', ...
                                           '000', ... % over current enable (I1,I2 and I3)
                                             '0', ... % under frequency enable
                                             '0', ... % over frequency enable
                                           '111', ... % under voltage enable (V1,V2 and V3)
                                           '111']);   % over voltage enable (V1,V2 and V3)
        uc(3) = 750000;
        uc(4:6) = floor([+1.730432838898867e-05  -1.726021465392558e-05 0]*150e6*2^10);
        uc(7:8) = floor([+30e-6 -30e-6]*150e6*2^5);
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
        fprintf('ARM - Unit Config - Set to default\n')
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '24';
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
        fprintf('ARM - Unit Config - Get parameters\n')
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '22';
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
        fprintf('ARM - Unit Calibration - Set with custom parameters\n');
    else
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '26';
        uc(1:8) = [ -32768 , -32768 , -32768 , -32768 , 32768 , 32768 , 32768 , 32768 ];
        uc(9:16) = [-32768 , -32768 , -32768 , -32768 , -32768 , -32768 , -32768 , -32768 ]; 
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
        fprintf('ARM - Unit Calibration - Set to default\n');
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '2A';
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

function [ ] = option9( help , settings)
    
    if (help == 1)
        fprintf('ARM - Unit Calibration - Get parameters\n');
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '28';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );        
        
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
    
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option10( help , settings)
    
    if (help == 1)
        fprintf('ARM - RTC - Get time and date\n');
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '42';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % responde
        fprintf('\n')
        % unir datos de 8 bits en 32 bits
        for i=1:(length(rx_payload)/4)
            data_p(i) = rx_payload(i*4-3) + rx_payload(i*4-2)*2^8 + rx_payload(i*4-1)*2^16 + rx_payload(i*4)*2^24;
        end
        % guardar datos con formato
        k = 1;
        rtc.year   = data_p(k);      k=k+1;
        rtc.month  = data_p(k);      k=k+1;
        rtc.day    = data_p(k);      k=k+1;
        rtc.week   = data_p(k);      k=k+1;
        rtc.hour   = data_p(k);      k=k+1;
        rtc.minute = data_p(k);      k=k+1;
        rtc.second = data_p(k);      k=k+1;
        % show
        week = ['Dom';'Lun';'Mar';'Mie';'Jue';'Vie';'Sab'];
        fprintf('RTC\n');
        fprintf('date %4d/%2d/%2d', rtc.year , rtc.month , rtc.day );
        fprintf([' ',week(rtc.week,:),'\n']);
        fprintf('time %2d:%2d:%2d\n', rtc.hour , rtc.minute , rtc.second );
    
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option11( help , settings)
    
    if (help == 1)
        fprintf('ARM - RTC - Set with MatLab time\n');
    else
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '40';
        matlab_t = clock;
        time_1 = floor([matlab_t(1:3) , weekday(date) , matlab_t(4:6)]);
%        time_1 = [2017 12 31 1 23 58 0];
        time_2 = dec2hex(time_1,8);
        for i=1:size(time_2,1)
            tx.payload(4*i-3,:) = time_2(i,7:8);
            tx.payload(4*i-2,:) = time_2(i,5:6);
            tx.payload(4*i-1,:) = time_2(i,3:4);
            tx.payload(4*i  ,:) = time_2(i,1:2);
        end
        rx_payload = comm_routine( settings , tx , 20 );

        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option12( help , settings)
    
    if (help == 1)
        fprintf('ARM - Request unique ID\n');
    else
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '06';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % unir datos de 8 bits en 32 bits
        for i=1:(length(rx_payload)/4)
            data_p(i) = rx_payload(i*4-3) + rx_payload(i*4-2)*2^8 + rx_payload(i*4-1)*2^16 + rx_payload(i*4)*2^24;
        end
        
        dec2hex(data_p,8)
        
        
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option13( help , settings)
    
    if (help == 1)
        fprintf('ARM - Reset ARM\n');
    else
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '08';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%