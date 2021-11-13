function [ ] = comm_print_msg( title , msg , byte_per_line )

    tab = blanks(length(title)); % String of blanks.

    if (byte_per_line ~= 0) 
    
        fprintf(title);
        RX_show_counter = 0; % counter used to present data for line
        for g=1:length(msg)
            fprintf([dec2hex(msg(g),2),' ']); % write data in console
            RX_show_counter = RX_show_counter + 1;
            if (RX_show_counter>byte_per_line)
                fprintf('\n');
                fprintf(tab);
                RX_show_counter = 0;
            end
        end
        fprintf('\n');
    end

end

