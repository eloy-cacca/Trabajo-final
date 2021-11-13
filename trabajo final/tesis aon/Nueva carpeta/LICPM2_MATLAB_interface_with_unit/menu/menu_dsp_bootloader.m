function [  ] = menu_dsp_bootloader( help , settings )

    if (help == 1)
        fprintf('DSP bootloader\n')
    else
        
        % clear screem
        clc
        
        % start loop
        loop = 1;
        while (loop)

            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
            fprintf('Ver. 2\n');
            fprintf('Main - DSP bootloader\n');
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
        fprintf('DSP - Bootloader Start\n');
    else
        
        % serialport_write( settings.serial_port , 65 ); % 'A'
        %tic
        %while (toc<0.05)
        %end
      
        % leer archivo
        file_name = 'f28335_flash_kernel.txt';
        fid = fopen(file_name);
        data = fread(fid);
        % generar vector
        for i=4:length(data)
            
        end
        
       
    end
    
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%