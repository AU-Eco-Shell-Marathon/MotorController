#include "Filter_1.h"
#include "Filter_1_PVT.h"


/*******************************************************************************
* ChannelA filter coefficients.
* Filter Type is: Biquad
*******************************************************************************/

/* Renamed array for backward compatibility.
*  Should not be used in new designs.
*/
#define ChannelABiquadCoefficients Filter_1_ChannelABiquadCoefficients

/* Number of Biquad sections are: 12 */

const uint8 CYCODE Filter_1_ChannelABiquadCoefficients[Filter_1_BIQUAD_A_SIZE] = 
{
 /* Coefficients of Section 0 */
 0x6Fu, 0x30u, 0x02u, 0x00u, /* Section(0)_A0, 0.0342061519622803 */

 0xDEu, 0x60u, 0x04u, 0x00u, /* Section(0)_A1, 0.0684123039245605 */

 0x6Fu, 0x30u, 0x02u, 0x00u, /* Section(0)_A2, 0.0342061519622803 */

 0xCAu, 0xA0u, 0x31u, 0x00u, /* Section(0)_B1, -0.775438785552979 */

 0xE7u, 0x6Eu, 0xE9u, 0x00u, /* Section(0)_B2, 0.352606058120728 */

 /* Coefficients of Section 1 */
 0x63u, 0xEBu, 0x08u, 0x00u, /* Section(1)_A0, 0.139366865158081 */

 0xC7u, 0xD6u, 0x11u, 0x00u, /* Section(1)_A1, 0.278733968734741 */

 0x63u, 0xEBu, 0x08u, 0x00u, /* Section(1)_A2, 0.139366865158081 */

 0xEEu, 0xC8u, 0x31u, 0x00u, /* Section(1)_B1, -0.777888774871826 */

 0xEBu, 0x8Cu, 0xE4u, 0x00u, /* Section(1)_B2, 0.428899049758911 */

 /* Coefficients of Section 2 */
 0xDCu, 0xCFu, 0x08u, 0x00u, /* Section(2)_A0, 0.137686729431152 */

 0xB9u, 0x9Fu, 0x11u, 0x00u, /* Section(2)_A1, 0.275373697280884 */

 0xDCu, 0xCFu, 0x08u, 0x00u, /* Section(2)_A2, 0.137686729431152 */

 0x26u, 0x66u, 0x31u, 0x00u, /* Section(2)_B1, -0.771859645843506 */

 0x67u, 0x1Bu, 0xEDu, 0x00u, /* Section(2)_B2, 0.295202493667603 */

 /* Coefficients of Section 3 */
 0xB2u, 0x5Au, 0x09u, 0x00u, /* Section(3)_A0, 0.14616060256958 */

 0x63u, 0xB5u, 0x12u, 0x00u, /* Section(3)_A1, 0.292320966720581 */

 0xB2u, 0x5Au, 0x09u, 0x00u, /* Section(3)_A2, 0.14616060256958 */

 0xB6u, 0xBEu, 0x31u, 0x00u, /* Section(3)_B1, -0.777265071868896 */

 0x6Du, 0xBFu, 0xDDu, 0x00u, /* Section(3)_B2, 0.535191297531128 */

 /* Coefficients of Section 4 */
 0x2Du, 0x1Cu, 0x09u, 0x00u, /* Section(4)_A0, 0.14234471321106 */

 0x59u, 0x38u, 0x12u, 0x00u, /* Section(4)_A1, 0.28468918800354 */

 0x2Du, 0x1Cu, 0x09u, 0x00u, /* Section(4)_A2, 0.14234471321106 */

 0x44u, 0x27u, 0x31u, 0x00u, /* Section(4)_B1, -0.768021583557129 */

 0x10u, 0xEFu, 0xEFu, 0x00u, /* Section(4)_B2, 0.251033782958984 */

 /* Coefficients of Section 5 */
 0x80u, 0x75u, 0x08u, 0x00u, /* Section(5)_A0, 0.132171630859375 */

 0x01u, 0xEBu, 0x10u, 0x00u, /* Section(5)_A1, 0.264343500137329 */

 0x80u, 0x75u, 0x08u, 0x00u, /* Section(5)_A2, 0.132171630859375 */

 0x54u, 0xEBu, 0x30u, 0x00u, /* Section(5)_B1, -0.764363288879395 */

 0xCCu, 0x1Fu, 0xF2u, 0x00u, /* Section(5)_B2, 0.216809272766113 */

 /* Coefficients of Section 6 */
 0xD5u, 0x86u, 0x07u, 0x00u, /* Section(6)_A0, 0.117604494094849 */

 0xAAu, 0x0Du, 0x0Fu, 0x00u, /* Section(6)_A1, 0.235208988189697 */

 0xD5u, 0x86u, 0x07u, 0x00u, /* Section(6)_A2, 0.117604494094849 */

 0x3Cu, 0xB6u, 0x30u, 0x00u, /* Section(6)_B1, -0.761122703552246 */

 0x77u, 0xCFu, 0xF3u, 0x00u, /* Section(6)_B2, 0.190462350845337 */

 /* Coefficients of Section 7 */
 0x35u, 0xA1u, 0x06u, 0x00u, /* Section(7)_A0, 0.103589296340942 */

 0x6Au, 0x42u, 0x0Du, 0x00u, /* Section(7)_A1, 0.207178592681885 */

 0x35u, 0xA1u, 0x06u, 0x00u, /* Section(7)_A2, 0.103589296340942 */

 0x28u, 0x8Au, 0x30u, 0x00u, /* Section(7)_B1, -0.758432388305664 */

 0x46u, 0x14u, 0xF5u, 0x00u, /* Section(7)_B2, 0.170637607574463 */

 /* Coefficients of Section 8 */
 0xAFu, 0x66u, 0x06u, 0x00u, /* Section(8)_A0, 0.100017309188843 */

 0x5Eu, 0xCDu, 0x0Cu, 0x00u, /* Section(8)_A1, 0.200034618377686 */

 0xAFu, 0x66u, 0x06u, 0x00u, /* Section(8)_A2, 0.100017309188843 */

 0x56u, 0x68u, 0x30u, 0x00u, /* Section(8)_B1, -0.756368160247803 */

 0xFFu, 0xFCu, 0xF5u, 0x00u, /* Section(8)_B2, 0.156433343887329 */

 /* Coefficients of Section 9 */
 0xD2u, 0x46u, 0x06u, 0x00u, /* Section(9)_A0, 0.0980725288391113 */

 0xA5u, 0x8Du, 0x0Cu, 0x00u, /* Section(9)_A1, 0.196145296096802 */

 0xD2u, 0x46u, 0x06u, 0x00u, /* Section(9)_A2, 0.0980725288391113 */

 0x7Bu, 0x51u, 0x30u, 0x00u, /* Section(9)_B1, -0.754973173141479 */

 0x4Eu, 0x93u, 0xF6u, 0x00u, /* Section(9)_B2, 0.14725923538208 */

 /* Coefficients of Section 10 */
 0x42u, 0x37u, 0x06u, 0x00u, /* Section(10)_A0, 0.0971226692199707 */

 0x84u, 0x6Eu, 0x0Cu, 0x00u, /* Section(10)_A1, 0.194245338439941 */

 0x42u, 0x37u, 0x06u, 0x00u, /* Section(10)_A2, 0.0971226692199707 */

 0xF9u, 0x45u, 0x30u, 0x00u, /* Section(10)_B1, -0.754270792007446 */

 0x13u, 0xDDu, 0xF6u, 0x00u, /* Section(10)_B2, 0.142756700515747 */

 /* Coefficients of Section 11 */
 0xF0u, 0xAAu, 0x3Bu, 0x00u, /* Section(11)_A0, 0.932308197021484 */

 0xE0u, 0x55u, 0x77u, 0x00u, /* Section(11)_A1, 1.86461639404297 */

 0xF0u, 0xAAu, 0x3Bu, 0x00u, /* Section(11)_A2, 0.932308197021484 */

 0x1Bu, 0x26u, 0x31u, 0x00u, /* Section(11)_B1, -0.767950773239136 */

 0xDDu, 0x2Eu, 0xD3u, 0x00u, /* Section(11)_B2, 0.700264692306519 */
};

