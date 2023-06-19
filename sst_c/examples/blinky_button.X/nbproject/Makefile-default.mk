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
FINAL_IMAGE=${DISTDIR}/blinky_button.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/blinky_button.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=D:/projs/blinky_button.X/blinky1.c D:/projs/blinky_button.X/blinky3.c D:/projs/blinky_button.X/button2b.c D:/projs/blinky_button.X/bsp_dspic33ep128gs804.c D:/projs/blinky_button.X/button2a.c D:/projs/Super-Simple-Tasker/sst_c/ports/dspic/sst_port.c D:/projs/Super-Simple-Tasker/sst_c/src/sst.c D:/projs/blinky_button.X/main.c D:/projs/blinky_button.X/system_dspic33ep128gs804.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1889579238/blinky1.o ${OBJECTDIR}/_ext/1889579238/blinky3.o ${OBJECTDIR}/_ext/1889579238/button2b.o ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o ${OBJECTDIR}/_ext/1889579238/button2a.o ${OBJECTDIR}/_ext/149885617/sst_port.o ${OBJECTDIR}/_ext/1731567061/sst.o ${OBJECTDIR}/_ext/1889579238/main.o ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1889579238/blinky1.o.d ${OBJECTDIR}/_ext/1889579238/blinky3.o.d ${OBJECTDIR}/_ext/1889579238/button2b.o.d ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o.d ${OBJECTDIR}/_ext/1889579238/button2a.o.d ${OBJECTDIR}/_ext/149885617/sst_port.o.d ${OBJECTDIR}/_ext/1731567061/sst.o.d ${OBJECTDIR}/_ext/1889579238/main.o.d ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1889579238/blinky1.o ${OBJECTDIR}/_ext/1889579238/blinky3.o ${OBJECTDIR}/_ext/1889579238/button2b.o ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o ${OBJECTDIR}/_ext/1889579238/button2a.o ${OBJECTDIR}/_ext/149885617/sst_port.o ${OBJECTDIR}/_ext/1731567061/sst.o ${OBJECTDIR}/_ext/1889579238/main.o ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o

# Source Files
SOURCEFILES=D:/projs/blinky_button.X/blinky1.c D:/projs/blinky_button.X/blinky3.c D:/projs/blinky_button.X/button2b.c D:/projs/blinky_button.X/bsp_dspic33ep128gs804.c D:/projs/blinky_button.X/button2a.c D:/projs/Super-Simple-Tasker/sst_c/ports/dspic/sst_port.c D:/projs/Super-Simple-Tasker/sst_c/src/sst.c D:/projs/blinky_button.X/main.c D:/projs/blinky_button.X/system_dspic33ep128gs804.c



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/blinky_button.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP128GS804
MP_LINKER_FILE_OPTION=,--script=p33EP128GS804.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1889579238/blinky1.o: D:/projs/blinky_button.X/blinky1.c  .generated_files/flags/default/4013b298cc0ba4e9ac05aa38406d6eafb103dcfe .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/blinky1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/blinky1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/blinky1.c  -o ${OBJECTDIR}/_ext/1889579238/blinky1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/blinky1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/blinky3.o: D:/projs/blinky_button.X/blinky3.c  .generated_files/flags/default/154a07d6564ab0fec40ffa4d908e65be4871edbc .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/blinky3.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/blinky3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/blinky3.c  -o ${OBJECTDIR}/_ext/1889579238/blinky3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/blinky3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/button2b.o: D:/projs/blinky_button.X/button2b.c  .generated_files/flags/default/a98db815dab011d610f23623bc9ad4b87207b713 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/button2b.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/button2b.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/button2b.c  -o ${OBJECTDIR}/_ext/1889579238/button2b.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/button2b.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o: D:/projs/blinky_button.X/bsp_dspic33ep128gs804.c  .generated_files/flags/default/351ebe65d881729dab2c838a7891476379e174df .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/bsp_dspic33ep128gs804.c  -o ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/button2a.o: D:/projs/blinky_button.X/button2a.c  .generated_files/flags/default/acbc6f7a6efb2d7433c7e1b9a47ebafb02634c5a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/button2a.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/button2a.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/button2a.c  -o ${OBJECTDIR}/_ext/1889579238/button2a.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/button2a.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/149885617/sst_port.o: D:/projs/Super-Simple-Tasker/sst_c/ports/dspic/sst_port.c  .generated_files/flags/default/5927313e3f00413a02069d03cec1d441a684d862 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/149885617" 
	@${RM} ${OBJECTDIR}/_ext/149885617/sst_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/149885617/sst_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/Super-Simple-Tasker/sst_c/ports/dspic/sst_port.c  -o ${OBJECTDIR}/_ext/149885617/sst_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/149885617/sst_port.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1731567061/sst.o: D:/projs/Super-Simple-Tasker/sst_c/src/sst.c  .generated_files/flags/default/f920a13b6676debff03c7ae180b9693c6ba56b5d .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1731567061" 
	@${RM} ${OBJECTDIR}/_ext/1731567061/sst.o.d 
	@${RM} ${OBJECTDIR}/_ext/1731567061/sst.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/Super-Simple-Tasker/sst_c/src/sst.c  -o ${OBJECTDIR}/_ext/1731567061/sst.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1731567061/sst.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/main.o: D:/projs/blinky_button.X/main.c  .generated_files/flags/default/85a4f4fc8c1df5de0dbd2fe1be3da7b8daee7529 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/main.c  -o ${OBJECTDIR}/_ext/1889579238/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o: D:/projs/blinky_button.X/system_dspic33ep128gs804.c  .generated_files/flags/default/90546a96a9b2f26ec7a84816f9cb5767844f4c91 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/system_dspic33ep128gs804.c  -o ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/_ext/1889579238/blinky1.o: D:/projs/blinky_button.X/blinky1.c  .generated_files/flags/default/336461c16e7c68e40c9298abb6f36ce56484174e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/blinky1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/blinky1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/blinky1.c  -o ${OBJECTDIR}/_ext/1889579238/blinky1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/blinky1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/blinky3.o: D:/projs/blinky_button.X/blinky3.c  .generated_files/flags/default/1ffa31ec632d4c88ae982f0ab7d337f7c51d686e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/blinky3.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/blinky3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/blinky3.c  -o ${OBJECTDIR}/_ext/1889579238/blinky3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/blinky3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/button2b.o: D:/projs/blinky_button.X/button2b.c  .generated_files/flags/default/a8b44e1a0c8380cb221076112835209b5720299b .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/button2b.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/button2b.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/button2b.c  -o ${OBJECTDIR}/_ext/1889579238/button2b.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/button2b.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o: D:/projs/blinky_button.X/bsp_dspic33ep128gs804.c  .generated_files/flags/default/501f98a00cf4694d84ec2b8e6e9c52ebc0b359a4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/bsp_dspic33ep128gs804.c  -o ${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/bsp_dspic33ep128gs804.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/button2a.o: D:/projs/blinky_button.X/button2a.c  .generated_files/flags/default/f06ff63ce3bc01ce927e89e289cbe08ecafc7d4e .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/button2a.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/button2a.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/button2a.c  -o ${OBJECTDIR}/_ext/1889579238/button2a.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/button2a.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/149885617/sst_port.o: D:/projs/Super-Simple-Tasker/sst_c/ports/dspic/sst_port.c  .generated_files/flags/default/a7f3e01372f28e1c1f1cddd4927c124215f280de .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/149885617" 
	@${RM} ${OBJECTDIR}/_ext/149885617/sst_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/149885617/sst_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/Super-Simple-Tasker/sst_c/ports/dspic/sst_port.c  -o ${OBJECTDIR}/_ext/149885617/sst_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/149885617/sst_port.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1731567061/sst.o: D:/projs/Super-Simple-Tasker/sst_c/src/sst.c  .generated_files/flags/default/24cc1a8777813924cb430708f7b958caa4faae5a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1731567061" 
	@${RM} ${OBJECTDIR}/_ext/1731567061/sst.o.d 
	@${RM} ${OBJECTDIR}/_ext/1731567061/sst.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/Super-Simple-Tasker/sst_c/src/sst.c  -o ${OBJECTDIR}/_ext/1731567061/sst.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1731567061/sst.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/main.o: D:/projs/blinky_button.X/main.c  .generated_files/flags/default/e01ad5ac12684546dbcd779520af7aba6b87f2ee .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/main.c  -o ${OBJECTDIR}/_ext/1889579238/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o: D:/projs/blinky_button.X/system_dspic33ep128gs804.c  .generated_files/flags/default/b4aa71c07a73d04d0aa4d9a6ce0157a0fe624b97 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1889579238" 
	@${RM} ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o.d 
	@${RM} ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  D:/projs/blinky_button.X/system_dspic33ep128gs804.c  -o ${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1889579238/system_dspic33ep128gs804.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../Super-Simple-Tasker/include" -I"../Super-Simple-Tasker/sst_c/ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/blinky_button.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/blinky_button.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/blinky_button.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/blinky_button.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/blinky_button.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
