# Validation results

On 2026-10-10 IST, GitHub Actions recovery run 37986900514 losslessly decoded 62 bounded text chunks and pushed the original PNG to the same automation branch. Full gates ran explicitly on that decoded commit: C++ shared alert policy tests, 3 PNG transport tests, PNG/SVG/links/MIT/credential gates and NodeMCU ESP8266 build passed. Policy tests cover confirmation, cooldown, five-second output timeout, invalid/offline stop, explicit STOP and timer wrap.

PNG: 1480902 bytes, 1536×1024, SHA256 b02d234d49c855685e33fd13c14ac823c64806d0bbceeb702635a9b7add8eeb8. Transport chunks were removed. Target build used 28532 / 81920 bytes RAM and 279459 / 1044464 bytes flash.

Physical hardware, MQTT broker operation, HA mobile notification delivery and Matter pairing have not been tested. This documentation commit triggers final push checks; a PR is created only after they pass and merged only after final-head PR checks also pass.
