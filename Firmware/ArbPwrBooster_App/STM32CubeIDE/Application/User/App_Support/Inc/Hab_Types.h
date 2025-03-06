 /*****************************************************************************************************
 * @file 			Hab_Types.h
 * @brief			This is the Header file used to support Hab_Types.h.c
 * ****************************************************************************************************
 * @author original Hab Collector (habco)\n
 *
 * @version         See Main_Support.h: FW_MAJOR_REV, FW_MINOR_REV, FW_TEST_REV
 *
 * @param Development_Environment \n
 * Hardware:        <IC PN>  \n
 * IDE:             STMCubeIDE VER 1.17.0 \n
 * Compiler:        GCC \n
 * Editor Settings: 1 Tab = 4 Spaces, Recommended Courier New 11
 *
 * @note            <Add user notes starting here>
 *
 *                  This is an embedded application
 *                  It will be necessary to consult the reference documents to fully understand the code
 *                  It is suggested that the documents be reviewed in the order shown.
 *                    Schematic: <Schematic PN>
 *				      STM32F746G_DISCO
 *                    IMR
 *
 * @copyright       IMR Engineering, LLC
 ******************************************************************************************************/

#ifndef HAB_TYPES_H_
#define HAB_TYPES_H_
#ifdef __cplusplus
extern"C" {
#endif

#include <stdint.h>
#include <stdbool.h>

// DEFINES
#define INTEGER_ROUNDING                            (0.5)


// TYPES AND ENUMS
#ifndef bool_t
    #ifdef FALSE
        #undef FALSE
    #endif
    #ifdef TRUE
        #undef TRUE
    #endif
typedef enum
{
    FALSE = 0,
    TRUE
}bool_t;
#endif

typedef union
{
    struct
    {
        uint8_t     Byte0;
        uint8_t     Byte1;
    };
    uint16_t        Value;
}Type_Integer_UINT16;

typedef union
{
    struct
    {
        uint8_t     Byte0;
        uint8_t     Byte1;
        uint8_t     Byte2;
        uint8_t     Byte3;
    };
    uint32_t        Value;
}Type_Integer_UINT32;

typedef union
{
    struct
    {
        uint8_t     Byte0;
        uint8_t     Byte1;
        uint8_t     Byte2;
        uint8_t     Byte3;
    };
    float           Value;
}Type_Float;


// MACROS
#ifndef MIN
#define MIN(a, b)               (((a) < (b))? (a) : (b))
#endif

#ifndef MAX
#define MAX(a, b)               (((a) > (b))? (a) : (b))
#endif

#ifdef __cplusplus
}
#endif
#endif /* HAB_TYPES_H_*/
