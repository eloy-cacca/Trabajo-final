function [  ] = menu_arm_commands_2( help , settings )

    if (help == 1)
        fprintf('ARM commands 2 (SD-Card)\n')
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
        fprintf('ARM - SD-Card - Open directory\n')
    else
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '60';
        in = input('Directory Name (none = root): ','s');
        in(end+1) = 0; % add null character
        tx.payload = dec2hex(uint8(in));
        rx_payload = comm_routine( settings , tx , 20 );

        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option2( help , settings)
    
    if (help == 1)
        fprintf('ARM - SD-Card - Read item form open directory\n')
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '62';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % responde
        fprintf('\n')
        fprintf('NAME = %s\n',char(rx_payload(10:24)))
        fprintf('ATTRIBUTE = %s',dec2bin(rx_payload(9),8))
        if bitand( 2^4 , rx_payload(9) ) fprintf('(subdirectory)\n');
        elseif bitand( 2^5 , rx_payload(9) ) fprintf('(archive)\n');
        else fprintf('(none)\n'); end
        fprintf('SIZE = %d bytes\n',rx_payload(1) + rx_payload(2)*2^8 + rx_payload(3)*2^16 + rx_payload(4)*2^24)
    end

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option3( help , settings)
    
    if (help == 1)
        fprintf('ARM - SD-Card - Read LFN form last item\n')
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '64';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % responde
        fprintf('\n')
        fprintf('NAME = %s\n',char(rx_payload(1:end)))
    
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option4( help , settings)
    
    if (help == 1)
        fprintf('ARM - SD-Card - Open file\n')
    else
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '66';
        in = input('File Name (use "/" for folders): ','s');
        in(end+1) = 0; % add null character
        tx.payload = dec2hex(uint8(in));
        rx_payload = comm_routine( settings , tx , 20 );

        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option5( help , settings)
    
    if (help == 1)
        fprintf('ARM - SD-Card - Read file\n')
    else
        
        in = input('Enter location (ej: 0, 1, 2, etc): ');
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '68';
        in_1 = dec2hex(floor(typecast(int32(in),'uint32')),8);
        for i=1:size(in_1,1)
            tx.payload(4*i-3,:) = in_1(i,7:8);
            tx.payload(4*i-2,:) = in_1(i,5:6);
            tx.payload(4*i-1,:) = in_1(i,3:4);
            tx.payload(4*i  ,:) = in_1(i,1:2);
        end
        rx_payload = comm_routine( settings , tx , 20 );

        fprintf('\n------------------------\n')
        fprintf('BYTE READ = %d bytes',rx_payload(1) + rx_payload(2)*2^8 + rx_payload(3)*2^16 + rx_payload(4)*2^24)
        fprintf('\n------------------------\n')
        fprintf('%s',char(rx_payload(5:end)))
        fprintf('\n------------------------\n')

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option6( help , settings)
    
    if (help == 1)
        fprintf('ARM - SD-Card - Close file\n')
    else
        
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '6A';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );

        % responde
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1))
        end

    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function [ ] = option7( help , settings)
    
    if (help == 1)
        fprintf('ARM - SD-Card - Request open file size\n')
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '6C';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % responde
        fprintf('SIZE = %d bytes\n',rx_payload(1) + rx_payload(2)*2^8 + rx_payload(3)*2^16 + rx_payload(4)*2^24)
    end

end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


function [ ] = option8( help , settings)
    
    if (help == 1)
        fprintf('ARM - SD-Card - Request total and free Mbytes\n')
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = 'A0';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
        
        % responde
        total = rx_payload(1) + rx_payload(2)*2^8 + rx_payload(3)*2^16 + rx_payload(4)*2^24;
        free = rx_payload(5) + rx_payload(6)*2^8 + rx_payload(7)*2^16 + rx_payload(8)*2^24;
        fprintf('\n');
        fprintf('TOTAL = %5d MB (%6.2f GB)\n',total,total/1024)
        fprintf('USED  = %5d MB (%6.2f GB)\n',total-free,(total-free)/1024)
        fprintf('FREE  = %5d MB (%6.2f GB)\n',free,free/1024)
        
    end

end