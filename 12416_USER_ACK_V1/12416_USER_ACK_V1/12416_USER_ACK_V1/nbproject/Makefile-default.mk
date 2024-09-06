#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/12416_USER_ACK_V1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/12416_USER_ACK_V1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS
SUB_IMAGE_ADDRESS_COMMAND=--image-address $(SUB_IMAGE_ADDRESS)
else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../Testing/test2/Amigo_LCD_1_0_0/framework/common/drv_spi_16bit.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/nvm_w25.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tft003.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_epmp.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_ssd1289.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_sh1101a_ssd1303.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210_buffer.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_ssd1926.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_bitbang.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_scroll_bar.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_text_entry.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_static_text.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_button.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_primitive.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_group_box.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_palette.c src/main.c src/system.c src/isr.c src/uart.c src/internal_resource_main.S src/internal_resource_main_reference.c src/cs1108.c src/modbus.c src/device.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o ${OBJECTDIR}/_ext/1673465902/nvm_w25.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o ${OBJECTDIR}/_ext/1332134585/gfx_gol.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o ${OBJECTDIR}/_ext/1332134585/gfx_palette.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/isr.o ${OBJECTDIR}/src/uart.o ${OBJECTDIR}/src/internal_resource_main.o ${OBJECTDIR}/src/internal_resource_main_reference.o ${OBJECTDIR}/src/cs1108.o ${OBJECTDIR}/src/modbus.o ${OBJECTDIR}/src/device.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o.d ${OBJECTDIR}/_ext/1673465902/nvm_w25.o.d ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o.d ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o.d ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o.d ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o.d ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o.d ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o.d ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o.d ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o.d ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o.d ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o.d ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o.d ${OBJECTDIR}/_ext/1332134585/gfx_gol.o.d ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o.d ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o.d ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o.d ${OBJECTDIR}/_ext/1332134585/gfx_palette.o.d ${OBJECTDIR}/src/main.o.d ${OBJECTDIR}/src/system.o.d ${OBJECTDIR}/src/isr.o.d ${OBJECTDIR}/src/uart.o.d ${OBJECTDIR}/src/internal_resource_main.o.d ${OBJECTDIR}/src/internal_resource_main_reference.o.d ${OBJECTDIR}/src/cs1108.o.d ${OBJECTDIR}/src/modbus.o.d ${OBJECTDIR}/src/device.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o ${OBJECTDIR}/_ext/1673465902/nvm_w25.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o ${OBJECTDIR}/_ext/1332134585/gfx_gol.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o ${OBJECTDIR}/_ext/1332134585/gfx_palette.o ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/system.o ${OBJECTDIR}/src/isr.o ${OBJECTDIR}/src/uart.o ${OBJECTDIR}/src/internal_resource_main.o ${OBJECTDIR}/src/internal_resource_main_reference.o ${OBJECTDIR}/src/cs1108.o ${OBJECTDIR}/src/modbus.o ${OBJECTDIR}/src/device.o

# Source Files
SOURCEFILES=../../Testing/test2/Amigo_LCD_1_0_0/framework/common/drv_spi_16bit.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/nvm_w25.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tft003.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_epmp.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_ssd1289.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_sh1101a_ssd1303.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210_buffer.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_ssd1926.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_bitbang.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_scroll_bar.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_text_entry.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_static_text.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_button.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_primitive.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_group_box.c ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_palette.c src/main.c src/system.c src/isr.c src/uart.c src/internal_resource_main.S src/internal_resource_main_reference.c src/cs1108.c src/modbus.c src/device.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/12416_USER_ACK_V1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ256DA210
MP_LINKER_FILE_OPTION=,--script=p24FJ256DA210.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/drv_spi_16bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1673465902" 
	@${RM} ${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o.d 
	@${RM} ${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/drv_spi_16bit.c  -o ${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1673465902/nvm_w25.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/nvm_w25.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1673465902" 
	@${RM} ${OBJECTDIR}/_ext/1673465902/nvm_w25.o.d 
	@${RM} ${OBJECTDIR}/_ext/1673465902/nvm_w25.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/nvm_w25.c  -o ${OBJECTDIR}/_ext/1673465902/nvm_w25.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1673465902/nvm_w25.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1673465902/nvm_w25.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tft003.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tft003.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_epmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_epmp.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_ssd1289.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_ssd1289.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_sh1101a_ssd1303.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_sh1101a_ssd1303.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210_buffer.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -Os -I"./src" -I"./src/system_config/pic24fj256da210_dev_board_qvgav1" -I"../framework" -I"./src/memory_programmer" -I"../framework/driver/spi" -msmart-io=1 -Wall -msfr-warn=off   -mlarge-arrays  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_ssd1926.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_ssd1926.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_bitbang.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_bitbang.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_scroll_bar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_scroll_bar.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_text_entry.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_text_entry.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_static_text.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_static_text.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_button.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_primitive.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_primitive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_primitive.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_primitive.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_primitive.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_group_box.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_group_box.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_palette.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_palette.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_palette.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_palette.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_palette.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_palette.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_palette.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_palette.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/system.o: src/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system.c  -o ${OBJECTDIR}/src/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/system.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/isr.o: src/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/isr.o.d 
	@${RM} ${OBJECTDIR}/src/isr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/isr.c  -o ${OBJECTDIR}/src/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/isr.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/isr.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/uart.o: src/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uart.o.d 
	@${RM} ${OBJECTDIR}/src/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart.c  -o ${OBJECTDIR}/src/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/uart.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/internal_resource_main_reference.o: src/internal_resource_main_reference.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/internal_resource_main_reference.o.d 
	@${RM} ${OBJECTDIR}/src/internal_resource_main_reference.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/internal_resource_main_reference.c  -o ${OBJECTDIR}/src/internal_resource_main_reference.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/internal_resource_main_reference.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/internal_resource_main_reference.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/cs1108.o: src/cs1108.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cs1108.o.d 
	@${RM} ${OBJECTDIR}/src/cs1108.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cs1108.c  -o ${OBJECTDIR}/src/cs1108.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/cs1108.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/cs1108.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/modbus.o: src/modbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/modbus.o.d 
	@${RM} ${OBJECTDIR}/src/modbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/modbus.c  -o ${OBJECTDIR}/src/modbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/modbus.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/modbus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/device.o: src/device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/device.o.d 
	@${RM} ${OBJECTDIR}/src/device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/device.c  -o ${OBJECTDIR}/src/device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/device.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/drv_spi_16bit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1673465902" 
	@${RM} ${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o.d 
	@${RM} ${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/drv_spi_16bit.c  -o ${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1673465902/drv_spi_16bit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1673465902/nvm_w25.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/nvm_w25.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1673465902" 
	@${RM} ${OBJECTDIR}/_ext/1673465902/nvm_w25.o.d 
	@${RM} ${OBJECTDIR}/_ext/1673465902/nvm_w25.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/common/nvm_w25.c  -o ${OBJECTDIR}/_ext/1673465902/nvm_w25.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1673465902/nvm_w25.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1673465902/nvm_w25.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_da210.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tft003.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tft003.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_tft003.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_epmp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_epmp.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_epmp.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_ssd1289.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_ssd1289.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_ssd1289.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_sh1101a_ssd1303.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_sh1101a_ssd1303.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_sh1101a_ssd1303.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_da210_buffer.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -Os -I"./src" -I"./src/system_config/pic24fj256da210_dev_board_qvgav1" -I"../framework" -I"./src/memory_programmer" -I"../framework/driver/spi" -msmart-io=1 -Wall -msfr-warn=off   -mlarge-arrays  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_da210_buffer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_ssd1926.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_ssd1926.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_ssd1926.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_bitbang.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2137234866" 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o.d 
	@${RM} ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/driver/gfx/src/drv_gfx_tcon_bitbang.c  -o ${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/2137234866/drv_gfx_tcon_bitbang.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_scroll_bar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_scroll_bar.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_scroll_bar.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_text_entry.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_text_entry.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_text_entry.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_static_text.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_static_text.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_static_text.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_button.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_button.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_primitive.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_primitive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_primitive.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_primitive.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_primitive.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_primitive.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_group_box.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_gol_group_box.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_gol_group_box.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1332134585/gfx_palette.o: ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_palette.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1332134585" 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_palette.o.d 
	@${RM} ${OBJECTDIR}/_ext/1332134585/gfx_palette.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../Testing/test2/Amigo_LCD_1_0_0/framework/gfx/src/gfx_palette.c  -o ${OBJECTDIR}/_ext/1332134585/gfx_palette.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1332134585/gfx_palette.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/_ext/1332134585/gfx_palette.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/main.o: src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/main.o.d 
	@${RM} ${OBJECTDIR}/src/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/main.c  -o ${OBJECTDIR}/src/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/system.o: src/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/system.o.d 
	@${RM} ${OBJECTDIR}/src/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/system.c  -o ${OBJECTDIR}/src/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/system.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/isr.o: src/isr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/isr.o.d 
	@${RM} ${OBJECTDIR}/src/isr.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/isr.c  -o ${OBJECTDIR}/src/isr.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/isr.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/isr.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/uart.o: src/uart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/uart.o.d 
	@${RM} ${OBJECTDIR}/src/uart.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/uart.c  -o ${OBJECTDIR}/src/uart.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/uart.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/uart.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/internal_resource_main_reference.o: src/internal_resource_main_reference.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/internal_resource_main_reference.o.d 
	@${RM} ${OBJECTDIR}/src/internal_resource_main_reference.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/internal_resource_main_reference.c  -o ${OBJECTDIR}/src/internal_resource_main_reference.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/internal_resource_main_reference.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/internal_resource_main_reference.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/cs1108.o: src/cs1108.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cs1108.o.d 
	@${RM} ${OBJECTDIR}/src/cs1108.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/cs1108.c  -o ${OBJECTDIR}/src/cs1108.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/cs1108.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/cs1108.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/modbus.o: src/modbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/modbus.o.d 
	@${RM} ${OBJECTDIR}/src/modbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/modbus.c  -o ${OBJECTDIR}/src/modbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/modbus.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/modbus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/src/device.o: src/device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/device.o.d 
	@${RM} ${OBJECTDIR}/src/device.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  src/device.c  -o ${OBJECTDIR}/src/device.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/device.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mlarge-code -O0 -I"../../../../../../../../../../Program Files (x86)/Microchip/xc16/v1.36/include" -I"src" -I"../framework" -I"../framework/driver/spi" -I"../framework/driver/nvm_w25q" -I"../framework/common" -msmart-io=1 -Wall -msfr-warn=off    -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/device.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/internal_resource_main.o: src/internal_resource_main.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/internal_resource_main.o.d 
	@${RM} ${OBJECTDIR}/src/internal_resource_main.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/internal_resource_main.S  -o ${OBJECTDIR}/src/internal_resource_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/internal_resource_main.o.d"  -D__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../src" -I"../../../../../framework" -I"../src/system_config/pic24fj256da210_dev_board_qvgav1" -I"../src/memory_programmer" -I"../../../../../framework/usb/inc" -Wa,-MD,"${OBJECTDIR}/src/internal_resource_main.o.asm.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/internal_resource_main.o.d" "${OBJECTDIR}/src/internal_resource_main.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/internal_resource_main.o: src/internal_resource_main.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/internal_resource_main.o.d 
	@${RM} ${OBJECTDIR}/src/internal_resource_main.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  src/internal_resource_main.S  -o ${OBJECTDIR}/src/internal_resource_main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/src/internal_resource_main.o.d"  -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  -I"../src" -I"../../../../../framework" -I"../src/system_config/pic24fj256da210_dev_board_qvgav1" -I"../src/memory_programmer" -I"../../../../../framework/usb/inc" -Wa,-MD,"${OBJECTDIR}/src/internal_resource_main.o.asm.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)  -mdfp=${DFP_DIR}/xc16
	@${FIXDEPS} "${OBJECTDIR}/src/internal_resource_main.o.d" "${OBJECTDIR}/src/internal_resource_main.o.asm.d"  -t $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/12416_USER_ACK_V1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/12416_USER_ACK_V1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)   -mreserve=data@0x800:0x81B -mreserve=data@0x81C:0x81D -mreserve=data@0x81E:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x827 -mreserve=data@0x82A:0x84F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--heap=4000,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp=${DFP_DIR}/xc16 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/12416_USER_ACK_V1.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/12416_USER_ACK_V1.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--heap=4000,--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,--report-mem,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp=${DFP_DIR}/xc16 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/12416_USER_ACK_V1.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp=${DFP_DIR}/xc16 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
