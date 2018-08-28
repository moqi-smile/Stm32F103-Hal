#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
 extern "C" {
#endif

typedef enum 
{
	Spi1 = 0,
	Spi2,
	Spi3,
	Spinum,
} Spi_t;

void Spi_Init(void);
void Spi_Receive(Spi_t Spix, uint8_t *pData, uint16_t Size, uint32_t Timeout);
void Spi_Transmit(Spi_t Spix, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#ifdef __cplusplus
}
#endif

#endif
/* __SPI_H__ */
