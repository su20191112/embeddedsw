/*
 * adcFreezeCal.c
 *
 *  Created on: Aug 10, 2019
 *      Author: jlantz
 */
/***************************** Include Files *********************************/
#include <stdio.h>
#include "shared.h"
#include "xrfdc.h"
#include "adc_LinkCoupling.h"

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
* cli_adcFreezeCalStatus_init Add functions from this file to CLI
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void cli_adcGetLinkCoupling_init(void)
{
	static CMDSTRUCT cliCmds[] = {
		//000000000011111111112222    000000000011111111112222222222333333333
		//012345678901234567890123    012345678901234567890123456789012345678
		{"################ ADC Get Link Coupling #################" , " " , 0, *cmdComment   },
		{"adcGetLinkCoupling" , "- Get Link Coupling Status"         , 0, *adcGetLinkCoupling},
		{" "                       , " "                                      , 0, *cmdComment   },
};

	cli_addCmds(cliCmds, sizeof(cliCmds)/sizeof(cliCmds[0]));
}

/*****************************************************************************/
/**
*
* adcFreezeCalStatus TBD
*
* @param	None
*
* @return	None
*
* @note		TBD
*
******************************************************************************/
void adcGetLinkCoupling (u32 *cmdVals)
{
	u32 Tile_Id;
	u32 Block_Id;
	XRFdc_IPStatus ipStatus;
	XRFdc* RFdcInstPtr = &RFdcInst;
	u32 getlinkCouplingSettings;

	Tile_Id = cmdVals[0];
	Block_Id = cmdVals[1];

	XRFdc_GetIPStatus(RFdcInstPtr, &ipStatus);
	xil_printf("\n\r###############################################\n\r");
	xil_printf("=== ADC Get Link Coupling ===\n\r");

	for (Tile_Id=0; Tile_Id<=3; Tile_Id++) {
		for(Block_Id=0; Block_Id<=3; Block_Id++) {
	//		xil_printf("   ADC Tile%d ch%d \r\n", Tile_Id, Block_Id);
			if(XRFdc_IsADCBlockEnabled(RFdcInstPtr, Tile_Id, Block_Id)) {
				XRFdc_GetLinkCoupling(RFdcInstPtr, Tile_Id, Block_Id, &getlinkCouplingSettings);
				if (getlinkCouplingSettings == 1){
					xil_printf("   ADC Tile%d ch%d Link Coupling: %d (AC Coupled)\r\n", Tile_Id, Block_Id,getlinkCouplingSettings);
				}
				if (getlinkCouplingSettings == 0){
					xil_printf("   ADC Tile%d ch%d Link Coupling: %d (DC Coupled)\r\n", Tile_Id, Block_Id,getlinkCouplingSettings);
				}
			}
		}
	}

	xil_printf("###############################################\r\n\n");

	return;
}

#endif