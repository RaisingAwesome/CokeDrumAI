/***************************************************************************************************
   Name: OLED.c
   Sphere OS: 19.05
****************************************************************************************************/

#include "oled.h"
#include <math.h>
#include <stdint.h>

uint8_t oled_state = 0;

// Data of sensors (Acceleration, Gyro, Temperature, Presure)
sensor_var OLED_sensor_data_display;

// Data of network status
network_var network_data;

// Data of light sensor
float light_sensor;

// Altitude
extern float altitude;

// Array with messages from Azure
extern uint8_t oled_ms1[CLOUD_MSG_SIZE];
extern uint8_t oled_ms2[CLOUD_MSG_SIZE];
extern uint8_t oled_ms3[CLOUD_MSG_SIZE];
extern uint8_t oled_ms4[CLOUD_MSG_SIZE];

// Status variables of I2C bus and RT core
extern uint8_t RTCore_status;
extern uint8_t lsm6dso_status;
extern uint8_t lps22hh_status;

/**
  * @brief  OLED initialization.
  * @param  None.
  * @retval Positive if was unsuccefully, zero if was succefully.
  */
uint8_t oled_init()
{
	return sd1306_init();
}

// State machine to change the OLED status
void update_oled()
{
	switch (oled_state)  //was oled_state
	{
		case 0:
		{
			oled_draw_logo();
		}
		break;
		case 1:
		{
			oled_draw_logo(); //update_network();
		}
		break;
		case 2:
		{
			oled_draw_logo();
		}
		break;
		case 3:
		{
			update_accel(OLED_sensor_data_display.acceleration_mg[0], OLED_sensor_data_display.acceleration_mg[1], OLED_sensor_data_display.acceleration_mg[2]);
		}
		break;
		case 4:
		{
			update_angular_rate(OLED_sensor_data_display.angular_rate_dps[0], OLED_sensor_data_display.angular_rate_dps[1], OLED_sensor_data_display.angular_rate_dps[2]);
		}
		break;
		case 5:
		{
			update_environ(OLED_sensor_data_display.lsm6dsoTemperature_degC, OLED_sensor_data_display.lps22hhTemperature_degC, OLED_sensor_data_display.lps22hhpressure_hPa);
		}
		break;
		case 6:
		{
			update_other(light_sensor, 0, 0); 
		}
		break;
		case 7:
		{
			oled_draw_logo();
		}
		break;

		default:
		break;
	}
}

/**
  * @brief  Template to show I2C bus status
  * @param  sensor_number: Sensor number
  * @param  sensor_status: Sensor status
  * @retval None.
  */
void oled_i2c_bus_status(uint8_t sensor_number)
{

	// Strings for labels
	uint8_t str_bus_sta[]   = "I2C Bus Status:";
	uint8_t str_lsmod_sta[] = "Accelerometer:";
	uint8_t str_lps22_sta[] = "Barometer:";
	uint8_t str_rtcore_sta[] = "Real Time Core:";

	switch (sensor_number)
	{
		case 0:
		{
			// Clear OLED buffer
			clear_oled_buffer();

			// Draw the title
			sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "I2C Status", FONT_SIZE_TITLE, white_pixel);

			// Draw a label at line 1
			sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_bus_sta, FONT_SIZE_LINE, white_pixel);

			// I2C bus OK, if not OLED doesn't show a image
			sd1306_draw_string(sizeof(str_bus_sta) * 6, OLED_LINE_1_Y, "OK", FONT_SIZE_LINE, white_pixel);
		}
		break;
		case 1:
		{
			// Draw a label at line 2
			sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_lsmod_sta, FONT_SIZE_LINE, white_pixel);

			// Show LSMOD status
			if (lsm6dso_status == 0)
			{
				sd1306_draw_string(sizeof(str_lsmod_sta) * 6, OLED_LINE_2_Y, "OK", FONT_SIZE_LINE, white_pixel);
			}
			else
			{
				sd1306_draw_string(sizeof(str_lsmod_sta) * 6, OLED_LINE_2_Y, "ERROR", FONT_SIZE_LINE, white_pixel);
			}
		}
		break;
		case 2:
		{
			// Draw a label at line 3
			sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_lps22_sta, FONT_SIZE_LINE, white_pixel);

			// Show LPS22 status
			if (lps22hh_status == 0)
			{
				sd1306_draw_string(sizeof(str_lps22_sta) * 6, OLED_LINE_3_Y, "OK", FONT_SIZE_LINE, white_pixel);
			}
			else
			{
				sd1306_draw_string(sizeof(str_lps22_sta) * 6, OLED_LINE_3_Y, "ERROR", FONT_SIZE_LINE, white_pixel);
			}
		}
		break;
		case 3:
		{
			// If we are here I2C is working well

			// Clear OLED buffer
			clear_oled_buffer();

			// Draw the title
			sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "I2C Status", FONT_SIZE_TITLE, white_pixel);

			// Draw a label at line 1
			sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_bus_sta, FONT_SIZE_LINE, white_pixel);

			// I2C bus OK, if not OLED doesn't show a image
			sd1306_draw_string(sizeof(str_bus_sta) * 6, OLED_LINE_1_Y, "OK", FONT_SIZE_LINE, white_pixel);

			// Draw a label at line 2
			sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_lsmod_sta, FONT_SIZE_LINE, white_pixel);

			// Show LSMOD status
			if (lsm6dso_status == 1)
			{
				sd1306_draw_string(sizeof(str_lsmod_sta) * 6, OLED_LINE_2_Y, "OK", FONT_SIZE_LINE, white_pixel);
			}
			else
			{
				sd1306_draw_string(sizeof(str_lsmod_sta) * 6, OLED_LINE_2_Y, "ERROR", FONT_SIZE_LINE, white_pixel);
			}

			// Draw a label at line 3
			sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_lps22_sta, FONT_SIZE_LINE, white_pixel);

			// Show LPS22 status
			if (lps22hh_status == 1)
			{
				sd1306_draw_string(sizeof(str_lps22_sta) * 6, OLED_LINE_3_Y, "OK", FONT_SIZE_LINE, white_pixel);
			}
			else
			{
				sd1306_draw_string(sizeof(str_lps22_sta) * 6, OLED_LINE_3_Y, "ERROR", FONT_SIZE_LINE, white_pixel);
			}
			
			// Draw a label at line 4
			sd1306_draw_string(OLED_LINE_4_X, OLED_LINE_4_Y, str_rtcore_sta, FONT_SIZE_LINE, white_pixel);

			// Show RTcore status
			if ( RTCore_status == 0)
			{
				sd1306_draw_string(sizeof(str_rtcore_sta) * 6, OLED_LINE_4_Y, "OK", FONT_SIZE_LINE, white_pixel);
			}
			else
			{
				sd1306_draw_string(sizeof(str_rtcore_sta) * 6, OLED_LINE_4_Y, "ERROR", FONT_SIZE_LINE, white_pixel);
			}
		}
		break;
		default:
		break;
	}

	// Send the buffer to OLED RAM
	sd1306_refresh();
}

/**
  * @brief  Get the channel at given frequency
  * @param  freq_MHz: Frequency in MHz
  * @retval Channel.
  */
uint16_t get_channel(uint16_t freq_MHz)
{
	if (freq_MHz < 5000 && freq_MHz > 2400)
	{
		// channel of in 2.4 GHz band
		freq_MHz -= 2407;
	}
	else if(freq_MHz > 5000)
	{
		// channel of in 5 GHz band
		freq_MHz -= 5000;
	}
	else
	{
		// channel not in 2.4 or 5 GHz bands
		freq_MHz = 0;
	}

	freq_MHz /= 5;

	return freq_MHz;
}

/**
  * @brief  Template to show Network status
  * @param  None
  * @retval None.
  */
void update_network()
{
	uint8_t string_data[10];
	uint16_t channel;
	uint8_t aux_size;
	
	// Strings for labels
	uint8_t str_SSID[] = "SSID:";
	uint8_t str_freq[] = "Freq:";
	uint8_t str_RSSI[] = "RSSI:";
	uint8_t str_chan[] = "Chan:";

	uint8_t aux_data_str[] = "%d";

	// Clear oled buffer
	clear_oled_buffer();

	// Draw the title
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "  Network", FONT_SIZE_TITLE, white_pixel);

	// Draw a label at line 1
	sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_SSID, FONT_SIZE_LINE, white_pixel);
	// Draw SSID string
	sd1306_draw_string(sizeof(str_SSID)*6, OLED_LINE_1_Y, network_data.SSID, FONT_SIZE_LINE, white_pixel);


	// Draw a label at line 2
	sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_freq, FONT_SIZE_LINE, white_pixel);

	// Convert frequency value to string
	intToStr(network_data.frequency_MHz, string_data, 1);

	// Draw frequency value
	sd1306_draw_string(sizeof(str_freq) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units
	//sd1306_draw_string(sizeof(str_freq) * 6 + (get_str_size(string_data)+1) * 6, OLED_LINE_2_Y, "MHz", FONT_SIZE_LINE, white_pixel);


	// Draw channel label at line 2
	sd1306_draw_string(sizeof(str_freq) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, str_chan, FONT_SIZE_LINE, white_pixel);

	channel = get_channel(network_data.frequency_MHz);

	aux_size = get_str_size(string_data);

	// Convert frequency value to string
	intToStr(channel, string_data, 1);

	// Draw channel value
	sd1306_draw_string(sizeof(str_freq) * 6 + (aux_size + sizeof(str_chan)+ 1) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);

	// Draw a label at line 3
	sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_RSSI, FONT_SIZE_LINE, white_pixel);

	// Convert RSSI value to string (Currently RSSI is always zero)
	intToStr(network_data.rssi, string_data, 1);

	strcpy(string_data, "%d");
	snprintf(string_data, 10, aux_data_str, network_data.rssi);

	// Draw RSSI value
	sd1306_draw_string(sizeof(str_RSSI) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);

	// Draw dBm unit
	sd1306_draw_string(sizeof(str_freq) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "dBm", FONT_SIZE_LINE, white_pixel);

	// Send the buffer to OLED RAM
	sd1306_refresh();
}

/**
  * @brief  Template to show Acceleration data
  * @param  x: Acceleration in axis X
  * @param  y: Acceleration in axis Y
  * @param  z: Acceleration in axis Z
  * @retval None.
  */
void update_accel(float x, float y, float z)
{
	uint32_t i;
	uint8_t string_data[10];

	// Strings for labels
	uint8_t str_ax[] = "Axis X:";
	uint8_t str_ay[] = "Axis Y:";
	uint8_t str_az[] = "Axis Z:";

	// Clear OLED buffer
	clear_oled_buffer();

	// Draw the title
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "   Accel.", FONT_SIZE_TITLE, white_pixel);

	// Convert x value to string
	ftoa(x, string_data, 2);

	// Draw a label at line 1
	sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_ax, FONT_SIZE_LINE, white_pixel);
	// Draw the value of x
	sd1306_draw_string(sizeof(str_ax) * 6, OLED_LINE_1_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of x
	sd1306_draw_string(sizeof(str_ax) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_1_Y, "mg", FONT_SIZE_LINE, white_pixel);

	// Convert y value to string
	ftoa(y, string_data, 2);

	// Draw a label at line 2
	sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_ay, FONT_SIZE_LINE, white_pixel);
	// Draw the value of y
	sd1306_draw_string(sizeof(str_az) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of y
	sd1306_draw_string(sizeof(str_ay) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "mg", FONT_SIZE_LINE, white_pixel);

	// Convert z value to string
	ftoa(z, string_data, 2);

	// Draw a label at line 3
	sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_az, FONT_SIZE_LINE, white_pixel);
	// Draw the value of z
	sd1306_draw_string(sizeof(str_az) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of z
	sd1306_draw_string(sizeof(str_az) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "mg", FONT_SIZE_LINE, white_pixel);

	// Send the buffer to OLED RAM
	sd1306_refresh();
}

/**
  * @brief  Template to show Angular rate data
  * @param  x: Angular rate in axis X
  * @param  y: Angular rate in axis Y
  * @param  z: Angular rate in axis Z
  * @retval None.
  */
void update_angular_rate(float x, float y, float z)
{
	uint32_t i;
	uint8_t string_data[10];

	// Strings for labels
	uint8_t str_gx[] = "GX:";
	uint8_t str_gy[] = "GY:";
	uint8_t str_gz[] = "GZ:";

	// Clear OLED buffer
	clear_oled_buffer();

	// Draw the title
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "   Gyro.", FONT_SIZE_TITLE, white_pixel);

	// Convert x value to string
	ftoa(x, string_data, 2);

	// Draw a label at line 1
	sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_gx, FONT_SIZE_LINE, white_pixel);
	// Draw the value of x
	sd1306_draw_string(sizeof(str_gx) * 6, OLED_LINE_1_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of x
	sd1306_draw_string(sizeof(str_gx) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_1_Y, "dps", FONT_SIZE_LINE, white_pixel);

	// Convert y value to string
	ftoa(y, string_data, 2);

	// Draw a label at line 2
	sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_gy, FONT_SIZE_LINE, white_pixel);
	// Draw the value of y
	sd1306_draw_string(sizeof(str_gy) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of y
	sd1306_draw_string(sizeof(str_gy) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "dps", FONT_SIZE_LINE, white_pixel);
	
	// Convert z value to string
	ftoa(z, string_data, 2);

	// Draw a label at line 3
	sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_gz, FONT_SIZE_LINE, white_pixel);
	// Draw the value of z
	sd1306_draw_string(sizeof(str_gz) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of z
	sd1306_draw_string(sizeof(str_gz) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "dps", FONT_SIZE_LINE, white_pixel);

	// Send the buffer to OLED RAM
	sd1306_refresh();
}

/**
  * @brief  Template to show Enviromental data
  * @param  temp1: Temperature 1 in celsius degrees
  * @param  temp2: Temperature 2 in celsius degrees
  * @param  atm: Presure in hPa
  * @retval None.
  */
void update_environ(float temp1, float temp2, float atm)
{
	uint32_t i;
	uint8_t string_data[10];
	
	// Strings for labels
	uint8_t str_temp1[] = "Temp1:";
	uint8_t str_temp2[] = "Temp2:";
	uint8_t str_atm[] = "Barom:";
	uint8_t str_altitude[] = "Elev :";

	// Clear OLED buffer
	clear_oled_buffer();

	// Draw the title
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "  Environ.", FONT_SIZE_TITLE, white_pixel);

	// Convert temp1 value to string
	ftoa(((temp1*1.8) + 32), string_data, 2);

	// Draw a label at line 1
	sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_temp1, FONT_SIZE_LINE, white_pixel);
	// Draw the value of temp1
	sd1306_draw_string(sizeof(str_temp1) * 6, OLED_LINE_1_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of temp1
	sd1306_draw_string(sizeof(str_temp1) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_1_Y, "°F", FONT_SIZE_LINE, white_pixel);

	// Convert temp2 value to string
	ftoa(((temp2*1.8)+32), string_data, 2);

	// Draw a label at line 2
	sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_temp2, FONT_SIZE_LINE, white_pixel);
	// Draw the value of temp2
	sd1306_draw_string(sizeof(str_temp2) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the value of temp2
	sd1306_draw_string(sizeof(str_temp2) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "°F", FONT_SIZE_LINE, white_pixel);

	// Convert atm value to string
	ftoa(atm, string_data, 2);

	// Draw a label at line 3
	sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_atm, FONT_SIZE_LINE, white_pixel);
	// Draw the value of atm
	sd1306_draw_string(sizeof(str_atm) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of atm
	sd1306_draw_string(sizeof(str_atm) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "hPa", FONT_SIZE_LINE, white_pixel);

	// Convert altitude value to string
	ftoa(altitude*3.2808399, string_data, 2);

	// Draw a label at line 4
	sd1306_draw_string(OLED_LINE_4_X, OLED_LINE_4_Y, str_altitude, FONT_SIZE_LINE, white_pixel);
	// Draw the value of altitude
	sd1306_draw_string(sizeof(str_altitude) * 6, OLED_LINE_4_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of altitude
	sd1306_draw_string(sizeof(str_altitude) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_4_Y, "ft", FONT_SIZE_LINE, white_pixel);

	// Send the buffer to OLED RAM
	sd1306_refresh();
}

/**
  * @brief  Template to show other variables curently not available
  * @param  x: var 1
  * @param  y: var 2
  * @param  z: var 3
  * @retval None.
  */
void update_other(float x, float y, float z)
{
	uint32_t i;
	uint8_t string_data[10];

	// Strings for labels
	uint8_t str_light[] = "Light:";
	uint8_t str_tbd1[] = "TBD 1:";
	uint8_t str_tbd2[] = "TBD 2:";

	// Clear OLED buffer
	clear_oled_buffer();

	// Draw the title
	sd1306_draw_string(OLED_TITLE_X, OLED_TITLE_Y, "   Other", FONT_SIZE_TITLE, white_pixel);

	// Convert x value to string
	ftoa(x, string_data, 2);

	// Draw a label at line 1
	sd1306_draw_string(OLED_LINE_1_X, OLED_LINE_1_Y, str_light, FONT_SIZE_LINE, white_pixel);
	// Draw the value of x
	sd1306_draw_string(sizeof(str_light) * 6, OLED_LINE_1_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of x
	sd1306_draw_string(sizeof(str_light) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_1_Y, "Lux", FONT_SIZE_LINE, white_pixel);

	// Convert y value to string
	ftoa(y, string_data, 2);

	// Draw a label at line 2
	sd1306_draw_string(OLED_LINE_2_X, OLED_LINE_2_Y, str_tbd1, FONT_SIZE_LINE, white_pixel);
	// Draw the value of y
	sd1306_draw_string(sizeof(str_tbd1) * 6, OLED_LINE_2_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of y
	sd1306_draw_string(sizeof(str_tbd1) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_2_Y, "Units", FONT_SIZE_LINE, white_pixel);

	// Convert z value to string
	ftoa(z, string_data, 2);

	// Draw a label at line 3
	sd1306_draw_string(OLED_LINE_3_X, OLED_LINE_3_Y, str_tbd2, FONT_SIZE_LINE, white_pixel);
	// Draw the value of z
	sd1306_draw_string(sizeof(str_tbd2) * 6, OLED_LINE_3_Y, string_data, FONT_SIZE_LINE, white_pixel);
	// Draw the units of z
	sd1306_draw_string(sizeof(str_tbd2) * 6 + (get_str_size(string_data) + 1) * 6, OLED_LINE_3_Y, "Units", FONT_SIZE_LINE, white_pixel);

	// Send the buffer to OLED RAM
	sd1306_refresh();
}

/**
  * @brief  Template to show a logo
  * @param  None.
  * @retval None.
  */
void oled_draw_logo(void)
{
	// Copy image_avnet to OLED buffer
	sd1306_draw_img(Image_avnet_bmp);

	// Send the buffer to OLED RAM
	sd1306_refresh();
}

// reverses a string 'str' of length 'len' 
static void reverse(uint8_t *str, int32_t len)
{
	int32_t i = 0;
	int32_t j = len - 1;
	int32_t temp;

	while (i < j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}

/**
  * @brief  Converts a given integer x to string uint8_t[]
  * @param  x: x integer input
  * @param  str: uint8_t array output
  * @param  d: Number of zeros added
  * @retval i: number of digits
  */
int32_t intToStr(int32_t x, uint8_t str[], int32_t d)
{
	int32_t i = 0;
	uint8_t flag_neg = 0;

	if (x < 0)
	{
		flag_neg = 1;
		x *= -1;
	}
	while (x)
	{
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	// If number of digits required is more, then 
	// add 0s at the beginning 
	while (i < d)
	{
		str[i++] = '0';
	}

	if (flag_neg)
	{
		str[i] = '-';
		i++;
	}

	reverse(str, i);
	str[i] = '\0';
	return i;
}

/**
  * @brief  Converts a given integer x to string uint8_t[]
  * @param  n: float number to convert
  * @param  res:
  * @param  afterpoint:
  * @retval None.
  */
void ftoa(float n, uint8_t *res, int32_t afterpoint)
{
	// Extract integer part 
	int32_t ipart = (int32_t)n;

	// Extract floating part 
	float fpart = n - (float)ipart;

	int32_t i;

	if (ipart < 0)
	{
		res[0] = '-';
		res++;
		ipart *= -1;
	}

	if (fpart < 0)
	{
		fpart *= -1;

		if (ipart == 0)
		{
			res[0] = '-';
			res++;
		}
	}

	// convert integer part to string 
	i = intToStr(ipart, res, 1);

	// check for display option after point 
	if (afterpoint != 0)
	{
		res[i] = '.';  // add dot 

		// Get the value of fraction part upto given no. 
		// of points after dot. The third parameter is needed 
		// to handle cases like 233.007 
		fpart = fpart * pow(10, afterpoint);

		intToStr((int32_t)fpart, res + i + 1, afterpoint);
	}
}

// AVNET logo

const unsigned char Image_avnet_bmp[BUFFER_SIZE] =

{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0x7C, 0x1F, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x20,
0x70, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xF0, 0xF0, 0x70, 0x00, 0x00, 0xE0, 0xF0, 0xE0, 0x00, 0x00,
0x00, 0xE0, 0xF0, 0xE0, 0x00, 0x00, 0xF0, 0xF0, 0xF0, 0xE0, 0xF0, 0x70, 0xF0, 0xF0, 0xE0, 0x00,
0xC0, 0xE0, 0xE0, 0x70, 0x70, 0x70, 0x70, 0xF0, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xE0, 0xFC, 0xFE, 0x3F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x1F, 0x7F, 0xFE, 0xE0, 0x00, 0xC0,
0xE0, 0xF0, 0xFC, 0xFE, 0xEF, 0xE7, 0xE1, 0xE0, 0x40, 0x00, 0x00, 0x3F, 0x7F, 0xFF, 0xE0, 0xE0,
0xE0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x7F, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x1F,
0x3F, 0x7F, 0xFD, 0xEE, 0xEE, 0xE7, 0xE7, 0xF3, 0x73, 0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF8, 0x7C, 0xFC, 0xF8, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0xFC, 0xFC, 0xFC, 0x1C, 0x1C, 0x1C, 0x1C,
0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x80, 0xE0, 0xF8, 0xFE, 0xFF, 0xDF, 0xC7, 0xC1, 0xE0, 0xEF, 0xFF, 0xFF, 0xE0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xC0, 0xF8, 0xFF, 0x3F, 0x1F, 0x07, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x07, 0x7F, 0xFF, 0xFC,
0xC0, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0,
0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x1C, 0x3F, 0x7F, 0x73, 0x71, 0x71, 0x71, 0x71, 0xF1, 0xE1, 0xE1, 0x80, 0x00, 0xF0,
0xF8, 0xF8, 0x70, 0x38, 0x38, 0x38, 0xF8, 0xF0, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xF0, 0x78,
0x38, 0x38, 0xF8, 0xF8, 0xF0, 0x00, 0x80, 0xE0, 0xF0, 0xF0, 0x38, 0x38, 0xB8, 0xB8, 0xF8, 0xF0,
0xE0, 0x00, 0x00, 0xF8, 0xF8, 0xF8, 0x70, 0x78, 0x38, 0xF8, 0xF8, 0xF0, 0x80, 0xE0, 0xF0, 0xF0,
0x38, 0x38, 0xB8, 0xB8, 0xF8, 0xF0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
0x01, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x1C, 0x3C, 0x7C, 0x78, 0x70, 0x70, 0x70, 0x70, 0x78, 0x3C, 0x3F, 0x1F, 0x0F, 0x00, 0xFF,
0xFF, 0xFF, 0x70, 0x70, 0x70, 0x78, 0x3F, 0x3F, 0x0F, 0x00, 0x00, 0x3F, 0x7F, 0x3F, 0x00, 0x00,
0x00, 0x00, 0x3F, 0x7F, 0x3F, 0x00, 0x0F, 0x1F, 0x3F, 0x7E, 0x77, 0x77, 0x73, 0x73, 0x79, 0x39,
0x18, 0x00, 0x00, 0x3F, 0x7F, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x3F, 0x7E,
0x77, 0x77, 0x73, 0x73, 0x79, 0x39, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
0x1F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

uint8_t get_str_size(uint8_t * str)
{
	uint8_t legth = 0;
	while (*(str) != NULL)
	{
		str++;
		legth++;
	}
	return legth;
}