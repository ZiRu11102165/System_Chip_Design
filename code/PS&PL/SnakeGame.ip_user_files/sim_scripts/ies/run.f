-makelib ies_lib/xil_defaultlib -sv \
  "C:/Xilinx/Vivado/2017.3/data/ip/xpm/xpm_cdc/hdl/xpm_cdc.sv" \
  "C:/Xilinx/Vivado/2017.3/data/ip/xpm/xpm_fifo/hdl/xpm_fifo.sv" \
  "C:/Xilinx/Vivado/2017.3/data/ip/xpm/xpm_memory/hdl/xpm_memory.sv" \
-endlib
-makelib ies_lib/xpm \
  "C:/Xilinx/Vivado/2017.3/data/ip/xpm/xpm_VCOMP.vhd" \
-endlib
-makelib ies_lib/axi_infrastructure_v1_1_0 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/ec67/hdl/axi_infrastructure_v1_1_vl_rfs.v" \
-endlib
-makelib ies_lib/smartconnect_v1_0 -sv \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/d5d3/hdl/sc_util_v1_0_vl_rfs.sv" \
-endlib
-makelib ies_lib/axi_protocol_checker_v2_0_0 -sv \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/d5c1/hdl/axi_protocol_checker_v2_0_vl_rfs.sv" \
-endlib
-makelib ies_lib/axi_vip_v1_1_0 -sv \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/8b42/hdl/axi_vip_v1_1_vl_rfs.sv" \
-endlib
-makelib ies_lib/processing_system7_vip_v1_0_2 -sv \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/e0a2/hdl/processing_system7_vip_v1_0_vl_rfs.sv" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/SnakeGame_BD/ip/SnakeGame_BD_processing_system7_0_0/sim/SnakeGame_BD_processing_system7_0_0.v" \
-endlib
-makelib ies_lib/axi_lite_ipif_v3_0_4 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/cced/hdl/axi_lite_ipif_v3_0_vh_rfs.vhd" \
-endlib
-makelib ies_lib/lib_cdc_v1_0_2 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/ef1e/hdl/lib_cdc_v1_0_rfs.vhd" \
-endlib
-makelib ies_lib/interrupt_control_v3_1_4 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/8e66/hdl/interrupt_control_v3_1_vh_rfs.vhd" \
-endlib
-makelib ies_lib/axi_gpio_v2_0_16 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/e9c1/hdl/axi_gpio_v2_0_vh_rfs.vhd" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/SnakeGame_BD/ip/SnakeGame_BD_axi_gpio_0_0/sim/SnakeGame_BD_axi_gpio_0_0.vhd" \
  "../../../bd/SnakeGame_BD/ip/SnakeGame_BD_axi_gpio_1_0/sim/SnakeGame_BD_axi_gpio_1_0.vhd" \
-endlib
-makelib ies_lib/proc_sys_reset_v5_0_12 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/f86a/hdl/proc_sys_reset_v5_0_vh_rfs.vhd" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/SnakeGame_BD/ip/SnakeGame_BD_rst_ps7_0_100M_0/sim/SnakeGame_BD_rst_ps7_0_100M_0.vhd" \
-endlib
-makelib ies_lib/generic_baseblocks_v2_1_0 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/b752/hdl/generic_baseblocks_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/axi_register_slice_v2_1_14 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/a259/hdl/axi_register_slice_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/fifo_generator_v13_2_0 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/0798/simulation/fifo_generator_vlog_beh.v" \
-endlib
-makelib ies_lib/fifo_generator_v13_2_0 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/0798/hdl/fifo_generator_v13_2_rfs.vhd" \
-endlib
-makelib ies_lib/fifo_generator_v13_2_0 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/0798/hdl/fifo_generator_v13_2_rfs.v" \
-endlib
-makelib ies_lib/axi_data_fifo_v2_1_13 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/74ae/hdl/axi_data_fifo_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/axi_crossbar_v2_1_15 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/a1b8/hdl/axi_crossbar_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/SnakeGame_BD/ip/SnakeGame_BD_xbar_0/sim/SnakeGame_BD_xbar_0.v" \
-endlib
-makelib ies_lib/axi_protocol_converter_v2_1_14 \
  "../../../../SnakeGame.srcs/sources_1/bd/SnakeGame_BD/ipshared/33cc/hdl/axi_protocol_converter_v2_1_vl_rfs.v" \
-endlib
-makelib ies_lib/xil_defaultlib \
  "../../../bd/SnakeGame_BD/ip/SnakeGame_BD_auto_pc_0/sim/SnakeGame_BD_auto_pc_0.v" \
  "../../../bd/SnakeGame_BD/sim/SnakeGame_BD.v" \
-endlib
-makelib ies_lib/xil_defaultlib \
  glbl.v
-endlib

