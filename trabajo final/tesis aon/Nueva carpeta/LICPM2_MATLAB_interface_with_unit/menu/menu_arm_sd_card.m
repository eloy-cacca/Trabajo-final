function [  ] = menu_arm_sd_card( help , settings )
    
    if (help == 1)
        fprintf('ARM SD-Card Routines\n')
    else
        
        % clear screem
        clc
        
        % start loop
        loop = 1;
        while (loop)

            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
            fprintf('Ver. 2\n');
            fprintf('Main -> SD-Card content\n');
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
        fprintf('ARM - SD-Card - Scan directory\n')
    else
        
        % open dir
        in = input('Directory Name (none = root / exit): ','s');
        in(end+1) = 0; % add null character
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '60';
        tx.payload = dec2hex(uint8(in),2);
        rx_payload = comm_routine( settings , tx , 20 );   
        if (rx_payload(1) ~= 0)
            fprintf('\nERROR\n')
            return;
        end

        % scan directory
        item = scan_dir( settings );
            
        % show result
        fprintf('\n')
        M = length(item);
        for i=1:M % show folders
            if (item(i).atribute(4) == '1')
                fprintf('%s\n',item(i).LFN)
            end
        end
        for i=1:M % show files
            if (item(i).atribute(3) == '1')
                fprintf('%s\n',item(i).LFN)
            end
        end

    end
end

function [ ] = option2( help , settings)

    if (help == 1)
        fprintf('ARM - SD-Card - Copy file\n')
    else

        % open file
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '66';
        in = input('File Name (use "/" for folders): ','s');
        in(end+1) = 0; % add null character
        tx.payload = dec2hex(uint8(in));
        rx_payload = comm_routine( settings , tx , 0 );
        if (rx_payload(1) ~= 0)
            fprintf('\nERROR\n')
            return;
        end
                
        % request file size
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '6C';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 0 );
        file_size = rx_payload(1) + rx_payload(2)*2^8 + rx_payload(3)*2^16 + rx_payload(4)*2^24;
        
        % generate file. check that '/' is not present
        pos = strfind(in , '/');
        if (isempty(pos))
            fid = fopen(in,'wt'); 
        else
            fid = fopen(in(pos(end)+1:end),'wt');                       
        end
        
        % request data
        loop = 1;
        index = 0;
        file_size_accum = 0;
        tic;
        while (loop)
            
            % request data
            tx.dest_id = settings.MSG_ARM_ID;
            tx.msg_id = '68';
            in_1 = dec2hex(floor(typecast(int32(index),'uint32')),8);
            for i=1:size(in_1,1)
                tx.payload(4*i-3,:) = in_1(i,7:8);
                tx.payload(4*i-2,:) = in_1(i,5:6);
                tx.payload(4*i-1,:) = in_1(i,3:4);
                tx.payload(4*i  ,:) = in_1(i,1:2);
            end
            rx_payload = comm_routine( settings , tx , 0 );
            byte_read = rx_payload(1) + rx_payload(2)*2^8 + rx_payload(3)*2^16 + rx_payload(4)*2^24;

            % show result
            file_size_accum = file_size_accum + byte_read;
            if (mod(file_size_accum,124*10)==0)
                fprintf('%d of %d', file_size_accum , file_size );
                fprintf(' - time = %.2f\n',toc)
            end
            
            % print data or go out
            if (byte_read == 0)
                loop = 0;
            else
                fprintf(fid,'%s',char(rx_payload(5:end)));
                index = index + 1;
            end
                  
        end
        fclose(fid);
        
    end
end

function [ ] = option3( help , settings)

    if (help == 1)
        fprintf('ARM - SD-Card - Remove file or directory (it is performed in background)\n')
    else

        % open file
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '6E';
        in = input('Dir Name: ','s');
        in(end+1) = 0; % add null character
        tx.payload = dec2hex(uint8(in));
        rx_payload = comm_routine( settings , tx , 20 );
        if (rx_payload(1) == 0)
            fprintf('\nOK (update file/directory to be removed)\n')
        else
            fprintf('\nERROR = %d\n',rx_payload(1))
        end        
    end
end

function [item] =  scan_dir( settings )

    item_loop = 1;
    item_N = 0;
    while (item_loop)
        % read item
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '62';
        tx.payload = [];
        rx_payload = comm_routine( settings , tx , 20 );
%          tic
%          while(toc<1)
%          end
        if (rx_payload == 0)
            item_loop = 0;
        else
            % save data
            item_N = item_N + 1;
            item(item_N).atribute = dec2bin(rx_payload(9),8);
            item(item_N).name = char(rx_payload(10:24));
            % read long file name (LFN)
            tx.dest_id = settings.MSG_ARM_ID;
            tx.msg_id = '64';
            tx.payload = [];
            rx_payload = comm_routine( settings , tx , 20 );
            % unir datos de 8 bits en 16 bits
            item(item_N).LFN = char(rx_payload);
        end
    end

end

