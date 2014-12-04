/*
 * LightSensor.cpp
 *
 *  Created on: 28.11.2014
 *      Author: holger
 */

#include "LightSensor.h"

#define SLAVE_ADDRESS 0x29

#define PWR_ADDRESS 0x00
#define PWR_ON_CMD 0x03

#define INTEGRATE_ADDRESS 0x81
#define INTEGRATE_CMD_101MS 0x01
#define INTEGRATE_CMD_402MS 0x02
#define INTEGRATE_CMD_13MS 0x00

#define CH0_ADDRESS 0xAC
#define CH1_ADDRESS 0xAE

#define TSL2561_LUX_LUXSCALE (14) // Scale by 2^14
#define TSL2561_LUX_RATIOSCALE (9) // Scale ratio by 2^9
#define TSL2561_LUX_CHSCALE (10) // Scale channel values by 2^10
#define TSL2561_LUX_CHSCALE_TINT0 (0x7517) // 322/11 * 2^TSL2561_LUX_CHSCALE
#define TSL2561_LUX_CHSCALE_TINT1 (0x0FE7) // 322/81 * 2^TSL2561_LUX_CHSCALE

#define TSL2561_LUX_K1C (0x0043) // 0.130 * 2^RATIO_SCALE
#define TSL2561_LUX_B1C (0x0204) // 0.0315 * 2^LUX_SCALE
#define TSL2561_LUX_M1C (0x01ad) // 0.0262 * 2^LUX_SCALE
#define TSL2561_LUX_K2C (0x0085) // 0.260 * 2^RATIO_SCALE
#define TSL2561_LUX_B2C (0x0228) // 0.0337 * 2^LUX_SCALE
#define TSL2561_LUX_M2C (0x02c1) // 0.0430 * 2^LUX_SCALE
#define TSL2561_LUX_K3C (0x00c8) // 0.390 * 2^RATIO_SCALE
#define TSL2561_LUX_B3C (0x0253) // 0.0363 * 2^LUX_SCALE
#define TSL2561_LUX_M3C (0x0363) // 0.0529 * 2^LUX_SCALE
#define TSL2561_LUX_K4C (0x010a) // 0.520 * 2^RATIO_SCALE
#define TSL2561_LUX_B4C (0x0282) // 0.0392 * 2^LUX_SCALE
#define TSL2561_LUX_M4C (0x03df) // 0.0605 * 2^LUX_SCALE
#define TSL2561_LUX_K5C (0x014d) // 0.65 * 2^RATIO_SCALE
#define TSL2561_LUX_B5C (0x0177) // 0.0229 * 2^LUX_SCALE
#define TSL2561_LUX_M5C (0x01dd) // 0.0291 * 2^LUX_SCALE
#define TSL2561_LUX_K6C (0x019a) // 0.80 * 2^RATIO_SCALE
#define TSL2561_LUX_B6C (0x0101) // 0.0157 * 2^LUX_SCALE
#define TSL2561_LUX_M6C (0x0127) // 0.0180 * 2^LUX_SCALE
#define TSL2561_LUX_K7C (0x029a) // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B7C (0x0037) // 0.00338 * 2^LUX_SCALE
#define TSL2561_LUX_M7C (0x002b) // 0.00260 * 2^LUX_SCALE
#define TSL2561_LUX_K8C (0x029a) // 1.3 * 2^RATIO_SCALE
#define TSL2561_LUX_B8C (0x0000) // 0.000 * 2^LUX_SCALE
#define TSL2561_LUX_M8C (0x0000) // 0.000 * 2^LUX_SCALE

Topic<uint32_t> lightTopic(-1, "LightTopic");

LightSensor::LightSensor(const char* name, HAL_I2C *i2c,  uint64_t periode){
	this->i2c = i2c;

	//default integration time = 400ms
	if(periode < 100 * MILLISECONDS)
		this->periode = 100 * MILLISECONDS;
	else
		this->periode = periode;
}
void LightSensor::init(){
	xprintf("start lightsensor init\n");
	int32_t error = i2c->init();
	if(error == 0)
		xprintf("i2c successfully initialised\n");
	else
		xprintf("i2c ERROR@init\n");

	uint8_t pwr_cmd[2] = { PWR_ADDRESS, PWR_ON_CMD };
	//int32_t retVal = i2c->write(SLAVE_ADDRESS, pwr_cmd, 2);
	uint8_t buf[1] = { 0 };

	int32_t retVal = i2c->writeRead(SLAVE_ADDRESS, pwr_cmd, 2, buf, 1);

	xprintf("i2c light init \npowerUp retVal: %d \n",retVal);
	xprintf("answer: %d(=%d), right answer: %d(t/f)\n", buf[0], PWR_ON_CMD, buf[0] == PWR_ON_CMD);

	//setting custom integration time -> 101ms
	uint8_t integrate_cmd[2] = { INTEGRATE_ADDRESS , INTEGRATE_CMD_13MS };
	retVal = i2c->write(SLAVE_ADDRESS, integrate_cmd, 2);
	xprintf("i2c light integr. retVal: %d \n", retVal);

}
void LightSensor::run(){
	int32_t err[2] = { 0 };

	uint8_t channel0[2] = {0};
	uint8_t channel1[2] = {0};

	uint16_t channel0_combined = 0;
	uint16_t channel1_combined = 0;

	uint8_t channel0CMD[1] = { CH0_ADDRESS };
	uint8_t channel1CMD[1] = { CH1_ADDRESS };

	uint32_t lux = 0;

	TIME_LOOP(0, periode){


		err[0] = i2c->writeRead(SLAVE_ADDRESS, channel0CMD, 1, channel0, 2);
		err[1] = i2c->writeRead(SLAVE_ADDRESS, channel1CMD, 1, channel1, 2);


		if(err[1] > 0 && err[0 > 0]){
			channel0_combined = (channel0[0] << 8) + channel0[1];
			channel1_combined = (channel1[0] << 8) + channel1[1];

//			xprintf("ch0: %d, %d\n",  channel0[0], channel0[1]);
//			xprintf("ch1: %d, %d\n",  channel1[0], channel1[1]);
//
//			xprintf("combined val of ch0: %d\n",  channel0_combined);
//			xprintf("combined val of ch1: %d\n", channel1_combined);

			lux =  calculateLux(channel0_combined, channel1_combined);
			lightTopic.publish(lux);

			xprintf("lux: %d\n", lux);

		}else{
			xprintf("err@reading i2c sun registers\n");
			xprintf("reg0: %d, reg1: %d\n", err[0], err[1]);
		}

	}
}

uint32_t LightSensor::calculateLux(uint16_t ch0, uint16_t ch1)
{
	unsigned long chScale;
	unsigned long channel1;
	unsigned long channel0;


	//INT0 for 13ms, INT1 for 103ms
	chScale = TSL2561_LUX_CHSCALE_TINT0;

	//std mode = integration every 400ms
	//chScale = (1 << TSL2561_LUX_CHSCALE);


	channel0 = (ch0 * chScale) >> TSL2561_LUX_CHSCALE;
	channel1 = (ch1 * chScale) >> TSL2561_LUX_CHSCALE;
	// find the ratio of the channel values (Channel1/Channel0)
	unsigned long ratio1 = 0;
	if (channel0 != 0) ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE+1)) / channel0;
	// round the ratio value
	unsigned long ratio = (ratio1 + 1) >> 1;
	unsigned int b, m;

	if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1C))
	{b=TSL2561_LUX_B1C; m=TSL2561_LUX_M1C;}
	else if (ratio <= TSL2561_LUX_K2C)
	{b=TSL2561_LUX_B2C; m=TSL2561_LUX_M2C;}
	else if (ratio <= TSL2561_LUX_K3C)
	{b=TSL2561_LUX_B3C; m=TSL2561_LUX_M3C;}
	else if (ratio <= TSL2561_LUX_K4C)
	{b=TSL2561_LUX_B4C; m=TSL2561_LUX_M4C;}
	else if (ratio <= TSL2561_LUX_K5C)
	{b=TSL2561_LUX_B5C; m=TSL2561_LUX_M5C;}
	else if (ratio <= TSL2561_LUX_K6C)
	{b=TSL2561_LUX_B6C; m=TSL2561_LUX_M6C;}
	else if (ratio <= TSL2561_LUX_K7C)
	{b=TSL2561_LUX_B7C; m=TSL2561_LUX_M7C;}
	else if (ratio > TSL2561_LUX_K8C)
	{b=TSL2561_LUX_B8C; m=TSL2561_LUX_M8C;}

	unsigned long temp;
	temp = ((channel0 * b) - (channel1 * m));
	// do not allow negative lux value
	if (temp < 0) temp = 0;
	// round lsb (2^(LUX_SCALE-1))
	temp += (1 << (TSL2561_LUX_LUXSCALE-1));
	// strip off fractional portion
	uint32_t lux = temp >> TSL2561_LUX_LUXSCALE;
	// Signal I2C had no errors
	return lux;
}
