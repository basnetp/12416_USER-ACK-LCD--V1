/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation


//Header 
#define header_topleft_x 0
#define header_topleft_y 0
#define header_bottomright_x 319
#define header_bottomright_y 59

//Footer 
#define footer_height 50        //Thickness of the footer bar
#define footer_topleft_x 0
#define footer_topleft_y footer_bottomright_y - footer_height
#define footer_bottomright_x 319
#define footer_bottomright_y 239
#define footer_text_top_x 0
#define footer_text_top_y 199
#define footer_text_height 30

//Body 
#define body_midpoint_x 160
#define body_midpoint_y 125

//Dash animation
#define anim_dash_start_x 0
#define anim_dash_start_y footer_topleft_y
#define anim_dash_width  320
#define anim_dash_height 1

//Battery icon
#define batt_icon_width 40
#define batt_icon_height 60
#define batt_icon_pos_x 54
#define batt_icon_pos_y (body_midpoint_y-(batt_icon_height/2))



//Battery numeric text
//This is the topleft coordinate of the text, where the processor starts drawing the pixel.
#define batt_num_pos_x 100 
#define batt_num_pos_y 68 
#define batt_num_height
//int batt_lvl_y = 78, batt_lvl_x = 118;
#define batt_font_size_y 107
#define batt_font_size_x 70
#define perc_pos_x 240
#define perc_pos_y 120
#define perc_font_size_x 18
#define perc_font_size_y 22

//extern enum UI_STATE;

//Color definitions
#define GRAY_10_ SILVER//GFX_RGBConvert(10, 10, 10)//INDEX_82
#define CS_BLUE PACIFIC_BLUE//GFX_RGBConvert(0, 0, 128)//INDEX_144
# define DISCLAIMER_BKGCLR WHITE
#define DISCLAIMER_TXTCLR BLACK
#define DRIVE_BKGCLR CORNFLOWER//MAGIC_MINT
#define DRIVE_BKGCLR_DRK CORNFLOWER
#define FOOTER_CLR_RED TORCH_RED
#define FOOTER_CLR_YELLOW SUNGLOW
#define FOOTER_CLR_GREEN ULTRA_GREEN
#define FOOTER_CLR_BLUE


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

