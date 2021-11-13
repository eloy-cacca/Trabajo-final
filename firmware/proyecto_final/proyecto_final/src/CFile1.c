/*
 * CFile1.c
 *
 * Created: 20/9/2021 11:06:36
 *  Author: eloy_
 */// ADC configuration

/* Tracking Time*/
#define TRACKING_TIME         1
/* Transfer Period */
#define TRANSFER_PERIOD       1
/** Total number of ADC channels in use */
#define NUM_CHANNELS    (2)
/** ADC convention done mask. */
#define ADC_DONE_MASK   ( (1<<NUM_CHANNELS) - 1 )
/** Size of the receive buffer and transmit buffer. */
#define BUFFER_SIZE     NUM_CHANNELS
/** The maximal digital value */
#define MAX_DIGITAL     (4095)
/** Reference voltage for ADC, in V. */
#define VOLT_REF (3)


/**************************** ENUMS, STRUCTS *********************************/
/** ADC sample data */ //creo una estructura de datos, con numero de canal, valor, y si esta hecha
struct {
	uint8_t uc_ch_num[NUM_CHANNELS];
	uint16_t us_value[NUM_CHANNELS];
	uint16_t us_done;
} adc_sample_data;


/**************************** LOCAL VARIABLES ********************************/
static adc_sample_data sample_data; //creo una variable del tipo


/**Channel list for sequence*/
enum adc_channel_num_t		ch_list[NUM_CHANNELS] = {
	// enumero los canales a utilizar
	ADC_TEMPERATURE_SENSOR,
	ADC_CHANNEL_7
}

/**************************** LOCAL FUNCTIONS ********************************/
static uint32_t adc_read_buffer(Adc * p_adc, uint16_t * p_s_buffer, uint32_t ul_size)
{
	/* Check if the first PDC bank is free. */
	if ((p_adc->ADC_RCR == 0) && (p_adc->ADC_RNCR == 0)) {
		p_adc->ADC_RPR = (uint32_t) p_s_buffer;
		p_adc->ADC_RCR = ul_size;
		p_adc->ADC_PTCR = ADC_PTCR_RXTEN;

		return 1;
	}
	 else 
	{ /* Check if the second PDC bank is free. */
		if (p_adc->ADC_RNCR == 0) {
			p_adc->ADC_RNPR = (uint32_t) p_s_buffer;
			p_adc->ADC_RNCR = ul_size;

			return 1;
		} 
		else 
		{
			return 0;
		}
	}
}


/**************************** GLOBAL FUNCTIONS *******************************/
/**
 * \brief Start ADC sample.
 * Initialize ADC, set clock and timing, and set ADC to given mode.
 */
static void start_adc(void)
{
	// Initialize sample variable
	memset((void *)&adc_sample_data, 0, sizeof(adc_sample_data)); //inicializo en 0
	
	/* Enable peripheral clock. */
	uint32_t i;
	pmc_enable_periph_clk(ID_ADC);

	/* Formula:
	*
	* Formula 1: ADCClock = MCK / ( (PRESCAL+1) * 2 )
	* For example, MCK = 64MHZ, PRESCAL = 4, then:
	* ADCClock = 64 / ((4+1) * 2) = 6.4MHz;
	
	 *     Startup  Time = startup value / ADCClock
	 *     Startup time = 64 / 6.4MHz = 10 us
	 */
	adc_init(ADC, sysclk_get_cpu_hz(), 6400000, ADC_STARTUP_TIME_4); /**< \brief (ADC_MR) 64 periods of ADCClock */


	 
	/* Set ADC timing. */
	/* Formula:
	 *     Transfer Time = (TRANSFER * 2 + 3) / ADCClock
	 *     Tracking Time = (TRACKTIM + 1) / ADCClock
	 *     Settling Time = settling value / ADCClock
	 *
	 *     Transfer Time = (1 * 2 + 3) / 6.4MHz = 781 ns
	 *     Tracking Time = (1 + 1) / 6.4MHz = 312 ns
	 *     Settling Time = 3 / 6.4MHz = 469 ns
	 */
	adc_configure_timing(ADC, TRACKING_TIME, ADC_SETTLING_TIME_3, TRANSFER_PERIOD);
	/* Formula:
	 *     Sample & Hold Time = SHTIM/ADCClock
	 *
	 *     Sample & Hold Time = 6 / 6.4 = 938 ns
	 */
	//adc_configure_timing(ADC, SAMPLE_HOLD_TIME);

/* Enable channel number tag. */
	adc_enable_tag(ADC);
	// habilito canal del ADC
	/* Enable channels. */
	adc_enable_channel(ADC, ADC_CHANNEL_7);
	adc_enable_channel(ADC, ADC_TEMPERATURE_SENSOR);
	
	
	/* Set gain and offset (only single ended mode used here). */
	adc_disable_anch(ADC); /* Disable analog change. */
	 for (i = 0; i < SETTINGS_CH_TOTAL_NUM; i++)
	 {
		 adc_set_channel_input_gain(ADC, ch_list[i], ADC_GAINVALUE_0);
		 adc_disable_channel_input_offset(ADC, ch_list[i]);
	 }
	/* Update channel number. */
	adc_sample_data.uc_ch_num[0] = ADC_CHANNEL_POTENTIOMETER;
	adc_sample_data.uc_ch_num[1] = ADC_TEMPERATURE_SENSOR;
	
	//AJUSTO LA GANANCIA EN 2
	// adc_enable_anch(ADC);
	/* gain = 2 */
	//adc_set_channel_input_gain(ADC, ADC_CHANNEL_POTENTIOMETER, ADC_GAINVALUE_2);
	// adc_enable_channel_input_offset(ADC, ADC_CHANNEL_POTENTIOMETER);

   /* Auto calibrate. */
    adc_set_calibmode(ADC);
    while (1)
    {
		if ((adc_get_status(ADC) & ADC_ISR_EOCAL) == ADC_ISR_EOCAL)
	    break;
    }
	
	/* Transfer with PDC. */
	adc_read_buffer(ADC, adc_sample_data.us_value, BUFFER_SIZE);
	/* Enable PDC channel interrupt. */
	adc_enable_interrupt(ADC, ADC_IER_RXBUFF);
	/* Enable ADC interrupt. */
	NVIC_EnableIRQ(ADC_IRQn);
	
	void ADC_Handler(void)
	{
		uint32_t i;
		uint32_t ul_temp;
		uint8_t uc_ch_num;

		/* With PDC transfer */
		
		if ((adc_get_status(ADC) & ADC_ISR_RXBUFF) == ADC_ISR_RXBUFF) {
				adc_sample_data.us_done = ADC_DONE_MASK;
				adc_read_buffer(ADC, adc_sample_data.us_value, BUFFER_SIZE);
				/* Only keep sample value, and discard channel number. */
				for (i = 0; i < NUM_CHANNELS; i++) {
					adc_sample_data.us_value[i] &= ADC_LCDR_LDATA_Msk;
				}
		}
			



	




