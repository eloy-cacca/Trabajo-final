% Power Meter V2 - Comunication script
% Ver 4
%
% Implementa una interface de comunicación con la placa de procesamiento
% mediante UART. Se ejecuta el script con la función y un menu indica las
% opciones disponibles.
%
% La carpeta menu almacena las opciones principales disponibles.
%
% En el archivo settings_file se almacena la configuración del proyecto así
% como el COM utilizado y los baud.
%
function [ ] = proj_PMv2fw1_MATLAB( type )

    % add path
    addpath('common')
    addpath('app')
    addpath('menu')
    
    % add macros and init port
    settings_file
    
    % save main availables options
    folder = 'menu';
    list = dir(folder);
    N = length(list);
    option_N = 0;
    for i=3:N % read list
        if ~strcmp( 'asv' , list(i).name(end-2:end) ) % ignore matlab save files
            option_N = option_N + 1;
            option{option_N} = list(i).name(1:end-2);
        end
    end
    option_N = length(option);
    clear folder list N
    
    % start loop
    loop = 1;
    while (loop)
        
        % clear screem
        clc
        
        % show name
        fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
        fprintf('Ver. 2\n');
        fprintf('Main\n');
        fprintf('Options: \n');
        
        % show options
        for k=1:option_N
            fprintf(['(',num2str(k,'%1.2d'),') '])         % show number
            eval([option{k},'(1,settings);']);  % show description
        end
        fprintf('(other) Return to menu \n');
        in = input('Choose an option: '); % read input as num
        
        % check option
        if ((in>0)&&(in<=option_N))
            eval([option{in},'(0,settings);']);
        else
            loop = 0;
        end
        
    end
    
    % close port
    if strcmp( settings.type , 'UART')
        serialport_close(settings.serial_port)
    end

    
end
