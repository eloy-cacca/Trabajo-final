% show trend
function [ ] = func_trend_show ( trend )
     
    % MOSTRAR RESULTADOS
    fprintf('Test Trend\n');
    fprintf('----------------------------------------------------------------------------------------------------------------------\n')
    fprintf('RTC time %2d:', trend.time_h );
    fprintf('%2d:', trend.time_m );
    fprintf('%2d ', trend.time_s );
    fprintf('%4d\n', trend.time_ms );
    fprintf('Time utilization = %6.2fus\n', trend.time_utilization*1e6 );
    fprintf('Frequency = %6.2fHz [%6.2fHz - %6.2fHz]\n', trend.fL_AVG , trend.fL_MIN , trend.fL_MAX );
    fprintf('----------------------------------------------------------------------------------------------------------------------\n')
    signal_name = ['V1 ';'V2 ';'V3 ';'VE ';'I1A';'I2A';'I3A';'IN ';'I1B';'I2B';'I3B';'IE '];
    signal_type = ['V';'V';'V';'V';'A';'A';'A';'A';'A';'A';'A';'A'];
    fprintf('    |   RMS    |   TWD    |        DC           |       1st          |        3nd         |        5th         |        7th         |\n')
    fprintf('    |          |          | Module   |  Phase   | Module  |  Phase   | Module  |  Phase   | Module  |  Phase   | Module  |  Phase   |\n')
    for i=1:8%12
       fprintf([signal_name(i,:),' | '])
       fprintf('%7.2f%s | ',trend.ch(i).rms_AVG,signal_type(i,:)); %
       fprintf('%7.2f%% | ',trend.ch(i).TWD_AVG);
       for k=1:5
           value = trend.ch(i).harmonic(k).real_AVG + 1i*trend.ch(i).harmonic(k).imag_AVG;
           fprintf('%7.2f%s | %7.2fº |',abs(value),signal_type(i,:),180/pi*angle(value));
       end
       fprintf('\n');
    end
    fprintf('----------------------------------------------------------------------------------------------------------------------\n')
    fprintf('          |                           Sequences                             |\n')
    fprintf('          |      Positive       |      Negative       |    Homopolar        |\n')
    fprintf('          |  Module  |  Phase   |  Module  |  Phase   |  Module  |  Phase   |\n')
    fprintf('Voltage   | ');
    value = trend.v_seq_pos.real_AVG + 1i*trend.v_seq_pos.imag_AVG;
    fprintf('%7.2fV | %7.2fº | ',abs(value),180/pi*angle(value));
    value = trend.v_seq_neg.real_AVG + 1i*trend.v_seq_neg.imag_AVG;
    fprintf('%7.2fV | %7.2fº | ',abs(value),180/pi*angle(value));
    value = trend.v_seq_hom.real_AVG + 1i*trend.v_seq_hom.imag_AVG;
    fprintf('%7.2fV | %7.2fº | ',abs(value),180/pi*angle(value));
    fprintf('\n');
    fprintf('Current A | ');
    value = trend.iA_seq_pos.real_AVG + 1i*trend.iA_seq_pos.imag_AVG;
    fprintf('%7.2fA | %7.2fº | ',abs(value),180/pi*angle(value));
    value = trend.iA_seq_neg.real_AVG + 1i*trend.iA_seq_neg.imag_AVG;
    fprintf('%7.2fA | %7.2fº | ',abs(value),180/pi*angle(value));
    value = trend.iA_seq_hom.real_AVG + 1i*trend.iA_seq_hom.imag_AVG;
    fprintf('%7.2fA | %7.2fº | ',abs(value),180/pi*angle(value));
    fprintf('\n');
    fprintf('Current B | ');
    value = trend.iB_seq_pos.real_AVG + 1i*trend.iB_seq_pos.imag_AVG;
    fprintf('%7.2fA | %7.2fº | ',abs(value),180/pi*angle(value));
    value = trend.iB_seq_neg.real_AVG + 1i*trend.iB_seq_neg.imag_AVG;
    fprintf('%7.2fA | %7.2fº | ',abs(value),180/pi*angle(value));
    value = trend.iB_seq_hom.real_AVG + 1i*trend.iB_seq_hom.imag_AVG;
    fprintf('%7.2fA | %7.2fº | ',abs(value),180/pi*angle(value));    
    fprintf('\n');
    fprintf('----------------------------------------------------------------------------------------------------------------------\n')
    fprintf('          |       Desbalance       |\n')
    fprintf('          |  Negative  | Homopolar |\n')
    fprintf('Voltage   | ');
    fprintf(' %7.2f %% | %7.2f %% |\n',trend.v_desb_neg.mod_AVG,trend.v_desb_hom.mod_AVG);
    fprintf('Current A | ');
    fprintf(' %7.2f %% | %7.2f %% |\n',trend.iA_desb_neg.mod_AVG,trend.iA_desb_hom.mod_AVG);
    fprintf('Current B | ');
    fprintf(' %7.2f %% | %7.2f %% |\n',trend.iB_desb_neg.mod_AVG,trend.iB_desb_hom.mod_AVG);
    fprintf('----------------------------------------------------------------------------------------------------------------------\n')
    fprintf('          |                      Power                       |\n')
    fprintf('          |  Apparent  | Real      | Reactive    |   PF      |\n')   
    signal_power_name = ['P1A';'P2A';'P3A';'P1B';'P2B';'P3B'];
    for i=1:6
       fprintf([signal_power_name(i,:),'       |'])
       fprintf(' %8.2fVA | %8.2fW | %8.2fVAR | %8.2f%% |\n',trend.ch_power(i).ApparentPower_AVG,trend.ch_power(i).RealPower_AVG,trend.ch_power(i).ReactivePower_AVG,trend.ch_power(i).PowerFactor_AVG);
    end  
    fprintf('\n');
    
    % figure
    color = ['r' , 'y' , 'b' , 'g' , 'r' , 'y' , 'b' , 'k' ];
    name = ['V1' ; 'V2' ; 'V3' ; 'VE' ; 'I1' ; 'I2' ; 'I3' ; 'IN'];

    subplot(2,1,1)
    for i=1:4
        quiver(0,0,trend.ch(i).harmonic(2).real_AVG,trend.ch(i).harmonic(2).imag_AVG,color(i),'LineWidth',2);hold on;
        text(trend.ch(i).harmonic(2).real_AVG,trend.ch(i).harmonic(2).imag_AVG,name(i,:));
    end
    axis([-300 300 -300 300])
    grid on   
    drawnow;
    hold off

    subplot(2,1,2)
    for i=5:8
        quiver(0,0,trend.ch(i).harmonic(2).real_AVG,trend.ch(i).harmonic(2).imag_AVG,color(i),'LineWidth',2);hold on;
        text(trend.ch(i).harmonic(2).real_AVG,trend.ch(i).harmonic(2).imag_AVG,name(i,:));
    end
    axis([-100 100 -100 100])
    grid on   
    drawnow;
    hold off

    
end
