function [  ] = serialport_close( serial_port )

    fclose(serial_port);
    delete(serial_port);

end

