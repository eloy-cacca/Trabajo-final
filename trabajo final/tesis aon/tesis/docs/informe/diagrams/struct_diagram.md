classDiagram
    signals --> signals_t
    processed_signals --> sp_t
    sp_t --> sp_channel_t
    sp_channel_t --> sp_complex_t
    index_accum --> index_accum_t
    index_accum_t --> index_accum_ch_t
    index_accum_ch_t --> index_complex32_t
    index_result --> index_result_t
    index_result_t --> index_result_ch_t
    index_result_ch_t --> index_complex16_t
    trend_accum --> trend_accum_t
    trend_accum_t --> trend_accum_ch_t
    trend_accum_ch_t --> trend_complex32_t
    trend_result --> trend_result_t
    trend_result_t --> trend_result_ch_t
    trend_result_ch_t --> trend_complex16_t
  
    class signals_t{
      int32_t : Q15 : [4]
    }

    class sp_t{
      ch[4] : sp_channel_t
    }

    class sp_channel_t{
      int64_t : Q30 : rms2
      sp_complex_t : harmonic[5]
    }

    class sp_complex_t{
      int64_t : Q33 : real
      int64_t : Q33 : imag
    }

    class index_accum_t{
      index_accum_ch_t : ch[4]
    }

    class index_accum_ch_t{
      int64_t : Q30 : rms2
      index_complex32_t : harmonic[5]
    }

    class index_complex32_t{
      int32_t : Q15 : real
      int32_t : Q15 : imag
    }

    class index_result_t{
      index_result_ch_t : ch[4]
    }

    class index_result_ch_t{
      int16_t : Q15 : rms
      int16_t : Q?? : twd
      index_complex16_t : harmonic[5]
    }

    class index_complex16_t{
      int16_t : Q15 : real
      int16_t : Q15 : imag
    }

    class trend_accum_t{
      trend_accum_ch_t : ch[4]
    }

    class trend_accum_ch_t{
      int64_t : Q30 : rms2[3]
      int64_t : Q?? : twd2[3]
      trend_complex32_t : harmonic[5]
    }

    class trend_complex32_t{
      int32_t : Q15 : real[3]
      int32_t : Q15 : imag[3]
    }

    class trend_result_t{
      trend_result_ch_t : ch[4]
    }

    class trend_result_ch_t{
      int16_t : Q15 : rms[3]
      int16_t : Q?? : twd[3]
      trend_complex16_t : harmonic[5]
    }

    class trend_complex16_t{
      int16_t : Q15 : real[3]
      int16_t : Q15 : imag[3]
    }