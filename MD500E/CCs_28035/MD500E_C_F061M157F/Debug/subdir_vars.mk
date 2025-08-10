################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/common/DSP28035Peripheral.cmd \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/common/F28035.cmd 

ASM_SRCS += \
D:/Product_Code/013_MD500E_C_2019/MD500E/04_subprogram/ASMSubPrg.asm \
D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/DSP2803x_CodeStartBranch.asm \
D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/DSP280x_CSMPasswords.asm 

OBJ_SRCS += \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500_func_obj_28034/DSP2803x_usDelay.obj 

C_SRCS += \
D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/DSP2803x_CpuTimers.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/DSP2803x_GlobalVariableDefs.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorCarrier.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorConstant.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorCurrentTransform.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorDataExchange.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorDebug.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/encoder/MotorEncoder.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorFlyingStart.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/vc/PM_svc/MotorIPMSVC.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorInfoCollect.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorInvProtect.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorMain.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/pwm/MotorPWM.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorParChange.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/para_estimation/MotorParEst.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/vc/MotorPmsmMain.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/para_estimation/MotorPmsmParEst.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorPublicCal.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/encoder/MotorSpeed.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/vc/MotorVCMain.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/vf/MotorVF.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorVar.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/PMFluxWeakCsr2.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/04_subprogram/SubPrg.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/pwm/Svpwm1.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_CANlinkEx.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_canlink.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_comm.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_common.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_dspcan.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_eeprom.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_error.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_fcDeal.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_frqSrc.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_frqSrc_pid.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_funcCode.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_interface.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_invPara.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_io.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_main.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_menu.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_modbus.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_motorFc.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_osc.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_p2p.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_plc.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_runSrc.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_runSrc_accDecFrq.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_torqueCtrl.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_ui.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_vf.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/main.c \
D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/mainInit_2803x.c 

C_DEPS += \
./DSP2803x_CpuTimers.d \
./DSP2803x_GlobalVariableDefs.d \
./MotorCarrier.d \
./MotorConstant.d \
./MotorCurrentTransform.d \
./MotorDataExchange.d \
./MotorDebug.d \
./MotorEncoder.d \
./MotorFlyingStart.d \
./MotorIPMSVC.d \
./MotorInfoCollect.d \
./MotorInvProtect.d \
./MotorMain.d \
./MotorPWM.d \
./MotorParChange.d \
./MotorParEst.d \
./MotorPmsmMain.d \
./MotorPmsmParEst.d \
./MotorPublicCal.d \
./MotorSpeed.d \
./MotorVCMain.d \
./MotorVF.d \
./MotorVar.d \
./PMFluxWeakCsr2.d \
./SubPrg.d \
./Svpwm1.d \
./f_CANlinkEx.d \
./f_canlink.d \
./f_comm.d \
./f_common.d \
./f_dspcan.d \
./f_eeprom.d \
./f_error.d \
./f_fcDeal.d \
./f_frqSrc.d \
./f_frqSrc_pid.d \
./f_funcCode.d \
./f_interface.d \
./f_invPara.d \
./f_io.d \
./f_main.d \
./f_menu.d \
./f_modbus.d \
./f_motorFc.d \
./f_osc.d \
./f_p2p.d \
./f_plc.d \
./f_runSrc.d \
./f_runSrc_accDecFrq.d \
./f_torqueCtrl.d \
./f_ui.d \
./f_vf.d \
./main.d \
./mainInit_2803x.d 

OBJS += \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/ASMSubPrg.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/DSP2803x_CodeStartBranch.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/DSP2803x_CpuTimers.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/DSP2803x_GlobalVariableDefs.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/DSP280x_CSMPasswords.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorCarrier.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorConstant.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorCurrentTransform.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorDataExchange.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorDebug.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorEncoder.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorFlyingStart.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorIPMSVC.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorInfoCollect.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorInvProtect.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorMain.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorPWM.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorParChange.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorParEst.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorPmsmMain.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorPmsmParEst.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorPublicCal.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorSpeed.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorVCMain.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorVF.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/MotorVar.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/PMFluxWeakCsr2.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/SubPrg.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/Svpwm1.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_CANlinkEx.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_canlink.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_comm.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_common.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_dspcan.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_eeprom.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_error.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_fcDeal.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_frqSrc.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_frqSrc_pid.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_funcCode.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_interface.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_invPara.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_io.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_main.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_menu.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_modbus.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_motorFc.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_osc.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_p2p.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_plc.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_runSrc.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_runSrc_accDecFrq.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_torqueCtrl.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_ui.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/f_vf.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/main.obj \
D:/Product_Code/013_MD500E_C_2019/MD500E/05_public/obj/MD500E_28035_motor_obj/mainInit_2803x.obj 

ASM_DEPS += \
./ASMSubPrg.d \
./DSP2803x_CodeStartBranch.d \
./DSP280x_CSMPasswords.d 

OBJS__QUOTED += \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\ASMSubPrg.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\DSP2803x_CodeStartBranch.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\DSP2803x_CpuTimers.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\DSP2803x_GlobalVariableDefs.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\DSP280x_CSMPasswords.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorCarrier.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorConstant.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorCurrentTransform.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorDataExchange.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorDebug.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorEncoder.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorFlyingStart.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorIPMSVC.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorInfoCollect.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorInvProtect.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorMain.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorPWM.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorParChange.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorParEst.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorPmsmMain.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorPmsmParEst.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorPublicCal.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorSpeed.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorVCMain.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorVF.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\MotorVar.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\PMFluxWeakCsr2.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\SubPrg.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\Svpwm1.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_CANlinkEx.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_canlink.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_comm.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_common.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_dspcan.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_eeprom.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_error.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_fcDeal.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_frqSrc.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_frqSrc_pid.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_funcCode.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_interface.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_invPara.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_io.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_main.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_menu.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_modbus.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_motorFc.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_osc.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_p2p.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_plc.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_runSrc.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_runSrc_accDecFrq.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_torqueCtrl.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_ui.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\f_vf.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\main.obj" \
"D:\Product_Code\013_MD500E_C_2019\MD500E\05_public\obj\MD500E_28035_motor_obj\mainInit_2803x.obj" 

C_DEPS__QUOTED += \
"DSP2803x_CpuTimers.d" \
"DSP2803x_GlobalVariableDefs.d" \
"MotorCarrier.d" \
"MotorConstant.d" \
"MotorCurrentTransform.d" \
"MotorDataExchange.d" \
"MotorDebug.d" \
"MotorEncoder.d" \
"MotorFlyingStart.d" \
"MotorIPMSVC.d" \
"MotorInfoCollect.d" \
"MotorInvProtect.d" \
"MotorMain.d" \
"MotorPWM.d" \
"MotorParChange.d" \
"MotorParEst.d" \
"MotorPmsmMain.d" \
"MotorPmsmParEst.d" \
"MotorPublicCal.d" \
"MotorSpeed.d" \
"MotorVCMain.d" \
"MotorVF.d" \
"MotorVar.d" \
"PMFluxWeakCsr2.d" \
"SubPrg.d" \
"Svpwm1.d" \
"f_CANlinkEx.d" \
"f_canlink.d" \
"f_comm.d" \
"f_common.d" \
"f_dspcan.d" \
"f_eeprom.d" \
"f_error.d" \
"f_fcDeal.d" \
"f_frqSrc.d" \
"f_frqSrc_pid.d" \
"f_funcCode.d" \
"f_interface.d" \
"f_invPara.d" \
"f_io.d" \
"f_main.d" \
"f_menu.d" \
"f_modbus.d" \
"f_motorFc.d" \
"f_osc.d" \
"f_p2p.d" \
"f_plc.d" \
"f_runSrc.d" \
"f_runSrc_accDecFrq.d" \
"f_torqueCtrl.d" \
"f_ui.d" \
"f_vf.d" \
"main.d" \
"mainInit_2803x.d" 

ASM_DEPS__QUOTED += \
"ASMSubPrg.d" \
"DSP2803x_CodeStartBranch.d" \
"DSP280x_CSMPasswords.d" 

ASM_SRCS__QUOTED += \
"D:/Product_Code/013_MD500E_C_2019/MD500E/04_subprogram/ASMSubPrg.asm" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/DSP2803x_CodeStartBranch.asm" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/DSP280x_CSMPasswords.asm" 

C_SRCS__QUOTED += \
"D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/DSP2803x_CpuTimers.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/DSP2803x_GlobalVariableDefs.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorCarrier.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorConstant.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorCurrentTransform.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorDataExchange.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorDebug.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/encoder/MotorEncoder.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorFlyingStart.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/vc/PM_svc/MotorIPMSVC.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorInfoCollect.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorInvProtect.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorMain.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/pwm/MotorPWM.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorParChange.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/para_estimation/MotorParEst.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/vc/MotorPmsmMain.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/para_estimation/MotorPmsmParEst.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorPublicCal.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/encoder/MotorSpeed.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/vc/MotorVCMain.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/vf/MotorVF.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/main_public/MotorVar.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/PMFluxWeakCsr2.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/04_subprogram/SubPrg.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/02_motor/pwm/Svpwm1.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_CANlinkEx.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_canlink.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_comm.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_common.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_dspcan.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_eeprom.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_error.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_fcDeal.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_frqSrc.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_frqSrc_pid.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_funcCode.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_interface.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_invPara.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_io.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_main.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_menu.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_modbus.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_motorFc.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_osc.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_p2p.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_plc.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_runSrc.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_runSrc_accDecFrq.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_torqueCtrl.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_ui.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/func/src/f_vf.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/main.c" \
"D:/Product_Code/013_MD500E_C_2019/MD500E/01_system/mainInit_2803x.c" 


