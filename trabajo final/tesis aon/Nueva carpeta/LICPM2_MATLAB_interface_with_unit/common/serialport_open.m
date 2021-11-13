function [ serial_port ] = serialport_open( settings  )

    delete(instrfind({'Port'},{settings.port})); % Delete communication interface objects if it exist
    serial_port = serial(settings.port);         % Serial Port Initialization
    serial_port.BaudRate = settings.baud;        % set baud rate
    serial_port.Timeout = settings.timeout;      % timeout
    fopen(serial_port);                          % Open serial port

end

