% Generate trend variable from data
function [trend] = func_trend_generate ( data )

    gain = [400 400 400 400 400 400 400 400 400 400 400 400];

    % unir datos de 8 bits en 16 bits
    for i=1:(length(data)/2)
        data_p(i) = double(typecast(uint16(data(i*2-1) + data(i*2)*2^8),'int16'));
    end

    % guardar datos con formato
    k = 1;
    trend.time_ms           = data_p(k);       k=k+1;
    trend.time_s            = data_p(k);       k=k+1;
    trend.time_m            = data_p(k);       k=k+1;
    trend.time_h            = data_p(k);       k=k+1;
    trend.flags             = data_p(k);       k=k+1;
    trend.time_utilization  = data_p(k)/150e6; k=k+1;
    trend.Ts_AVG            = data_p(k)/150;   k=k+1;
    trend.Ts_MAX            = data_p(k)/150;   k=k+1;
    trend.Ts_MIN            = data_p(k)/150;   k=k+1;
    trend.fL_AVG            = data_p(k)/2^8;   k=k+1;
    trend.fL_MAX            = data_p(k)/2^8;   k=k+1;
    trend.fL_MIN            = data_p(k)/2^8;   k=k+1;
    for i=1:12
%    for i=1:8    
        trend.ch(i).rms_AVG = gain(i)/2^15*( data_p(k) ); k=k+1;
        trend.ch(i).rms_MAX = gain(i)/2^15*( data_p(k) ); k=k+1;
        trend.ch(i).rms_MIN = gain(i)/2^15*( data_p(k) ); k=k+1;
        for h=1:5
            trend.ch(i).harmonic(h).real_AVG = gain(i)/2^15*( data_p(k) ); k=k+1;
            trend.ch(i).harmonic(h).real_MAX = gain(i)/2^15*( data_p(k) ); k=k+1;
            trend.ch(i).harmonic(h).real_MIN = gain(i)/2^15*( data_p(k) ); k=k+1;
            trend.ch(i).harmonic(h).imag_AVG = gain(i)/2^15*( data_p(k) ); k=k+1;
            trend.ch(i).harmonic(h).imag_MAX = gain(i)/2^15*( data_p(k) ); k=k+1;
            trend.ch(i).harmonic(h).imag_MIN = gain(i)/2^15*( data_p(k) ); k=k+1;
        end
        trend.ch(i).TWD_AVG = 100*( data_p(k) )/4096; k=k+1;
        trend.ch(i).TWD_MAX = 100*( data_p(k) )/4096; k=k+1;
        trend.ch(i).TWD_MIN = 100*( data_p(k) )/4096; k=k+1;
    end
    
    trend.v_seq_pos.real_AVG = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_pos.real_MAX = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_pos.real_MIN = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_pos.imag_AVG = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_pos.imag_MAX = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_pos.imag_MIN = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_neg.real_AVG = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_neg.real_MAX = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_neg.real_MIN = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_neg.imag_AVG = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_neg.imag_MAX = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_neg.imag_MIN = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_hom.real_AVG = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_hom.real_MAX = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_hom.real_MIN = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_hom.imag_AVG = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_hom.imag_MAX = gain(1)/2^15*( data_p(k) ); k=k+1;
    trend.v_seq_hom.imag_MIN = gain(1)/2^15*( data_p(k) ); k=k+1;
    
    trend.iA_seq_pos.real_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_pos.real_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_pos.real_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_pos.imag_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_pos.imag_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_pos.imag_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_neg.real_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_neg.real_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_neg.real_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_neg.imag_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_neg.imag_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_neg.imag_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_hom.real_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_hom.real_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_hom.real_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_hom.imag_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_hom.imag_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iA_seq_hom.imag_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    
    trend.iB_seq_pos.real_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_pos.real_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_pos.real_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_pos.imag_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_pos.imag_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_pos.imag_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_neg.real_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_neg.real_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_neg.real_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_neg.imag_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_neg.imag_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_neg.imag_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_hom.real_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_hom.real_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_hom.real_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_hom.imag_AVG = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_hom.imag_MAX = gain(5)/2^15*( data_p(k) ); k=k+1;
    trend.iB_seq_hom.imag_MIN = gain(5)/2^15*( data_p(k) ); k=k+1;
    
    trend.v_desb_neg.mod_AVG = 100*( data_p(k)/4096 ); k=k+1;
    trend.v_desb_neg.mod_MAX = 100*( data_p(k)/4096 ); k=k+1;
    trend.v_desb_neg.mod_MIN = 100*( data_p(k)/4096 ); k=k+1;
    trend.v_desb_hom.mod_AVG = 100*( data_p(k)/4096 ); k=k+1;
    trend.v_desb_hom.mod_MAX = 100*( data_p(k)/4096 ); k=k+1;
    trend.v_desb_hom.mod_MIN = 100*( data_p(k)/4096 ); k=k+1;

    trend.iA_desb_neg.mod_AVG = 100*( data_p(k)/4096 ); k=k+1;
    trend.iA_desb_neg.mod_MAX = 100*( data_p(k)/4096 ); k=k+1;
    trend.iA_desb_neg.mod_MIN = 100*( data_p(k)/4096 ); k=k+1;
    trend.iA_desb_hom.mod_AVG = 100*( data_p(k)/4096 ); k=k+1;
    trend.iA_desb_hom.mod_MAX = 100*( data_p(k)/4096 ); k=k+1;
    trend.iA_desb_hom.mod_MIN = 100*( data_p(k)/4096 ); k=k+1;

    trend.iB_desb_neg.mod_AVG = 100*( data_p(k)/4096 ); k=k+1;
    trend.iB_desb_neg.mod_MAX = 100*( data_p(k)/4096 ); k=k+1;
    trend.iB_desb_neg.mod_MIN = 100*( data_p(k)/4096 ); k=k+1;
    trend.iB_desb_hom.mod_AVG = 100*( data_p(k)/4096 ); k=k+1;
    trend.iB_desb_hom.mod_MAX = 100*( data_p(k)/4096 ); k=k+1;
    trend.iB_desb_hom.mod_MIN = 100*( data_p(k)/4096 ); k=k+1;
    
    gain = (400/2^15)*(400/2^15)*2^14;
    for i=1:6
         trend.ch_power(i).ApparentPower_AVG = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).ApparentPower_MAX = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).ApparentPower_MIN = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).RealPower_AVG = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).RealPower_MAX = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).RealPower_MIN = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).ReactivePower_AVG = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).ReactivePower_MAX = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).ReactivePower_MIN = gain*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).PowerFactor_AVG = 100/2^12*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).PowerFactor_MAX = 100/2^12*double(typecast(uint16(data_p(k)),'int16')); k=k+1;
         trend.ch_power(i).PowerFactor_MIN = 100/2^12*double(typecast(uint16(data_p(k)),'int16')); k=k+1;    
    end
end
