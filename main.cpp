#include <iostream>
#include <time.h>

#include "inv_mpu_spi_wrapper.hpp"

extern "C" {
  #include "inv_mpu.h"
}

using namespace std;


#define LOOP_FREQUENCY (2)


static volatile int received_sigterm = 0;
static volatile int received_nb_signals = 0;


static void sigterm_handler(int sig)
{
  received_sigterm = sig;
  received_nb_signals++;
  if (received_nb_signals > 3) exit(123);
}

int main(int argc, char *argv[])
{
  const timespec req = {0, 1000000000L / LOOP_FREQUENCY};
  timespec rem; // remainder of interrupted nanosleep (unused);
  struct int_param_s int_param;

  inv_mpu_spi_wrapper_init();
  mpu_init(&int_param);
  mpu_set_sensors(INV_XYZ_ACCEL);

  while (received_sigterm == 0)
  {
    // Sleep.
    nanosleep(&req, &rem);

    // Read the raw accelerometer.
    short accelerometer[3];
    int ret = mpu_get_accel_reg(accelerometer, NULL);

    cout << ret << "\t" << accelerometer[0] << "\t" << accelerometer[1] << "\t"
      << accelerometer[2] << endl;
  }

  inv_mpu_spi_wrapper_close();

  return 0;
}
