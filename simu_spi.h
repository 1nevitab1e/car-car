/*
 * simu_spi.h
 *
 *  Created on: 2023Äê4ÔÂ12ÈÕ
 *      Author: 86131
 */

#ifndef SIMU_SPI_H_
#define SIMU_SPI_H_

#include "zf_common_headfile.h"

#define simu_spi_MOSI E2
#define simu_spi_MISO E3
#define simu_spi_SCK E4
#define simu_spi_RES E5
#define simu_spi_CS E6

void simu_spi_w_mosi(uint8 bit);
void simu_spi_w_sck(uint8 bit);
void simu_spi_w_cs(uint8 bit);
uint8 simu_spi_r_miso(void);
void simu_spi_gpio_init();
void simu_spi_start();
void simu_spi_stop();
uint8 simu_spi_swapbyte(uint8 ByteSend);

#endif /* SIMU_SPI_H_ */
