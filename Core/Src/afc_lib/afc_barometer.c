/*
 * afc_barometer.c
 *
 *  Created on: Dec 31, 2020
 *      Author: hendrik
 */


#include "afc_lib/afc_barometer.h"

#define BMP280_ADDR ((uint16_t)(0x76 << 1))
#define BMP280_CHIPID 0x58

#define BMP280_REGISTER_DIG_T1 0x88
#define BMP280_REGISTER_DIG_T2 0x8A
#define BMP280_REGISTER_DIG_T3 0x8C
#define BMP280_REGISTER_DIG_P1 0x8E
#define BMP280_REGISTER_DIG_P2 0x90
#define BMP280_REGISTER_DIG_P3 0x92
#define BMP280_REGISTER_DIG_P4 0x94
#define BMP280_REGISTER_DIG_P5 0x96
#define BMP280_REGISTER_DIG_P6 0x98
#define BMP280_REGISTER_DIG_P7 0x9A
#define BMP280_REGISTER_DIG_P8 0x9C
#define BMP280_REGISTER_DIG_P9 0x9E
#define BMP280_REGISTER_CHIPID 0xD0
#define BMP280_REGISTER_VERSION 0xD1
#define BMP280_REGISTER_SOFTRESET 0xE0
#define BMP280_REGISTER_CAL26 0xE1 /**< R calibration 0xE1-0xF0 */
#define BMP280_REGISTER_STATUS 0xF3
#define BMP280_REGISTER_CONTROL 0xF4
#define BMP280_REGISTER_CONFIG 0xF5
#define BMP280_REGISTER_PRESSUREDATA 0xF7
#define BMP280_REGISTER_TEMPDATA 0xFA

/** Oversampling rate for the sensor. */
enum sensor_sampling {
	/** No over-sampling. */
	SAMPLING_NONE = 0x00,
	/** 1x over-sampling. */
	SAMPLING_X1 = 0x01,
	/** 2x over-sampling. */
	SAMPLING_X2 = 0x02,
	/** 4x over-sampling. */
	SAMPLING_X4 = 0x03,
	/** 8x over-sampling. */
	SAMPLING_X8 = 0x04,
	/** 16x over-sampling. */
	SAMPLING_X16 = 0x05
};

/** Operating mode for the sensor. */
enum sensor_mode {
	/** Sleep mode. */
	MODE_SLEEP = 0x00,
	/** Forced mode. */
	MODE_FORCED = 0x01,
	/** Normal mode. */
	MODE_NORMAL = 0x03,
	/** Software reset. */
	MODE_SOFT_RESET_CODE = 0xB6
};

/** Filtering level for sensor data. */
enum sensor_filter {
	/** No filtering. */
	FILTER_OFF = 0x00,
	/** 2x filtering. */
	FILTER_X2 = 0x01,
	/** 4x filtering. */
	FILTER_X4 = 0x02,
	/** 8x filtering. */
	FILTER_X8 = 0x03,
	/** 16x filtering. */
	FILTER_X16 = 0x04
};

/** Standby duration in ms */
enum standby_duration {
	/** 1 ms standby. */
	STANDBY_MS_1 = 0x00,
	/** 62.5 ms standby. */
	STANDBY_MS_63 = 0x01,
	/** 125 ms standby. */
	STANDBY_MS_125 = 0x02,
	/** 250 ms standby. */
	STANDBY_MS_250 = 0x03,
	/** 500 ms standby. */
	STANDBY_MS_500 = 0x04,
	/** 1000 ms standby. */
	STANDBY_MS_1000 = 0x05,
	/** 2000 ms standby. */
	STANDBY_MS_2000 = 0x06,
	/** 4000 ms standby. */
	STANDBY_MS_4000 = 0x07
};

/** Encapsulates the config register */
struct config {
	/** Inactive duration (standby time) in normal mode */
	unsigned int t_sb : 3;
	/** Filter settings */
	unsigned int filter : 3;
	/** Unused - don't set */
	unsigned int none : 1;
	/** Enables 3-wire SPI */
	unsigned int spi3w_en : 1;
};

/** Encapsulates trhe ctrl_meas register */
struct ctrl_meas {
	/** Temperature oversampling. */
	unsigned int osrs_t : 3;
	/** Pressure oversampling. */
	unsigned int osrs_p : 3;
	/** Device mode */
	unsigned int mode : 2;
};


extern I2C_HandleTypeDef hi2c1;

afc_bar_bmp280_calib_t calib_data;
int32_t temperature_fine;

int16_t read_i16(uint8_t address) {
	int16_t result = 0;
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, address, 1, &address, 2, 200);

	return result;
}


HAL_StatusTypeDef afc_baro_init() {
	HAL_StatusTypeDef result;
	uint8_t buf;

	result = HAL_I2C_IsDeviceReady(&hi2c1, BMP280_ADDR, 3, 1000);
	if (result != HAL_OK) return result;

	result = HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_CHIPID, 1, &buf, 1, 1000);
	if (result != HAL_OK) return result;
	if (buf != BMP280_CHIPID) return HAL_ERROR;

	afc_baro_read_calibration_data(&calib_data);

	struct config _configReg;
	struct ctrl_meas _measReg;

	_measReg.mode = MODE_NORMAL;
	_measReg.osrs_t = SAMPLING_X16;
	_measReg.osrs_p = SAMPLING_X16;

	_configReg.filter = FILTER_OFF;
	_configReg.t_sb = STANDBY_MS_1;
	_configReg.spi3w_en = 0;

	uint8_t config = (_configReg.t_sb << 5) | (_configReg.filter << 2) | _configReg.spi3w_en;
	uint8_t meas = (_measReg.osrs_t << 5) | (_measReg.osrs_p << 2) | _measReg.mode;

	HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDR, BMP280_REGISTER_CONFIG, 1, &config, 1, 100);
	HAL_I2C_Mem_Write(&hi2c1, BMP280_ADDR, BMP280_REGISTER_CONTROL, 1, &meas, 1, 100);

	return HAL_OK;
}

HAL_StatusTypeDef afc_baro_deinit() {
	return HAL_OK;
}

void afc_baro_read_calibration_data(afc_bar_bmp280_calib_t *calibration_data){
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_T1, 1, (uint8_t*)&calibration_data->dig_T1, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_T2, 1, (uint8_t*)&calibration_data->dig_T2, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_T3, 1, (uint8_t*)&calibration_data->dig_T3, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P1, 1, (uint8_t*)&calibration_data->dig_P1, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P2, 1, (uint8_t*)&calibration_data->dig_P2, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P3, 1, (uint8_t*)&calibration_data->dig_P3, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P4, 1, (uint8_t*)&calibration_data->dig_P4, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P5, 1, (uint8_t*)&calibration_data->dig_P5, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P6, 1, (uint8_t*)&calibration_data->dig_P6, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P7, 1, (uint8_t*)&calibration_data->dig_P7, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P8, 1, (uint8_t*)&calibration_data->dig_P8, 2, 100);
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_DIG_P9, 1, (uint8_t*)&calibration_data->dig_P9, 2, 100);
}


float afc_baro_read_temperature() {
	int32_t var1, var2;
	uint8_t buf[3];

	int32_t adc_T;
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_TEMPDATA, 1, buf, 3, 300);
	adc_T = (buf[0] << 16) | (buf[1] << 8) | buf[2];
	adc_T >>= 4;

	var1 = ((((adc_T >> 3) - ((int32_t)calib_data.dig_T1 << 1))) * ((int32_t)calib_data.dig_T2)) >> 11;

	var2 = (((((adc_T >> 4) - ((int32_t)calib_data.dig_T1)) *
		   ((adc_T >> 4) - ((int32_t)calib_data.dig_T1))) >> 12) *
		   ((int32_t)calib_data.dig_T3)) >> 14;

	temperature_fine = var1 + var2;

	float T = (temperature_fine * 5 + 128) >> 8;
	return T / 100;
}

float afc_baro_read_pressure() {
	int64_t var1, var2, p;
	uint8_t buf[3];

	// Must be done first to get the t_fine variable set up
	afc_baro_read_temperature();

	int32_t adc_P;
	HAL_I2C_Mem_Read(&hi2c1, BMP280_ADDR, BMP280_REGISTER_PRESSUREDATA, 1, buf, 3, 300);
	adc_P = (buf[0] << 16) | (buf[1] << 8) | buf[2];
	adc_P >>= 4;

	var1 = ((int64_t)temperature_fine) - 128000;
	var2 = var1 * var1 * (int64_t)calib_data.dig_P6;
	var2 = var2 + ((var1 * (int64_t)calib_data.dig_P5) << 17);
	var2 = var2 + (((int64_t)calib_data.dig_P4) << 35);
	var1 = ((var1 * var1 * (int64_t)calib_data.dig_P3) >> 8) + ((var1 * (int64_t)calib_data.dig_P2) << 12);
	var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib_data.dig_P1) >> 33;

	if (var1 == 0) {
		return 0; // avoid exception caused by division by zero
	}

	p = 1048576 - adc_P;
	p = (((p << 31) - var2) * 3125) / var1;
	var1 = (((int64_t)calib_data.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
	var2 = (((int64_t)calib_data.dig_P8) * p) >> 19;

	p = ((p + var1 + var2) >> 8) + (((int64_t)calib_data.dig_P7) << 4);
	return (float)p / 256;
}
