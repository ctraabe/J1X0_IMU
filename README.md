J1X0_IMU
==========

Sample code for setting and reading the MPU9250 IMU on the AUVIDEA J100 and J120
carrier board for the Jetson TX1

Prerequisites:
--------------

#### SPI enabled kernel

A custom kernel may be desirable, but as a starting point, you may also use the
kernel provided by AUVIDEA here:

http://www.auvidea.eu/index.php/support

(Look for "J100/J120 firmware")

Follow the instructions in the included README file.

#### Give permission to use the SPI device

Create a file called:

/etc/udev/rules.d/99-j100-devices.rules

Add the following contents to the newly created file:

<pre><code>
ACTION=="remove" GOTO="j100_end"
KERNEL=="spidev3.0" OWNER="root" GROUP="dialout" MODE="0660"
LABEL="j100_end"
</code></pre>

Make sure that your account belongs to the "dialout" group

<pre><code>
sudo usermod -a [your_username] -G dialout
</code></pre>

Building:
---------

The included makefile will build using the standard gnu toolchain

<pre><code>
make
</code></pre>
