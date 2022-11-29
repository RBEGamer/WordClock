# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/pico-sdk/tools/pioasm"
  "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/pioasm"
  "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/pico-sdk/src/rp2_common/cyw43_driver/pioasm"
  "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/pico-sdk/src/rp2_common/cyw43_driver/pioasm/tmp"
  "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src"
  "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/marcelochsendorf/Downloads/WordClockNew/src/base_rp2040_picosdk/wordclock/pico-sdk/src/rp2_common/cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()