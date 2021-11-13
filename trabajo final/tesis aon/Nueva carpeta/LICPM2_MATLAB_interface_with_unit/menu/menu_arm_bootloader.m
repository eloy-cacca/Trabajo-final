function [  ] = menu_arm_bootloader( help , settings )

    if (help == 1)
        fprintf('ARM bootloader\n')
    else
        
        % clear screem
        clc
        
        % start loop
        loop = 1;
        while (loop)

            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
            fprintf('Ver. 2\n');
            fprintf('Main - ARM bootloader\n');
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
        fprintf('ARM - Send START command (test)\n');
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '80';
        image(1) = 1*2^8 + 15;
        image(2) = 200*1024;
        image(3) = 0;
        image(4) = 126;
        uc_1 = dec2hex(floor(typecast(int32(image),'uint32')),8);
        for i=1:size(uc_1,1)
            tx.payload(4*i-3,:) = uc_1(i,7:8);
            tx.payload(4*i-2,:) = uc_1(i,5:6);
            tx.payload(4*i-1,:) = uc_1(i,3:4);
            tx.payload(4*i  ,:) = uc_1(i,1:2);
        end
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
        fprintf('ARM - Send UPLOAD BLOCK command (test)\n');
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '82';
        %tx.payload = ['05';'00';'01';'02';'03'];
        %tx.payload = ['10';'00';'04';'05';'06'];
        tx.payload = ['00';'01';'07';'08';'09'];
        rx_payload = comm_routine( settings , tx , 20 );
        
    end
    
end
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option3( help , settings)
    
    if (help == 1)
        fprintf('ARM - Send INSTALL IMAGE command (test)\n');
    else
        % request
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '84';
        image(1) = 1*2^8 + 15;
        image(2) = 200*1024;
        image(3) = 0;
        image(4) = 126;
        uc_1 = dec2hex(floor(typecast(int32(image),'uint32')),8);
        for i=1:size(uc_1,1)
            tx.payload(4*i-3,:) = uc_1(i,7:8);
            tx.payload(4*i-2,:) = uc_1(i,5:6);
            tx.payload(4*i-1,:) = uc_1(i,3:4);
            tx.payload(4*i  ,:) = uc_1(i,1:2);
        end
        rx_payload = comm_routine( settings , tx , 20 )
        
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [ ] = option4( help , settings)
    
    if (help == 1)
        fprintf('ARM - Complete routine\n')
    else
        L = 64;
        
        in = input('OTA file name: ','s');
 %       in2 = input('samples: ');
        % processing file
        fprintf('Processing file...')
        app = func_intelhex(in);
        fprintf('OK')
 %           N_temp = length(app);
 %           temp = app(1:in2);
 %           clear app;
 %           app = temp;
        N = length(app);
        msg_N = ceil(N/L);
        fprintf('OK, image = %6.2f Kbyte\n',N/1024)
        
        % print image
        if 0 
            fprintf('\n')
            fprintf('0x00440000 ');
            for i=1:N
                fprintf('%s ',dec2hex(app(i),2));
                if (mod(i,18) == 0)
                    fprintf('\n0x004%s ',dec2hex(262144 + i,5)); % hex2dec('40000')=262144
                end
            end
            fprintf('\n');    
        end

        % calcular CRC
        fprintf('Computing CRC...')
        CRC = crc_calculate(app);
        fprintf('OK\n')   
                
        % send start command
        fprintf('Send START command...')
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '80';
        image(1) = 1*2^8 + 15;  % version del firmware (no se esta utilizando)
        image(2) = N;           % tamaño de la imagen 
        image(3) = CRC;         % CRC
        image(4) = L;           % bytes por paquete
        uc_1 = dec2hex(floor(typecast(int32(image),'uint32')),8);
        for i=1:size(uc_1,1)
            tx.payload(4*i-3,:) = uc_1(i,7:8);
            tx.payload(4*i-2,:) = uc_1(i,5:6);
            tx.payload(4*i-1,:) = uc_1(i,3:4);
            tx.payload(4*i  ,:) = uc_1(i,1:2);
        end
        rx_payload = comm_routine( settings , tx , 0 );
        if isempty(rx_payload) return;
        end
        if (rx_payload(1) == 0) fprintf('OK\n')
        else                    fprintf('ERROR = %d\n',rx_payload(1)); return
        end
        
        % Send data
        for i=0:(msg_N-1)
            clear tx;
            tx.dest_id = settings.MSG_ARM_ID;
            tx.msg_id = '82';
            index = dec2hex(i,4);
            if (i<(msg_N-1))
                tx.payload = [index(3:4) ; index(1:2) ; dec2hex(app(i*L+1:i*L+L),2)];
            else
                tx.payload = [index(3:4) ; index(1:2) ; dec2hex(app(i*L+1:end),2)];
            end
            rx_payload = comm_routine( settings , tx , 0 );
            if isempty(rx_payload) return;
            end
            if (rx_payload(1) ~= 0) fprintf('ERROR = %d\n',rx_payload(1)); return
            end
            if (mod( i , 50) == 0)
                fprintf('Send DATA command...%d/%d\n',i,msg_N);
            end
        end
        fprintf('Send DATA command...OK\n')
        
        % Send intall
        fprintf('Send INSTALL command...')
        clear tx;
        tx.dest_id = settings.MSG_ARM_ID;
        tx.msg_id = '84';
        for i=1:size(uc_1,1)
            tx.payload(4*i-3,:) = uc_1(i,7:8);
            tx.payload(4*i-2,:) = uc_1(i,5:6);
            tx.payload(4*i-1,:) = uc_1(i,3:4);
            tx.payload(4*i  ,:) = uc_1(i,1:2);
        end
        rx_payload = comm_routine( settings , tx , 0 );
        if isempty(rx_payload) return;
        end
        if (rx_payload(1) ~= 0) fprintf('ERROR = %d\n',rx_payload(1)); return
        end
        fprintf('OK\n') 
        
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%