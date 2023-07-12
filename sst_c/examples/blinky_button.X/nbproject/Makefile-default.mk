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
SOURCEFILES_QUOTED_IF_SPACED=blinky1.c blinky3.c bsp_dspic33ep128gs804.c button2a.c button2b.c ../../ports/dspic/sst_port.c ../../src/sst.c main.c system_dspic33ep128gs804.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/blinky1.o ${OBJECTDIR}/blinky3.o ${OBJECTDIR}/bsp_dspic33ep128gs804.o ${OBJECTDIR}/button2a.o ${OBJECTDIR}/button2b.o ${OBJECTDIR}/_ext/1564044098/sst_port.o ${OBJECTDIR}/_ext/1445274692/sst.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system_dspic33ep128gs804.o
POSSIBLE_DEPFILES=${OBJECTDIR}/blinky1.o.d ${OBJECTDIR}/blinky3.o.d ${OBJECTDIR}/bsp_dspic33ep128gs804.o.d ${OBJECTDIR}/button2a.o.d ${OBJECTDIR}/button2b.o.d ${OBJECTDIR}/_ext/1564044098/sst_port.o.d ${OBJECTDIR}/_ext/1445274692/sst.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/system_dspic33ep128gs804.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/blinky1.o ${OBJECTDIR}/blinky3.o ${OBJECTDIR}/bsp_dspic33ep128gs804.o ${OBJECTDIR}/button2a.o ${OBJECTDIR}/button2b.o ${OBJECTDIR}/_ext/1564044098/sst_port.o ${OBJECTDIR}/_ext/1445274692/sst.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system_dspic33ep128gs804.o

# Source Files
SOURCEFILES=blinky1.c blinky3.c bsp_dspic33ep128gs804.c button2a.c button2b.c ../../ports/dspic/sst_port.c ../../src/sst.c main.c system_dspic33ep128gs804.c



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
${OBJECTDIR}/blinky1.o: blinky1.c  .generated_files/flags/default/6bb4bf2b8a6380f8adb4f28506fcc9cd76d563d1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/blinky1.o.d 
	@${RM} ${OBJECTDIR}/blinky1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  blinky1.c  -o ${OBJECTDIR}/blinky1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/blinky1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/blinky3.o: blinky3.c  .generated_files/flags/default/5642c580c2e0c50f94b33ca3b5d52380cf80abd4 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/blinky3.o.d 
	@${RM} ${OBJECTDIR}/blinky3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  blinky3.c  -o ${OBJECTDIR}/blinky3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/blinky3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/bsp_dspic33ep128gs804.o: bsp_dspic33ep128gs804.c  .generated_files/flags/default/cb7a8c77d3c88a3e0ade636f47747ffcfd8fe42a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bsp_dspic33ep128gs804.o.d 
	@${RM} ${OBJECTDIR}/bsp_dspic33ep128gs804.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  bsp_dspic33ep128gs804.c  -o ${OBJECTDIR}/bsp_dspic33ep128gs804.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/bsp_dspic33ep128gs804.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/button2a.o: button2a.c  .generated_files/flags/default/a21c7a50d7926cdf9d7dbc699a13973650100173 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/button2a.o.d 
	@${RM} ${OBJECTDIR}/button2a.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  button2a.c  -o ${OBJECTDIR}/button2a.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/button2a.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/button2b.o: button2b.c  .generated_files/flags/default/be0d901207193bd6e3d4590457cbe857f0abbf47 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/button2b.o.d 
	@${RM} ${OBJECTDIR}/button2b.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  button2b.c  -o ${OBJECTDIR}/button2b.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/button2b.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1564044098/sst_port.o: ../../ports/dspic/sst_port.c  .generated_files/flags/default/41186a2b0ddf2307578da8b4768964d66e72c6f1 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1564044098" 
	@${RM} ${OBJECTDIR}/_ext/1564044098/sst_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1564044098/sst_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../ports/dspic/sst_port.c  -o ${OBJECTDIR}/_ext/1564044098/sst_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1564044098/sst_port.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1445274692/sst.o: ../../src/sst.c  .generated_files/flags/default/1e5f61ca9ad8bb89c72ebac63d93d7a0c608001 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1445274692" 
	@${RM} ${OBJECTDIR}/_ext/1445274692/sst.o.d 
	@${RM} ${OBJECTDIR}/_ext/1445274692/sst.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../src/sst.c  -o ${OBJECTDIR}/_ext/1445274692/sst.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1445274692/sst.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/2a153e677d9b5fcb1e2c97e1e51a0d42205924ae .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/system_dspic33ep128gs804.o: system_dspic33ep128gs804.c  .generated_files/flags/default/fb2bbbdaa369f5c4931c2a9541cd103430559dad .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_dspic33ep128gs804.o.d 
	@${RM} ${OBJECTDIR}/system_dspic33ep128gs804.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_dspic33ep128gs804.c  -o ${OBJECTDIR}/system_dspic33ep128gs804.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/system_dspic33ep128gs804.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/blinky1.o: blinky1.c  .generated_files/flags/default/b96645270597bed95aeb0f5d70774560bdb91ca3 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/blinky1.o.d 
	@${RM} ${OBJECTDIR}/blinky1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  blinky1.c  -o ${OBJECTDIR}/blinky1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/blinky1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/blinky3.o: blinky3.c  .generated_files/flags/default/c02d779d6388246b04712c2e295125ad457351da .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/blinky3.o.d 
	@${RM} ${OBJECTDIR}/blinky3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  blinky3.c  -o ${OBJECTDIR}/blinky3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/blinky3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/bsp_dspic33ep128gs804.o: bsp_dspic33ep128gs804.c  .generated_files/flags/default/c7f2d7cddde5280a2003818dd7857332e855cc59 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/bsp_dspic33ep128gs804.o.d 
	@${RM} ${OBJECTDIR}/bsp_dspic33ep128gs804.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  bsp_dspic33ep128gs804.c  -o ${OBJECTDIR}/bsp_dspic33ep128gs804.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/bsp_dspic33ep128gs804.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/button2a.o: button2a.c  .generated_files/flags/default/d2475d90ce2b825551566483400d455a43506fec .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/button2a.o.d 
	@${RM} ${OBJECTDIR}/button2a.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  button2a.c  -o ${OBJECTDIR}/button2a.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/button2a.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/button2b.o: button2b.c  .generated_files/flags/default/4f69ec3bc1b81e5e528e1495cf4474390c5cdc9a .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/button2b.o.d 
	@${RM} ${OBJECTDIR}/button2b.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  button2b.c  -o ${OBJECTDIR}/button2b.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/button2b.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1564044098/sst_port.o: ../../ports/dspic/sst_port.c  .generated_files/flags/default/cc1928f264746b56e124c1a37e14068f639854f8 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1564044098" 
	@${RM} ${OBJECTDIR}/_ext/1564044098/sst_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1564044098/sst_port.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../ports/dspic/sst_port.c  -o ${OBJECTDIR}/_ext/1564044098/sst_port.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1564044098/sst_port.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/_ext/1445274692/sst.o: ../../src/sst.c  .generated_files/flags/default/1030b61f629f02c27f79826e930d3a31165856fd .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/_ext/1445274692" 
	@${RM} ${OBJECTDIR}/_ext/1445274692/sst.o.d 
	@${RM} ${OBJECTDIR}/_ext/1445274692/sst.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../src/sst.c  -o ${OBJECTDIR}/_ext/1445274692/sst.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/_ext/1445274692/sst.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/8773facdc99c655f9d154841af6e196abdcd5c9c .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/system_dspic33ep128gs804.o: system_dspic33ep128gs804.c  .generated_files/flags/default/e515ec8d50ecdcbaac511ac59d2c09905382d8b9 .generated_files/flags/default/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_dspic33ep128gs804.o.d 
	@${RM} ${OBJECTDIR}/system_dspic33ep128gs804.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  system_dspic33ep128gs804.c  -o ${OBJECTDIR}/system_dspic33ep128gs804.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/system_dspic33ep128gs804.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_default=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -I"../../../include" -I"../../ports/dspic" -msmart-io=1 -Wall -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
