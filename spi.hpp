#ifndef SPI_HPP_
#define SPI_HPP_

#include <cinttypes>
#include <string>

#include <linux/spi/spidev.h>

// TODO: make this a parameter
#define SPI_BUFFER_SIZE (128)

class SPI
{
public:
  SPI() : id_(-1), bits_per_word_ (8), mode_ (SPI_MODE_0),
    tx_buffer_free_ (true) {}
  SPI(const std::string &comport, const uint32_t clock);

  operator bool() const { return id_ != -1; }

  // Opens the SPI port "comport" with clock speed "clock".
  void Open(const std::string &comport, const uint32_t clock);

  // Transfer error status (boolean).
  bool TransferError(void) { return transfer_error_; }

  // Return the RX buffer if available.
  const uint8_t * GetRXBuffer(void);

  // Return the TX buffer if available.
  uint8_t * GetTXBuffer(void);

  // Send "tx_length" bytes of the TX buffer and attempt to receive "rx_length"
  // bytes from the slave.
  int Transfer(size_t rx_length, size_t tx_length);

  // Closes the SPI port.
  void Close();

private:
  int id_;
  uint8_t bits_per_word_, mode_;
  bool tx_buffer_free_;

  uint32_t speed_hz_;
  uint8_t rx_buffer_[SPI_BUFFER_SIZE], tx_buffer_[SPI_BUFFER_SIZE];
  bool transfer_error_;
};


#endif // SPI_HPP_
