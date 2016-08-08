#include "spi.hpp"

#include <iostream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>


SPI::SPI(const std::string &device, const uint32_t clock)
  : id_(-1)
  , bits_per_word_ (8)
  , mode_ (SPI_MODE_0)
  , tx_buffer_free_ (true)
{
  Open(device, clock);
}

// -----------------------------------------------------------------------------
void SPI::Open(const std::string &device, const uint32_t clock)
{
  speed_hz_ = clock;

  id_ = open(device.c_str(), O_RDWR);
  if (id_ == -1) {
    std::cerr << "Failed to open " << device << "." << std::endl;
    return;
  }

  // Set SPI mode.
  int error = ioctl(id_, SPI_IOC_WR_MODE, &mode_);
  if (error == -1)
  {
    Close();
    std::cerr << "ERROR: Unable to set mode on " << device << "." << std::endl;
    return;
  }

  // Set bits per word.
  error = ioctl(id_, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word_);
  if (error == -1)
  {
    Close();
    std::cerr << "ERROR: Unable to set bits per word on " << device << "."
      << std::endl;
    return;
  }

  // Set clock speed (Hz).
  error = ioctl(id_, SPI_IOC_WR_MAX_SPEED_HZ, &clock);
  if (error == -1)
  {
    Close();
    std::cerr << "ERROR: Unable to set clock on " << device << "." << std::endl;
    return;
  }
}

// -----------------------------------------------------------------------------
const uint8_t * SPI::GetRXBuffer(void)
{
  if (transfer_error_) return NULL;

  return rx_buffer_;
}

// -----------------------------------------------------------------------------
uint8_t * SPI::GetTXBuffer(void)
{
  if (!tx_buffer_free_) return NULL;

  tx_buffer_free_ = false;
  return tx_buffer_;
}

// -----------------------------------------------------------------------------
// Start an SPI exchange.
int SPI::Transfer(size_t rx_length, size_t tx_length)
{
  uint32_t exchange_length = rx_length > tx_length ? rx_length : tx_length;

  // Pad the TX_buffer with dummy data.
  if (exchange_length > tx_length)
    for (size_t i = tx_length; i < exchange_length; i++) tx_buffer_[i] = 0xFF;

  struct spi_ioc_transfer tr;
  tr.tx_buf = (unsigned long)tx_buffer_;
  tr.rx_buf = (unsigned long)rx_buffer_;
  tr.len = exchange_length;
  tr.delay_usecs = 0;
  tr.speed_hz = speed_hz_;
  tr.bits_per_word = bits_per_word_;

  int ret = ioctl(id_, SPI_IOC_MESSAGE(1), &tr);
  transfer_error_ = ret < 1;

  tx_buffer_free_ = true;

  return ret;
}

// -----------------------------------------------------------------------------
void SPI::Close()
{
  close(id_);
  id_ = -1;
}
