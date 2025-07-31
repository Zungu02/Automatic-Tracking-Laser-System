## Possible pitfalls during Installation and setup
### ModuleNotFoundError: No module named 'cv2'
Since the specifice OpenCv 4.4.0 is used under BusterOS special setup is required to provide Python2 in the directory of OpenCV installation ,to solve this consult https://github.com/opencv/opencv
### Vnc server display "Cannot currently show the desktop"
When RealVNC shows “Cannot currently show the desktop”, it usually means the Pi isn’t actually running a graphical desktop on the display you’ve connected to. Here is the most common causes and how to fix this
Force a “dummy” HDMI so the desktop always runs
      
       sudo nano /boot/config.txt
Add at the bottom:
      
      hdmi_force_hotplug=1
      hdmi_group=2
      hdmi_mode=16   # or another mode matching your target resolution
Save and reboot:
      
      sudo reboot
## Arduino IDE installation 
### Using the official Linux ARM tarball (recommended)
Download the latest Arduino 1.8.x Linux ARM package

      cd ~/Downloads
      wget https://downloads.arduino.cc/arduino-1.8.19-linuxarm.tar.xz
Extract and install

      tar -xf arduino-1.8.19-linuxarm.tar.xz
      cd arduino-1.8.19
      sudo ./install.sh
This will copy the IDE into /usr/local and install menu shortcuts.

Launch Arduino
From your desktop menu or via terminal:

      arduino &

### Add the ESP32 Board Package(For use of driver board based on ESP32)
Open the IDE, then go to File → Preferences.In “Additional Boards Manager URLs”, paste:

      https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

###  Install ESP32 Cores
In Tools → Board → Boards Manager… choose  “esp32 by Espressif Systems”

### Select Your ESP32 Board
Tools → Board → ESP32 Arduino → choose “ESP32 Dev Module”

Tools → Port → dev/serial0

Tools → Flash Size → “4MB (32Mb)” (unless you know yours is larger)

Tools → PSRAM → Disabled (WROOM‑32UE does not have built‑in PSRAM; that’s on the WROVER modules)

Tools → Upload Speed → 115200 (or higher—e.g. 921600—if your USB‑serial adapter supports it)


