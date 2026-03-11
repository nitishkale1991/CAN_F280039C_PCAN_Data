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
#define NUM_OF_MSG                      (10U)
#define MCAN_STD_ID_FILTER_NUM          (0U)
#define MCAN_EXT_ID_FILTER_NUM          (0U)
#define MCAN_FIFO_0_NUM                 (0U)
#define MCAN_FIFO_0_ELEM_SIZE           (MCAN_ELEM_SIZE_64BYTES)
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


//
// Global Variables.
//
int32_t     error = 0;
MCAN_TxBufElement txMsg[NUM_OF_MSG];
volatile uint32_t nbtp_reg;
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
volatile uint32_t txbto_value;
//
// Function Prototype.
//
static void MCANConfig(void);

void main()
{
    uint32_t msgIdx = 0U;
    int i = 0;
    uint32_t dataBytes = 8U;

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
    sysclk_hz = SysCtl_getClock(DEVICE_OSCSRC_FREQ);
    mcanclk_hz = sysclk_hz / ((uint32_t)MCAN_CLK_DIVIDER + 1U);

    //
    // Configuring the GPIOs for MCAN.
    //
    GPIO_setPinConfig(DEVICE_GPIO_CFG_MCANRXA);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_MCANTXA);

    //
    // Initialize message to transmit.
    //
    for(msgIdx = 0U; msgIdx < NUM_OF_MSG; msgIdx++)
    {
        txMsg[msgIdx].id       = ((uint32_t)0x123U << 18U);
        txMsg[msgIdx].rtr      = 0U;
        txMsg[msgIdx].xtd      = 0U;
        txMsg[msgIdx].esi      = 0U;
        txMsg[msgIdx].dlc      = 8U;
        txMsg[msgIdx].brs      = 0U;
        txMsg[msgIdx].fdf      = 0U;
        txMsg[msgIdx].efc      = 0U;
        txMsg[msgIdx].mm       = (uint32_t)(0xA0U + msgIdx);
        txMsg[msgIdx].data[0]  = (uint16_t)(msgIdx & 0xFFU);
        for(i = 0; i < (int)dataBytes; i++)
        {
            txMsg[msgIdx].data[i] = 0xA1;
        }
    }

    //
    // Configure the MCAN Module.
    //
    MCANConfig();

    //
    // Write message to Message RAM.
    //
    // DEBUG: Read register immediately after setting
    asm(" NOP");
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
    asm(" NOP");
    for(msgIdx = 0U; msgIdx < NUM_OF_MSG; msgIdx++)
    {
        MCAN_writeMsgRam(MCANA_DRIVER_BASE, MCAN_MEM_TYPE_BUF, msgIdx,
                         &txMsg[msgIdx]);
    }

    while(1)
    {
        //
        // Transmit all 10 frames.
        //
        //nbtp_reg = HWREG(MCANA_DRIVER_BASE + MCAN_O_NBTP);
        //prescaler_reg = (nbtp_reg & MCAN_NBTP_NBRP_M) >> MCAN_NBTP_NBRP_S;
        //time_seg1_reg = (nbtp_reg & MCAN_NBTP_NTSEG1_M) >> MCAN_NBTP_NTSEG1_S;
        //time_seg2_reg = (nbtp_reg & MCAN_NBTP_NTSEG2_M) >> MCAN_NBTP_NTSEG2_S;
        //sjw_reg = (nbtp_reg & MCAN_NBTP_NSJW_M) >> MCAN_NBTP_NSJW_S;  
        for(msgIdx = 0U; msgIdx < NUM_OF_MSG; msgIdx++)
        {
            MCAN_txBufAddReq(MCANA_DRIVER_BASE, msgIdx);

            //
            // Wait till the message is transmitted.
            //

        while(MCAN_getTxBufReqPend(MCANA_DRIVER_BASE))
        {
            psr_reg = HWREG(MCANA_DRIVER_BASE + MCAN_PSR);
            psr_lec = (psr_reg & MCAN_PSR_LEC_M) >> MCAN_PSR_LEC_S;
            psr_act = (psr_reg & MCAN_PSR_ACT_M) >> MCAN_PSR_ACT_S;
            psr_ew = (psr_reg & MCAN_PSR_EW) >> 6U;
            lec_reg = psr_lec;
            act_reg = psr_act;
            ecr_reg = HWREG(MCANA_DRIVER_BASE + MCAN_ECR);
            ecr_tec = (ecr_reg & MCAN_ECR_TEC_M) >> MCAN_ECR_TEC_S;
            ecr_rec = (ecr_reg & MCAN_ECR_REC_M) >> MCAN_ECR_REC_S;
            tec_reg = ecr_tec;
            rec_reg = ecr_rec;
            ir_reg  = HWREG(MCANA_DRIVER_BASE + MCAN_IR);
            cccr_reg = HWREG(MCANA_DRIVER_BASE + MCAN_CCCR);
            txbar_reg = HWREG(MCANA_DRIVER_BASE + MCAN_TXBAR);
            txbrp_reg = HWREG(MCANA_DRIVER_BASE + MCAN_TXBRP);
            txbto_reg = HWREG(MCANA_DRIVER_BASE + MCAN_TXBTO);
            txfqs_reg = HWREG(MCANA_DRIVER_BASE + MCAN_TXFQS);
        }
        }

        //
        // Wait 500 seconds before transmitting next batch.
        //
        DEVICE_DELAY_US(500000);
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

    //
    //  Initializing all structs to zero to prevent stray values
    //
    memset(&initParams, 0, sizeof(initParams));
    memset(&msgRAMConfigParams, 0, sizeof(msgRAMConfigParams));
    memset(&bitTimes, 0, sizeof(bitTimes));

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
    msgRAMConfigParams.txStartAddr          = MCAN_TX_BUFF_START_ADDR;
    // Tx Buffers Start Address.
    msgRAMConfigParams.txBufNum             = MCAN_TX_BUFF_SIZE;
    // Number of Dedicated Transmit Buffers.
    msgRAMConfigParams.txBufMode            = 0U;
    msgRAMConfigParams.txBufElemSize        = MCAN_TX_BUFF_ELEM_SIZE;
    // Tx Buffer Element Size.

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

    //
    // Take MCAN out of the SW initialization mode
    //
    MCAN_setOpMode(MCANA_DRIVER_BASE, MCAN_OPERATION_MODE_NORMAL);

    while (MCAN_OPERATION_MODE_NORMAL != MCAN_getOpMode(MCANA_DRIVER_BASE))
    {

    }
}
