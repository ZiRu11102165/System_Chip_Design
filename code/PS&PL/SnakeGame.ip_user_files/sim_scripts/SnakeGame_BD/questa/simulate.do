onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib SnakeGame_BD_opt

do {wave.do}

view wave
view structure
view signals

do {SnakeGame_BD.udo}

run -all

quit -force
