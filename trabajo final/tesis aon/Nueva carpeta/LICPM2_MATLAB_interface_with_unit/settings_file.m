% Connection type
if strcmp( type , 'UNIT01_UART' )
    settings.port      = 'COM4';
%    settings.baud      = 38400;
    settings.baud      = 937500;  % [bps]
%    settings.baud      = 1.5e6;  % [bps]
    settings.timeout   = 5.0;    % [seconds]
    settings.serial_port = serialport_open( settings );  
    settings.type = 'UART';
elseif strcmp( type , 'UNIT01_UART_BOOTLOADER' )
    settings.port      = 'COM4';
    settings.baud      = 38400;  % [bps]
    settings.timeout   = 2.0;    % [seconds]
    settings.serial_port = serialport_open( settings );  
    settings.type = 'UART';    
elseif strcmp( type , 'UNIT02_IP_ETH' )
    settings.ip        = '10.2.11.154';
    settings.type      = 'IP';
elseif strcmp( type , 'UNIT02_IP_WIFI' )
    settings.ip        = '192.168.1.1';
    settings.type      = 'IP';
elseif strcmp( type , 'LOCAL_IP_ETH' )
    settings.ip        = '127.0.0.1';
    settings.type      = 'IP';
    %elseif strcmp( settings.type , 'IP02' )
%    %settings.ip        = '10.2.213.234';
%    %settings.ip        = '10.2.11.151';
%    %settings.ip        = '192.168.1.1';
%    settings.ip        = '10.2.11.154';
else
    fprintf(2,'No suported device\n');
    return;
end

% Message ID
settings.MSG_PROJECT_ID = '04';
settings.MSG_DSP_ID     = '33';
settings.MSG_ARM_ID     = '77';
settings.MSG_RPI_ID     = 'AA';
settings.MSG_OTHERS_ID  = 'EE';

% Message format
settings.MSG_START_BYTE      = '3C';
settings.MSG_LENGHT_HEADER   = 8;
settings.MSG_LENGHT_PAYLOAD  = 128;
settings.MSG_LENGHT_MAX_SIZE = settings.MSG_LENGHT_HEADER + settings.MSG_LENGHT_PAYLOAD;

% Message header
settings.MSG_HEADER_START       = 1;
settings.MSG_HEADER_PORJ_ID     = 2;
settings.MSG_HEADER_SRC_ID      = 3;
settings.MSG_HEADER_DES_ID      = 4;
settings.MSG_HEADER_MSG_ID      = 5;
settings.MSG_HEADER_PAYLOAD_LEN = 6;
settings.MSG_HEADER_1ST_BYTE    = 7;

