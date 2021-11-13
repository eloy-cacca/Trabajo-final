% Read message from port. Set number of byte to be read (N).
function [byte] = port_read ( serial_port , N )

    byte = fread(serial_port,N);
    
end