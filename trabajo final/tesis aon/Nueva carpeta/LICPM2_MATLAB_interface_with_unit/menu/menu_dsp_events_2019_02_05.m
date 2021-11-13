function [  ] = menu_dsp_events( help , settings )
    if (help == 1)
        fprintf('DSP events\n')
    else

        % infinite loop
        fprintf('\n');
        loop = 1; % set flag for exit
        while (loop)

            % show name
            fprintf('PROJ_PMV2FW1_PMV2_PBV1_MATLAB02\n');
            fprintf('Ver. 1\n');
            fprintf('Main -> DSP Event\n');
            fprintf('Options:\n');
            % show options
            fprintf('(01) Start\n');
            fprintf('(other) Exit \n');
            in = input('Choose an option: '); % read input as num
            fprintf('\n');        
            
            % take action depending on selected option
            if (in==1)
                
                tic
                
                % send commands
                N = 1024;
                msg ='';
                for i=[0:N+1]
                    
                    % show progress
                    if (mod(i,32)==0)
                        fprintf(repmat('\b', 1, length(msg)))
                        msg=sprintf('%d of %d (t = %4.3fsec)\n',i,N,toc);
                        fprintf(msg)
                    end
                    
                    % send command
                    tx.dest_id = settings.MSG_DSP_ID;
                    tx.msg_id = '78';
                    tx.payload = [];
                    i_HEX = dec2hex(floor(i),4);
                    tx.payload = [i_HEX(3:4);i_HEX(1:2)];
                    rx_payload = comm_routine( settings , tx , 0 );

                    % unir datos de 8 bits en 16 bits
                    for k=1:(length(rx_payload)/2)
                        data_p(k) = rx_payload(k*2-1) + rx_payload(k*2)*2^8;
                    end
                    
                    % response
                    if (i==0)
                        
                        % guardar datos con formato
                        k = 1;
                        event_info.start  = data_p(k);      k=k+1;
                        event_info.stop   = data_p(k);      k=k+1;
                        event_info.div    = data_p(k);      k=k+1;

                    elseif ( (i>0) & (i<(N+1)) )
                        data(i,:) = double(typecast(uint16(data_p'),'int16'));
                    elseif ( i == N+1)
                        fprintf('OK\n')                        
                    else
                        % none for now 
                    end
                
                end
                
                event_info
                SCA_type  = bitand( int16(floor(data(:,8)/2^0 )) , bin2dec('0000000000001111') );
                SCA_phase = bitand( int16(floor(data(:,8)/2^4 )) , bin2dec('0000000000001111') );
                SPA_type  = bitand( int16(floor(data(:,8)/2^8 )) , bin2dec('0000000000001111') );
                SPA_phase = bitand( int16(floor(data(:,8)/2^12)) , bin2dec('0000000000001111') );
                SVA_type  = bitand( int16(floor(data(:,9)/2^0 )) , bin2dec('0000000000001111') );
                SVA_phase = bitand( int16(floor(data(:,9)/2^4 )) , bin2dec('0000000000001111') );
                ASA_type  = bitand( int16(floor(data(:,9)/2^8 )) , bin2dec('0000000000001111') );
                ASA_phase = bitand( int16(floor(data(:,9)/2^12)) , bin2dec('0000000000001111') );
                %   data
                if (event_info.div == 1)
                    %%%%%%                    
                    subplot(5,2,1); plot(400/2^15*data(1:512,2:4)); ylabel('voltage [V]')
                    axis([0 N/2 -400 400]); grid on
                    subplot(5,2,2); plot(400/2^15*data(513:1024,2:4)); ylabel('voltage [V]')
                    axis([0 N/2 -400 400]); grid on
                    legend('V_A','V_B','V_C')
                    %%%%%%
                    subplot(5,2,3); plot(400/2^15*data(1:512,5:7)); ylabel('RMS [V]')
                    axis([0 N/2 0 300]); grid on
                    subplot(5,2,4); plot(400/2^15*data(513:1024,5:7)); ylabel('RMS [V]')
                    axis([0 N/2 0 300]); grid on                    
                    legend('V_A','V_B','V_C')
                    %%%%%%
                    if 0 % Ts
                        subplot(5,2,5); plot(data(1:512,1)/150e6); ylabel('Ts')
                        axis([0 N/2 140e-6 170e-6]); grid on
                        subplot(5,2,6); plot(data(513:1024,1)/150e6); ylabel('Ts')
                        axis([0 N/2 140e-6 170e-6]); grid on
                    else % Freq
                        subplot(5,2,5); plot(150e6./(128*data(1:512,1))); ylabel('freq')
                        axis([0 N/2 47 52]); grid on
                        subplot(5,2,6); plot(150e6./(128*data(513:1024,1))); ylabel('freq')
                        axis([0 N/2 47 52]); grid on
                    end                    
                    %%%%%%
                    subplot(5,2,7); plot(SVA_type(1:512,1),'b','LineWidth',3); hold on; plot(SCA_type(1:512,1),'r','LineWidth',2); plot(SPA_type(1:512,1),'g'); plot(ASA_type(1:512,1),'m'); hold off; ylabel('Type')
                    axis([0 N/2 0 11]); grid on
                    set(gca,'YTick',[0:1:11],'YTickLabel',...
                        ['IND';'NOR';'A  ';'B  ';'C  ';'D  ';'E  ';'F  ';'G  ';'H  ';'I* ';'I**'])
                    subplot(5,2,8); plot(SVA_type(513:1024,1),'b','LineWidth',3); hold on; plot(SCA_type(513:1024,1),'r','LineWidth',2); plot(SPA_type(513:1024,1),'g'); plot(ASA_type(513:1024,1),'m'); hold off; ylabel('Type')
                    axis([0 N/2 0 11]); grid on
                    set(gca,'YTick',[0:1:11],'YTickLabel',...
                        ['IND';'NOR';'A  ';'B  ';'C  ';'D  ';'E  ';'F  ';'G  ';'H  ';'I* ';'I**'])
                    legend('SVA','SCA','SPA','ASA')
                    %%%%%%
                    subplot(5,2,9); plot(SVA_phase(1:512,1),'b','LineWidth',3); hold on; plot(SCA_phase(1:512,1),'r','LineWidth',2); plot(SPA_phase(1:512,1),'g'); plot(ASA_phase(1:512,1),'m'); hold off; ylabel('Phase')
                    axis([0 N/2 0 8]); grid on
                    set(gca,'YTick',[0:1:8],'YTickLabel',...
                        ['IND';'NOR';'A  ';'B  ';'C  ';'AB ';'BC ';'CA ';'ABC'])                    
                    subplot(5,2,10); plot(SVA_phase(513:1024,1),'b','LineWidth',3); hold on; plot(SCA_phase(513:1024,1),'r','LineWidth',2); plot(SPA_phase(513:1024,1),'g'); plot(ASA_phase(513:1024,1),'m'); hold off; ylabel('Phase')
                    axis([0 N/2 0 8]); grid on
                    set(gca,'YTick',[0:1:8],'YTickLabel',...
                        ['IND';'NOR';'A  ';'B  ';'C  ';'AB ';'BC ';'CA ';'ABC'])                    
                    legend('SVA','SCA','SPA','ASA')
                else      
                    %%%%%%
                    %subplot(5,1,1); plot(data(:,2)/30000); ylabel('voltage [V]')
                    %axis([0 N 0.95 1]); grid on
                    %subplot(5,1,1); plot(data(:,2)/10); ylabel('voltage [V]')
                    %axis([0 N 0 80]); grid on
                    subplot(5,1,1); plot(400/2^15*data(:,2:4)); ylabel('voltage [V]')
                    axis([0 N -400 400]); grid on
                    legend('V_A','V_B','V_C')
                    %%%%%%
                    subplot(5,1,2); plot(400/2^15*data(:,5:7)); ylabel('RMS [V]')
                    axis([0 N 0 300]); grid on
                    legend('V_A','V_B','V_C')
                    %%%%%%
                    if 0 % Ts
                        subplot(5,1,3); plot(data(:,1)/150e6); ylabel('Ts')
                        axis([0 N 140e-6 170e-6]); grid on
                    else % Freq
                        subplot(5,1,3); plot(150e6./(128*data(:,1))); ylabel('freq')
                        axis([0 N 47 52]); grid on
                    end
                    %%%%%%
                    subplot(5,1,4); plot(SVA_type,'b','LineWidth',3); hold on; plot(SCA_type,'r','LineWidth',2); plot(SPA_type,'g'); plot(ASA_type,'m'); hold off; ylabel('Type')
                    axis([0 N 0 11]); grid on
                    set(gca,'YTick',[0:1:11],'YTickLabel',...
                        ['IND';'NOR';'A  ';'B  ';'C  ';'D  ';'E  ';'F  ';'G  ';'H  ';'I* ';'I**'])
                    legend('SVA','SCA','SPA','ASA')
                    subplot(5,1,5); plot(SVA_phase,'b','LineWidth',3); hold on; plot(SCA_phase,'r','LineWidth',2); plot(SPA_phase,'g'); plot(ASA_phase,'m'); hold off; ylabel('Phase')
                    axis([0 N 0 8]); grid on
                    set(gca,'YTick',[0:1:8],'YTickLabel',...
                        ['IND';'NOR';'A  ';'B  ';'C  ';'AB ';'BC ';'CA ';'ABC'])                    
                    legend('SVA','SCA','SPA','ASA')
                end
                

                
            else
                loop = 0;
            end 
        end
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [  ] = process_data_and_show_result ( data )

    data_i = 1;
    for i=1:size(data,1)
        for k=0:3
            data_p1(data_i,:) = data(i, 1+18*k : 18*(1+k));
            data_i = data_i+1;
        end
    end
    for i=1:size(data_p1,1)
        for k=1:9
            data_p2(i,k) = double(typecast(uint16(data_p1(i,k*2-1) + data_p1(i,k*2)*2^8),'int16'));
        end
    end

    subplot(3,1,1); plot(data_p2(:,1)); ylabel('counter')
    subplot(3,1,2); plot(400/2^15*data_p2(:,2:5)); ylabel('voltage [V]')
    subplot(3,1,3); plot(200/2^15*data_p2(:,6:9)); ylabel('current [A]')
    
end