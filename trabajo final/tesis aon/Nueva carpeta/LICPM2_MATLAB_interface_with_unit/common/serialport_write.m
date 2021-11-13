% Write message into port.
function [ ] = port_write ( serial_port , msg )

    fwrite(serial_port,msg); % send data
    
end