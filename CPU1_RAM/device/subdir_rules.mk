################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
device/%.obj: ../device/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit" --include_path="C:/ti/c2000/C2000Ware_6_00_01_00" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/device" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/device/driverlib" --include_path="C:/ti/c2000/C2000Ware_6_00_01_00/driverlib/f28003x/driverlib/" --include_path="C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --define=DEBUG --define=RAM --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/CPU1_RAM/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

device/%.obj: ../device/%.asm $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcrc -Ooff --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit" --include_path="C:/ti/c2000/C2000Ware_6_00_01_00" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/device" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/device/driverlib" --include_path="C:/ti/c2000/C2000Ware_6_00_01_00/driverlib/f28003x/driverlib/" --include_path="C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-c2000_22.6.3.LTS/include" --define=DEBUG --define=RAM --diag_suppress=10063 --diag_warning=225 --diag_wrap=off --display_error_number --gen_func_subsections=on --abi=eabi --preproc_with_compile --preproc_dependency="device/$(basename $(<F)).d_raw" --include_path="C:/Users/NitishKale/workspace_ccstheia/mcan_ex7_classic_transmit/CPU1_RAM/syscfg" --obj_directory="device" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


