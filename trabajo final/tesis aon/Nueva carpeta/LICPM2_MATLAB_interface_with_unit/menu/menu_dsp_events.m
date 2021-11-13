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

                % request info detection
                tx.dest_id = settings.MSG_DSP_ID;
                tx.msg_id = '7A';
                tx.payload = [];
                rx_payload = comm_routine( settings , tx , 0 );
                event_info.duration = (rx_payload(5) + rx_payload(6)*2^8 + rx_payload(7)*2^16 + rx_payload(8)*2^24)/150e6;
                
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
                
                % Clasificadores
                clasificador.SCA_type  = bitand( int16(floor(data(:,8)/2^0 )) , bin2dec('0000000000001111') );
                clasificador.SCA_phase = bitand( int16(floor(data(:,8)/2^4 )) , bin2dec('0000000000001111') );
                clasificador.SPA_type  = bitand( int16(floor(data(:,8)/2^8 )) , bin2dec('0000000000001111') );
                clasificador.SPA_phase = bitand( int16(floor(data(:,8)/2^12)) , bin2dec('0000000000001111') );
                clasificador.SVA_type  = bitand( int16(floor(data(:,9)/2^0 )) , bin2dec('0000000000001111') );
                clasificador.SVA_phase = bitand( int16(floor(data(:,9)/2^4 )) , bin2dec('0000000000001111') );
                clasificador.ASA_type  = bitand( int16(floor(data(:,9)/2^8 )) , bin2dec('0000000000001111') );
                clasificador.ASA_phase = bitand( int16(floor(data(:,9)/2^12)) , bin2dec('0000000000001111') );
                
                % show results
                event_info
                show_result( event_info , data , clasificador );
               
            else
                loop = 0;
            end 
        end
    end
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% function [  ] = process_data_and_show_result ( data )
% 
%     data_i = 1;
%     for i=1:size(data,1)
%         for k=0:3
%             data_p1(data_i,:) = data(i, 1+18*k : 18*(1+k));
%             data_i = data_i+1;
%         end
%     end
%     for i=1:size(data_p1,1)
%         for k=1:9
%             data_p2(i,k) = double(typecast(uint16(data_p1(i,k*2-1) + data_p1(i,k*2)*2^8),'int16'));
%         end
%     end
% 
%     subplot(3,1,1); plot(data_p2(:,1)); ylabel('counter')
%     subplot(3,1,2); plot(400/2^15*data_p2(:,2:5)); ylabel('voltage [V]')
%     subplot(3,1,3); plot(200/2^15*data_p2(:,6:9)); ylabel('current [A]')
%     
% end

function [ ] = show_result (event_info , data , clasificador )

    save figure_temp event_info data clasificador

% solo para debuging
%    clear all; close all;clc
%    load '2015-09-03 (T 10-47-12-929)Voltage Sag Waveform.mat'
%    load '2016-01-25 (T 09-11-31-074)Voltage Sag Waveform.mat'
%    load '2018-05-13 (T 05-53-13-980)Voltage Sag Waveform.mat'
%    load '2018-09-10 (T 01-29-39-760)Voltage Sag Waveform.mat'
%    load '2019-01-19 (T 07-35-15-847)Voltage Sag Waveform.mat'

    figure
    set(gcf,'Units','centimeters') % Unidades
    set(gcf,'Position',[1 1 20 28])% Posicion de ventana

    if (event_info.div == 1)
        f_loop = 2;
        f_pass = [0 2 4 6 8];
        f_i = [1 , 512 ; 513 , 1024];
        time_us = cumsum(2/150e6*data(1:1024,1));
        extra = time_us(256)+event_info.duration-time_us(767);
        time_us(513:1024) = time_us(513:1024)+extra;
        time_us = time_us - time_us(256);
    else
        f_loop = 1;
        f_pass = [0 1 2 3 4];
        f_i = [1 , 1024];
        time_us = cumsum(2/150e6*data(1:1024,1));
    end    
    %%%%%%
    for i=1:f_loop;
        %%%%%% vector de ticks para la escala horizontal
        time_ticks = round([time_us(f_i(i,1)):0.04:time_us(f_i(i,2))]*100)./100;
        %%%%%%
        subplot(5,f_loop,i+f_pass(1));
        plot(time_us(f_i(i,1):f_i(i,2)),400/2^15*data(f_i(i,1):f_i(i,2),2:4),'LineWidth',1.5); ylabel('Tensión [V]')
        axis([time_us(f_i(i,1)) time_us(f_i(i,2)) -400 400]); grid on
        set(gca,'XTick',time_ticks);
        if ( (f_loop == 1) || ( (f_loop == 2) && (i==2) ) )
            legend('V_a','V_b','V_c');
        end
        %%%%%%
        subplot(5,f_loop,i+f_pass(2));
        fill( [time_us(1),time_us(end),time_us(end),time_us(1)] , 220*[0.9,0.9,1.1,1.1] , 'g' , 'LineStyle','--'); hold on
        plot(time_us(f_i(i,1):f_i(i,2)),400/2^15*data(f_i(i,1):f_i(i,2),5:7),'LineWidth',1.5); ylabel('RMS [V]');
%        plot(time_us(f_i(i,1):f_i(i,2)),220*0.9*ones(size(time_us(f_i(i,1):f_i(i,2)))),'--k')
%        plot(time_us(f_i(i,1):f_i(i,2)),220*1.1*ones(size(time_us(f_i(i,1):f_i(i,2)))),'--k');
        axis([time_us(f_i(i,1)) time_us(f_i(i,2)) 0 300]); grid on
        set(gca,'XTick',time_ticks);
        if ( (f_loop == 1) || ( (f_loop == 2) && (i==1) ) )
            plot([time_us(event_info.start) time_us(event_info.start)],[40 220*0.9],'c')
            text(time_us(event_info.start),40,{'Detección de inicio'},'Background','w','HorizontalAlignment','center','EdgeColor','c')
            %text(time_us(10),10,{'Detección','de inicio'},'Background','w','HorizontalAlignment','left','VerticalAlignment','bottom','EdgeColor','k')
        end
        if ( (f_loop == 1) || ( (f_loop == 2) && (i==2) ) )    
            plot([time_us(event_info.stop) time_us(event_info.stop)],[40 220*0.9],'c')
            text(time_us(event_info.stop),40,{'Detección de fin'},'Background','w','HorizontalAlignment','center','EdgeColor','c')
        end
        %%%%%%
        if 1 % Ts
            subplot(5,f_loop,i+f_pass(3));
            plot(time_us(f_i(i,1):f_i(i,2)),1e6*data(f_i(i,1):f_i(i,2),1)/150e6,'LineWidth',1.5); ylabel({'Período de';'muestreo[\mus]'})
            axis([time_us(f_i(i,1)) time_us(f_i(i,2)) 140 170]); grid on
        else % Freq
            subplot(5,f_loop,i+f_pass(3));
            plot(time_us(f_i(i,1):f_i(i,2)),150e6./(128*data(f_i(i,1):f_i(i,2),1)),'LineWidth',1.5); ylabel('Frecuencia [Hz]')
            axis([time_us(f_i(i,1)) time_us(f_i(i,2)) 47 52]); grid on
        end
        set(gca,'XTick',time_ticks);
        %%%%%%
        subplot(5,f_loop,i+f_pass(4)); %plot(SVA_type(1:512,1),'m','LineWidth',3); hold on; plot(SCA_type(1:512,1),'r','LineWidth',2); plot(SPA_type(1:512,1),'g');
        plot(time_us(f_i(i,1):f_i(i,2)),clasificador.ASA_type(f_i(i,1):f_i(i,2),1),'color',[0,0.5,0],'LineWidth',1.5); hold off; ylabel('Tipo de evento')
        axis([time_us(f_i(i,1)) time_us(f_i(i,2)) 0 12]); grid on
        set(gca,'YTick',[1:1:12],'YTickLabel',['NOR';'A  ';'B  ';'C  ';'D  ';'E  ';'F  ';'G  ';'H  ';'I* ';'I**';'   '])
        set(gca,'XTick',time_ticks);
        %%%%%%
        subplot(5,f_loop,i+f_pass(5)); %plot(SVA_phase(1:512,1),'m','LineWidth',3); hold on; plot(SCA_phase(1:512,1),'r','LineWidth',2); plot(SPA_phase(1:512,1),'g');
        plot(time_us(f_i(i,1):f_i(i,2)),clasificador.ASA_phase(f_i(i,1):f_i(i,2),1),'r','LineWidth',1.5); hold off; ylabel('Fases afectadas')
        axis([time_us(f_i(i,1)) time_us(f_i(i,2)) 0 9]); grid on
        set(gca,'YTick',[1:1:9],'YTickLabel',['NOR';'A  ';'B  ';'C  ';'AB ';'BC ';'CA ';'ABC';'   '])
        set(gca,'XTick',time_ticks);
        xlabel('t[segundos]')
    end
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % IMPRIMIR PDF
    %set(gcf,'MenuBar','none')
    Scsize = get(gcf,'OuterPosition');
    Bar = 2;        
    set(gcf,'PaperPositionMode','auto','PaperSize',...
        [Scsize(3) Scsize(4)-Bar],'PaperUnits','centimeters')
    print('-dpdf','figure_temp.pdf')
    
end

%[ time_us(256)+event_info.duration ; time_us(767) ]