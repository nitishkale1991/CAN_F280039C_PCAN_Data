################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1007925551: ../c2000.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/ccs2041/ccs/utils/sysconfig_1.26.0/sysconfig_cli.bat" -s "C:/ti/c2000/C2000Ware_6_00_01_00/.metadata/sdk.json" -d "F28003x" -p "100PZ" -r "F28003x_100PZ" --script "C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/c2000.syscfg" -o "syscfg" --compiler ccs
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/board.c: build-1007925551 ../c2000.syscfg
syscfg/board.h: build-1007925551
syscfg/board.cmd.genlibs: build-1007925551
syscfg/board.opt: build-1007925551
syscfg/board.json: build-1007925551
syscfg/pinmux.csv: build-1007925551
syscfg/c2000ware_libraries.cmd.genlibs: build-1007925551
syscfg/c2000ware_libraries.opt: build-1007925551
syscfg/c2000ware_libraries.c: build-1007925551
syscfg/c2000ware_libraries.h: build-1007925551
syscfg/clocktree.h: build-1007925551
syscfg: build-1007925551

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit" --include_path="C:/ti/c2000/C2000Ware_6_00_01_00" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/device" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/device/driverlib" --include_path="C:/ti/c2000/C2000Ware_6_00_01_00/driverlib/f28003x/driverlib/" --include_path="C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --define=DEBUG --define=RAM --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/CPU1_RAM/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit" --include_path="C:/ti/c2000/C2000Ware_6_00_01_00" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/device" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/device/driverlib" --include_path="C:/ti/c2000/C2000Ware_6_00_01_00/driverlib/f28003x/driverlib/" --include_path="C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --define=DEBUG --define=RAM --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/CPU1_RAM/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


