/******************************************************************************
* File Name: main.c
*
* Version: 1.10
*
* Description: This is the source code for the Blinking LED code example.
*
* Related Document: CE195352_PSoC_5LP_Blinking_LED.pdf
*
* Hardware Dependency: See CE195352_PSoC_5LP_Blinking_LED.pdf
*
*******************************************************************************
* Copyright (2018), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* (“Software”), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries (“Cypress”) and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software (“EULA”).
*
* If no EULA applies, Cypress hereby grants you a personal, nonexclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death (“High Risk Product”). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability.
*******************************************************************************/

#include <project.h>
#include <stdlib.h>

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  The PWM is configured and started in the PWM_1_Start() function.
*  The firmware then enters an idle loop, leaving control of the project
*  to the fixed function PWM peripheral.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
float period = 9600;

void writeMS(float ms) {
    float perc = ms / 20; // 20 ms period
    int cmp = (int) ((1 - perc) * period);
    PWM_WriteCompare(cmp);
    CyDelay(1000);
}


int main()
{
    CyDelay(1000);
    /* Serial */
    CyGlobalIntEnable;
    UART_Start();
    int ch; 
    char buffer[512];
    uint16_t buffer_count = 0;
    
    /* PWM */
    PWM_Start();    
    int cmp = 9000;
    int mltplr = 1;
    while (1) {
        ch = UART_GetChar(); // returns an int apparently
        
        if (ch > 0) 
        { 
            if (ch == 13) { // if new line pressed
                // Null terminate
                buffer[buffer_count++] = '\0';

                // Print back what was typed
                UART_PutString(buffer);
                UART_PutStringConst("\n");

                // Parse int from buffer
                cmp = atoi(buffer);
                
                // Spin motor:
                for (int i = 0; i < 1; i++) {
                    PWM_WriteCompare(cmp); 
                    CyDelay(20);
                }

                // Reset buffer
                buffer_count = 0;
            }
            else if (ch >= 48 && ch <= 57 && buffer_count < sizeof(buffer) / sizeof(buffer[0]) - 1) { // if digit is pressed
                // Put character into buffer
                buffer[buffer_count++] = ch;
            }
        }
        
        continue;
        
        for (int i = 0; i < 2; i++) {
            PWM_WriteCompare(cmp); 
            CyDelay(100);    
            if (cmp == 9000 && 1==2) {
                i = 0;
            }
        }
        
        cmp += 1 * mltplr;
        if (cmp >= 9120 || cmp <= 8640) {
            mltplr *= -1;
        }
        /*
        if (cmp == 8800 ) {
            PWM_WriteCompare(cmp); 
            CyDelay(500);
            while (1) {}   
        }
        
       */
    }
    /*
    float ms = 2;
    int mltplr = -1;
    while (1) {
        writeMS(ms);
        // 480 = 9120 - 8640 = 9600 * .95 - 9600 * .9
        // number of increments
        ms += 1/480 * mltplr; 
        if (ms <= 1 || ms >= 2) {
            mltplr *= -1;   
        }
    }
    */
}


/* [] END OF FILE */
