#ifndef INV_MPU_SPI_WRAPPER_HPP_
#define INV_MPU_SPI_WRAPPER_HPP_


int inv_mpu_spi_wrapper_init(void);

int inv_mpu_spi_wrapper_close(void);

extern "C" int i2c_read(unsigned char slave_addr, unsigned char reg_addr,
  unsigned char length, unsigned char *data);

extern "C" int i2c_write(unsigned char slave_addr, unsigned char reg_addr,
  unsigned char length, unsigned char const *data);


#endif // INV_MPU_SPI_WRAPPER_HPP_
