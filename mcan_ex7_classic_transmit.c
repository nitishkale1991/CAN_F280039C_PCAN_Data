//#############################################################################
//
// FILE:   mcan_ex7_classic_transmit.c
//
// TITLE:  MCAN Classic frames transmission using Tx Buffer
//
//! \addtogroup driver_example_c28x_list
//! <h1> MCAN Classic frames transmission using Tx Buffer </h1>
//!
//! This simple example shows external communication between the MCAN module
//! and another CAN node. It shows how to transmit classic CAN frames.
//! The GPIOs of MCAN should be connected to a CAN Transceiver.
//! Bit Rate is 500 kbps. Standard Identifier 0x123 is transmitted with
//! 8 data bytes across 10 Tx buffers every 500 seconds.
//!
//! \b Hardware \b Required \n
//!  - A C2000 board with CAN transceiver
//!
//! \b External \b Connections \n
//!  Both nodes should communicate through CAN transceivers.
//!
//!  - MCAN is on DEVICE_GPIO_PIN_CANRXA (MCANRXA)
//!  - and DEVICE_GPIO_PIN_CANTXA (MCANTXA)
//!
//! \b Watch \b Variables \n
//!  - txMsg
//!
//
//#############################################################################
//
//
// $Copyright:
// Copyright (C) 2025 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//#############################################################################

//
// Include Files
//
#include "driverlib.h"
#include "device.h"
#include "inc/stw_types.h"
#include "inc/stw_dataTypes.h"
#include <string.h>
#include "device/driverlib/inc/hw_mcan.h"
//
// Defines.
//
#define NUM_OF_MSG                      (1U)
#define MCAN_STD_ID_FILTER_NUM          (1U)
#define MCAN_EXT_ID_FILTER_NUM          (0U)
#define MCAN_FIFO_0_NUM                 (1U)
#define MCAN_FIFO_0_ELEM_SIZE           (MCAN_ELEM_SIZE_8BYTES)
#define MCAN_FIFO_1_NUM                 (0U)
#define MCAN_FIFO_1_ELEM_SIZE           (MCAN_ELEM_SIZE_64BYTES)
#define MCAN_RX_BUFF_NUM                (0U)
#define MCAN_RX_BUFF_ELEM_SIZE          (MCAN_ELEM_SIZE_64BYTES)
#define MCAN_TX_BUFF_SIZE               (NUM_OF_MSG)
#define MCAN_TX_FQ_SIZE                 (0U)
#define MCAN_TX_BUFF_ELEM_SIZE          (MCAN_ELEM_SIZE_8BYTES)
#define MCAN_TX_EVENT_SIZE              (0U)
// Set to SYSCTL_MCANCLK_DIV_1 to test full 120 MHz MCAN input clock.
#define MCAN_CLK_DIVIDER                (SYSCTL_MCANCLK_DIV_3)

//
//  Defining Starting Addresses for Message RAM Sections,
//  (Calculated from Macros based on User defined configuration above)
//
#define MCAN_STD_ID_FILT_START_ADDR     (0x0U)
#define MCAN_EXT_ID_FILT_START_ADDR     (MCAN_STD_ID_FILT_START_ADDR + ((MCAN_STD_ID_FILTER_NUM * MCANSS_STD_ID_FILTER_SIZE_WORDS * 4U)))
#define MCAN_FIFO_0_START_ADDR          (MCAN_EXT_ID_FILT_START_ADDR + ((MCAN_EXT_ID_FILTER_NUM * MCANSS_EXT_ID_FILTER_SIZE_WORDS * 4U)))
#define MCAN_FIFO_1_START_ADDR          (MCAN_FIFO_0_START_ADDR + (MCAN_getMsgObjSize(MCAN_FIFO_0_ELEM_SIZE) * 4U * MCAN_FIFO_0_NUM))
#define MCAN_RX_BUFF_START_ADDR         (MCAN_FIFO_1_START_ADDR + (MCAN_getMsgObjSize(MCAN_FIFO_1_ELEM_SIZE) * 4U * MCAN_FIFO_1_NUM))
#define MCAN_TX_BUFF_START_ADDR         (MCAN_RX_BUFF_START_ADDR + (MCAN_getMsgObjSize(MCAN_RX_BUFF_ELEM_SIZE) * 4U * MCAN_RX_BUFF_NUM))
#define MCAN_TX_EVENT_START_ADDR        (MCAN_TX_BUFF_START_ADDR + (MCAN_getMsgObjSize(MCAN_TX_BUFF_ELEM_SIZE) * 4U * (MCAN_TX_BUFF_SIZE + MCAN_TX_FQ_SIZE)))

#define PCAN_CMD_MSG_ID                 (0x201U)
#define MCAN_RESP_MSG_ID                (0x123U)
#define MCAN_STD_ID_SHIFT               (18U)
#define PERIODIC_LOOP_US                (100000U)
#define PERIODIC_COUNTER_STEP           (1U)

//
// Global Variables.
//
int32_t     error = 0;
MCAN_TxBufElement txMsg[NUM_OF_MSG];
/*volatile uint32_t nbtp_reg;
volatile uint32_t prescaler_reg;
volatile uint32_t time_seg1_reg;
volatile uint32_t time_seg2_reg;
volatile uint32_t sjw_reg;
volatile uint32_t sysclk_hz;
volatile uint32_t mcanclk_hz;
volatile uint32_t psr_reg;
volatile uint32_t lec_reg;
volatile uint32_t act_reg;
volatile uint32_t ecr_reg;
volatile uint32_t tec_reg;
volatile uint32_t rec_reg;
volatile uint32_t ir_reg;
volatile uint32_t cccr_reg;
volatile uint32_t txbar_reg;
volatile uint32_t txbrp_reg;
volatile uint32_t txbto_reg;
volatile uint32_t txfqs_reg;
volatile uint32_t psr_lec;
volatile uint32_t psr_act;
volatile uint32_t psr_ew;
volatile uint32_t ecr_tec;
volatile uint32_t ecr_rec;
volatile uint32_t cccr_init;
volatile uint32_t cccr_cce;
volatile uint32_t cccr_ase;
volatile uint32_t txfqs_tffl;
volatile uint32_t txbto_value;*/
//
// Function Prototype.
//
static void MCANConfig(void);

void main()
{
    uint32_t periodicCounter = 0U;
    uint32_t rxStdId = 0U;
    int i = 0;
    uint32_t dataBytes = 8U;
    MCAN_RxFIFOStatus rxFIFOStatus;
    MCAN_RxBufElement rxMsg;
    rxFIFOStatus.num = MCAN_RX_FIFO_NUM_0;

    //
    // Initialize device clock and peripherals
    //
    Device_init();

    //
    // Initialize GPIO and configure GPIO pins for CANTX/CANRX
    //
    Device_initGPIO();

    //
    // Configure the divisor for the MCAN bit-clock
    //
    MCAN_selectClockSource(MCANA_DRIVER_BASE, MCAN_CLOCK_SOURCE_SYS);
    SysCtl_setMCANClk(MCAN_CLK_DIVIDER);
    //sysclk_hz = SysCtl_getClock(DEVICE_OSCSRC_FREQ);
    //mcanclk_hz = sysclk_hz / ((uint32_t)MCAN_CLK_DIVIDER + 1U);

    //
    // Configuring the GPIOs for MCAN.
    //
    GPIO_setPinConfig(DEVICE_GPIO_CFG_MCANRXA);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_MCANTXA);

    //
    // Initialize message to transmit.
    //
    txMsg[0].id       = ((uint32_t)MCAN_RESP_MSG_ID << MCAN_STD_ID_SHIFT);
    txMsg[0].rtr      = 0U;
    txMsg[0].xtd      = 0U;
    txMsg[0].esi      = 0U;
    txMsg[0].dlc      = 8U;
    txMsg[0].brs      = 0U;
    txMsg[0].fdf      = 0U;
    txMsg[0].efc      = 0U;
    txMsg[0].mm       = 0xA0U;
    for(i = 0; i < (int)dataBytes; i++)
    {
        txMsg[0].data[i] = 0x00U;
    }

    //
    // Configure the MCAN Module.
    //
    MCANConfig();

    //
    // Write message to Message RAM.
    //
    // DEBUG: Read register immediately after setting,
    /*    asm(" NOP");
    nbtp_reg = HWREG(MCANA_DRIVER_BASE + MCAN_NBTP);
    asm(" NOP");
    prescaler_reg = (nbtp_reg & MCAN_NBTP_NBRP_M) >> MCAN_NBTP_NBRP_S;
    asm(" NOP");
    time_seg1_reg = (nbtp_reg & MCAN_NBTP_NTSEG1_M) >> MCAN_NBTP_NTSEG1_S;
    asm(" NOP");
    time_seg2_reg = (nbtp_reg & MCAN_NBTP_NTSEG2_M) >> MCAN_NBTP_NTSEG2_S;
    asm(" NOP");
    sjw_reg = (nbtp_reg & MCAN_NBTP_NSJW_M) >> MCAN_NBTP_NSJW_S;
    asm(" NOP");
    psr_reg = HWREG(MCANA_DRIVER_BASE + MCAN_PSR);
    asm(" NOP");
    lec_reg = (psr_reg & MCAN_PSR_LEC_M) >> MCAN_PSR_LEC_S;
    asm(" NOP");
    act_reg = (psr_reg & MCAN_PSR_ACT_M) >> MCAN_PSR_ACT_S;
    asm(" NOP");
    ecr_reg = HWREG(MCANA_DRIVER_BASE + MCAN_ECR);
    asm(" NOP");
    tec_reg = (ecr_reg & MCAN_ECR_TEC_M) >> MCAN_ECR_TEC_S;
    asm(" NOP");
    rec_reg = (ecr_reg & MCAN_ECR_REC_M) >> MCAN_ECR_REC_S;
    asm(" NOP");
    ir_reg  = HWREG(MCANA_DRIVER_BASE + MCAN_IR);
    asm(" NOP");
    cccr_reg = HWREG(MCANA_DRIVER_BASE + MCAN_CCCR);
    asm(" NOP");
    txbar_reg = HWREG(MCANA_DRIVER_BASE + MCAN_TXBAR);
    asm(" NOP");
    txbrp_reg = HWREG(MCANA_DRIVER_BASE + MCAN_TXBRP);
    asm(" NOP");
    txbto_reg = HWREG(MCANA_DRIVER_BASE + MCAN_TXBTO);
    asm(" NOP");
    txfqs_reg = HWREG(MCANA_DRIVER_BASE + MCAN_TXFQS);
    asm(" NOP");    */
    MCAN_writeMsgRam(MCANA_DRIVER_BASE, MCAN_MEM_TYPE_BUF, 0U, &txMsg[0]);

    while(1)
    {
        periodicCounter += PERIODIC_COUNTER_STEP;

        MCAN_getRxFIFOStatus(MCANA_DRIVER_BASE, &rxFIFOStatus);
        while(rxFIFOStatus.fillLvl > 0U)
        {
            memset(&rxMsg, 0, sizeof(rxMsg));
            MCAN_readMsgRam(MCANA_DRIVER_BASE, MCAN_MEM_TYPE_FIFO, 0U,
                            MCAN_RX_FIFO_NUM_0, &rxMsg);

            MCAN_writeRxFIFOAck(MCANA_DRIVER_BASE, MCAN_RX_FIFO_NUM_0,
                                rxFIFOStatus.getIdx);

            rxStdId = (rxMsg.id >> MCAN_STD_ID_SHIFT) & 0x7FFU;
            if((rxMsg.xtd == 0U) && (rxMsg.rtr == 0U) &&
               (rxStdId == PCAN_CMD_MSG_ID))
            {
                txMsg[0].data[0] = (uint16_t)(periodicCounter & 0xFFU);
                txMsg[0].data[1] = (uint16_t)((periodicCounter >> 8U) & 0xFFU);
                txMsg[0].data[2] = (uint16_t)((periodicCounter >> 16U) & 0xFFU);
                txMsg[0].data[3] = (uint16_t)((periodicCounter >> 24U) & 0xFFU);
                txMsg[0].data[4] = 0x00U;
                txMsg[0].data[5] = 0x00U;
                txMsg[0].data[6] = 0x00U;
                txMsg[0].data[7] = 0x00U;

                MCAN_writeMsgRam(MCANA_DRIVER_BASE, MCAN_MEM_TYPE_BUF, 0U,
                                 &txMsg[0]);
                MCAN_txBufAddReq(MCANA_DRIVER_BASE, 0U);
                while(MCAN_getTxBufReqPend(MCANA_DRIVER_BASE))
                {
                }
            }

            MCAN_getRxFIFOStatus(MCANA_DRIVER_BASE, &rxFIFOStatus);
        }

        DEVICE_DELAY_US(PERIODIC_LOOP_US);
    }

    //
    // Stop Application.
    //
    asm("   ESTOP0");
}

static void MCANConfig(void)
{
    MCAN_InitParams initParams;
    MCAN_MsgRAMConfigParams    msgRAMConfigParams;
    MCAN_BitTimingParams       bitTimes;
    MCAN_StdMsgIDFilterElement stdFilter;

    //
    //  Initializing all structs to zero to prevent stray values
    //
    memset(&initParams, 0, sizeof(initParams));
    memset(&msgRAMConfigParams, 0, sizeof(msgRAMConfigParams));
    memset(&bitTimes, 0, sizeof(bitTimes));
    memset(&stdFilter, 0, sizeof(stdFilter));

    //
    // Initialize MCAN Init parameters.
    //
    initParams.fdMode            = 0x0U; // FD operation disabled.
    initParams.brsEnable         = 0x0U; // Bit rate switching for
                                         // transmissions disabled.
    initParams.emulationEnable   = 0x1U; // Keep MCAN active when CPU is halted.

    //
    // Initialize Message RAM Sections Configuration Parameters
    //
    msgRAMConfigParams.flssa                = MCAN_STD_ID_FILT_START_ADDR;
    msgRAMConfigParams.lss                  = MCAN_STD_ID_FILTER_NUM;
    msgRAMConfigParams.flesa                = MCAN_EXT_ID_FILT_START_ADDR;
    msgRAMConfigParams.lse                  = MCAN_EXT_ID_FILTER_NUM;
    msgRAMConfigParams.txStartAddr          = MCAN_TX_BUFF_START_ADDR;
    // Tx Buffers Start Address.
    msgRAMConfigParams.txBufNum             = MCAN_TX_BUFF_SIZE;
    // Number of Dedicated Transmit Buffers.
    msgRAMConfigParams.txFIFOSize           = MCAN_TX_FQ_SIZE;
    msgRAMConfigParams.txBufMode            = 0U;
    msgRAMConfigParams.txBufElemSize        = MCAN_TX_BUFF_ELEM_SIZE;
    // Tx Buffer Element Size.
    msgRAMConfigParams.txEventFIFOStartAddr = MCAN_TX_EVENT_START_ADDR;
    msgRAMConfigParams.txEventFIFOSize      = MCAN_TX_EVENT_SIZE;
    msgRAMConfigParams.txEventFIFOWaterMark = 0U;
    msgRAMConfigParams.rxFIFO0startAddr     = MCAN_FIFO_0_START_ADDR;
    msgRAMConfigParams.rxFIFO0size          = MCAN_FIFO_0_NUM;
    msgRAMConfigParams.rxFIFO0waterMark     = 1U;
    msgRAMConfigParams.rxFIFO0OpMode        = 0U;
    msgRAMConfigParams.rxFIFO1startAddr     = MCAN_FIFO_1_START_ADDR;
    msgRAMConfigParams.rxFIFO1size          = MCAN_FIFO_1_NUM;
    msgRAMConfigParams.rxFIFO1waterMark     = 0U;
    msgRAMConfigParams.rxFIFO1OpMode        = 0U;
    msgRAMConfigParams.rxBufStartAddr       = MCAN_RX_BUFF_START_ADDR;
    msgRAMConfigParams.rxBufElemSize        = MCAN_RX_BUFF_ELEM_SIZE;
    msgRAMConfigParams.rxFIFO0ElemSize      = MCAN_FIFO_0_ELEM_SIZE;
    msgRAMConfigParams.rxFIFO1ElemSize      = MCAN_FIFO_1_ELEM_SIZE;

    //
    // Initialize bit timings for 500 kbps.
    //
    // MCAN NBTP fields are register-coded values (actual timing uses +1).
    // For 500 kbps @ 40 MHz MCAN clock with 70% sample point:
    // NBRP=3 -> /4, NTSEG1=12 -> 13 tq, NTSEG2=5 -> 6 tq, NSJW=1 -> 2 tq.
    bitTimes.nomRatePrescalar   = 3U;
    bitTimes.nomTimeSeg1        = 12U;
    bitTimes.nomTimeSeg2        = 5U;
    bitTimes.nomSynchJumpWidth  = 1U;
    //
    // Wait for memory initialization to happen.
    //
    while(FALSE == MCAN_isMemInitDone(MCANA_DRIVER_BASE))
    {
    }

    //
    // Put MCAN in SW initialization mode.
    //
    MCAN_setOpMode(MCANA_DRIVER_BASE, MCAN_OPERATION_MODE_SW_INIT);

    //
    // Wait till MCAN is not initialized.
    //
    while (MCAN_OPERATION_MODE_SW_INIT != MCAN_getOpMode(MCANA_DRIVER_BASE))
    {}

    //
    // Initialize MCAN module.
    //
    MCAN_init(MCANA_DRIVER_BASE, &initParams);

    //
    // Configure Bit timings.
    //
    MCAN_setBitTime(MCANA_DRIVER_BASE, &bitTimes);

    //
    // Configure Message RAM Sections
    //
    MCAN_msgRAMConfig(MCANA_DRIVER_BASE, &msgRAMConfigParams);

    stdFilter.sfid1 = PCAN_CMD_MSG_ID;
    stdFilter.sfid2 = PCAN_CMD_MSG_ID;
    stdFilter.sfec  = MCAN_STDFILTEC_FIFO0;
    stdFilter.sft   = MCAN_STDFILT_DUAL;
    MCAN_addStdMsgIDFilter(MCANA_DRIVER_BASE, 0U, &stdFilter);

    //
    // Take MCAN out of the SW initialization mode
    //
    MCAN_setOpMode(MCANA_DRIVER_BASE, MCAN_OPERATION_MODE_NORMAL);

    while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(MCANA_DRIVER_BASE))
    {

    }
}
