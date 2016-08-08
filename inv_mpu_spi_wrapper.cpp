#include "inv_mpu_spi_wrapper.hpp"

#include <cstring>

#include "spi.hpp"


static SPI spi_;


int inv_mpu_spi_wrapper_init(void)
{
  spi_.Open("/dev/spidev3.0", 1000000);
}

// -----------------------------------------------------------------------------
int inv_mpu_spi_wrapper_close(void)
{
  spi_.Close();
}


// -----------------------------------------------------------------------------
extern "C" int i2c_read(unsigned char slave_addr, unsigned char reg_addr,
  unsigned char length, unsigned char *data)
{
  unsigned char * tx_buffer = spi_.GetTXBuffer();
  if (!tx_buffer) return (-1);

  tx_buffer[0] = reg_addr | 0x80;

  int ret = spi_.Transfer(1 + length, 1);
  if (ret < 1) return ret;

  const unsigned char * rx_buffer = spi_.GetRXBuffer();
  memcpy(data, &rx_buffer[1], length);

  return 0;
}

// -----------------------------------------------------------------------------
extern "C" int i2c_write(unsigned char slave_addr, unsigned char reg_addr,
  unsigned char length, unsigned char const *data)
{
  unsigned char * tx_buffer = spi_.GetTXBuffer();
  if (!tx_buffer) return (-1);

  tx_buffer[0] = reg_addr;
  memcpy(&tx_buffer[1], data, length);

  int ret = spi_.Transfer(0, 1 + length);
  if (ret < 1) return ret;

  return 0;
}
