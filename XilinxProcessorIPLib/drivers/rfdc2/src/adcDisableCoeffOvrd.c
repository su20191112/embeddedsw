/*
 * adcDisableCoeffOvrd.c
 *
 *  Created on: Aug 10, 2019
 *      Author: jlantz
 */

/***************************** Include Files *********************************/
#include <metal_api.h>
#include <stdio.h>
#include "shared.h"
#include "xrfdc.h"
#include "adcDisableCoeffOvrd.h"

#ifdef RFDC_CLI

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************** Function Definitions ******************************/



/*****************************************************************************/
/**
*
* cli_adcDisableCoeffOvrd_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_adcDisableCoeffOvrd_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
		{"adcDisableCoeffOvrd", "- <tile> <adc> - Disable Coeff Ovrd"     , 2, *adcDisableCoeffOvrd},
		{" "                       , " "                                  , 0, *cmdComment   },

	};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}

/*****************************************************************************/
/**
*
* adcDisableCoeffOvrd TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcDisableCoeffOvrd (u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	u32 Status = XRFDC_FAILURE;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];

	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\n\r");
	xil_printf("   Disable Coefficients Override for ADC Tile%d ch%d\n\r",Tile_Id,Block_Id);
	xil_printf("      - This should be run to re enable real time calibration \n\r");
	xil_printf("        after loading saved calibration coefficients.    \n\r");

	if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id) == 1) {

		// Disable Cal Coefficients Override for OCB1
		Status = XRFdc_DisableCoefficientsOverride(RFdcInstPtr, Tile_Id, Block_Id,XRFDC_CAL_BLOCK_OCB1);
		if (Status != XRFDC_SUCCESS) {
			xil_printf("   OCB1 Disable Coefficients Override error. \r\n");
			return;
		} else {
			xil_printf("   OCB1 Disable Coefficients Override complete. \r\n");
		}

		// Disable Cal Coefficients Override for OCB2
		Status = XRFdc_DisableCoefficientsOverride(RFdcInstPtr, Tile_Id, Block_Id,XRFDC_CAL_BLOCK_OCB2);
		if (Status != XRFDC_SUCCESS) {
			xil_printf("   OCB2 Disable Coefficients Override error. \r\n");
			return;
		} else {
			xil_printf("   OCB2 Disable Coefficients Override complete. \r\n");
		}

		// Disable Cal Coefficients Override for GCB
		Status = XRFdc_DisableCoefficientsOverride(RFdcInstPtr, Tile_Id, Block_Id,XRFDC_CAL_BLOCK_GCB);
		if (Status != XRFDC_SUCCESS) {
			xil_printf("   GCB Disable Coefficients Override error. \r\n");
			return;
		} else {
			xil_printf("   GCB Disable Coefficients Override complete. \r\n");
		}

		// Disable Cal Coefficients Override for TSCB
		Status = XRFdc_DisableCoefficientsOverride(RFdcInstPtr, Tile_Id, Block_Id,XRFDC_CAL_BLOCK_TSCB);
		if (Status != XRFDC_SUCCESS) {
			xil_printf("   TSCB Disable Coefficients Override error. \r\n");
			return;
		} else {
			xil_printf("   TSCB Disable Coefficients Override complete. \r\n");
		}

	} else {
		(xil_printf("Tile%d Ch%d is not available.\n\r",Tile_Id,Block_Id ));
	}

	xil_printf("###############################################\r\n\n");

	return;
}

#endif /*RFDC_CLI*/