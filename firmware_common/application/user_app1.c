/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  // Turn on blue left side LED
  /*LedOn(BLUE0);

  LedToggle(RED3);

  LedBlink(GREEN2, LED_2HZ);

  LedPWM(BLUE1, LED_PWM_5);*/

  // Initialize all LEDs to off.
  LedOff(RED0);
  LedOff(BLUE0);
  LedOff(GREEN0);
  
  LedOff(RED1);
  LedOff(BLUE1);
  LedOff(GREEN1);

  LedOff(RED2);
  LedOff(BLUE2);
  LedOff(GREEN2);

  LedOff(RED3);
  LedOff(BLUE3);
  LedOff(GREEN3);

  LedOff(LCD_BL);

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
  static u16 u16BlinkPeriod = 0;
  static u8 u8Counter = 0;
  static u8 u8ColourIndex = 0;

  static u8 u8ColorArray[][3] = {
    {RED0, 0xff, 0xff},
    {RED0, GREEN0, 0xff},
    {0xff, GREEN0, 0xff},
    {0xff, GREEN0, BLUE0},
    {0xff, 0xff, BLUE0},
    {RED0, 0xff, BLUE0},
    {RED0, GREEN0, BLUE0},
  };

  // Decrement blink counter
  u16BlinkPeriod++;

  if (u16BlinkPeriod == 250)
  {
    // Toggle blue LED
    LedToggle(RED3);

    // Reset blink counter
    u16BlinkPeriod = 0;
    
    u8Counter++;

    if (u8Counter == 16){
        u8Counter = 0;
    }

    for (u8 i = 0; i < (U8_TOTAL_LEDS - 1); i++){
      // Converts the integer i to a led in the enumerator. Goes through all and turns them off.
      LedOff((LedNameType)i);
    }

    /*if (u8Counter & 0x01){
      LedOn(RED3);
    }

    if (u8Counter & 0x02){
      LedOn(GREEN2);
    }

    if (u8Counter & 0x04){
      LedOn(BLUE1);
    }

    if (u8Counter & 0x08){
      LedOn(RED0);
      LedOn(BLUE0);
    }*/

    
    // Loop that shows the same functionality but with one colour. The other method may be better/easier to understand though.
    /*
    u8 mask = 0x01;
    LedNameType currentLED = GREEN3;

    while (mask <= 0x08){
      if (u8Counter & mask){
        LedOn(currentLED);
      }

      // Bitwise shift the mask by one. So it goes 0x01 -> 0x02 -> 0x04 -> 0x08 Since 0001 -> 0010 -> 0100 -> 1000
      mask = mask << 1;

      // Will increment current led to the next LED until it rolls over to the next colour at the first LED.
      // The loop should be done before this occurs though
      currentLED--;
    }
    */

    // Loop that shows the same functionality but with all colours.
    u8 mask = 0x01;
    // The 3rd LED is the furthest right so LSB (Least Significant Bit)
    u8 currentLEDOffset = 3;

    while (mask <= 0x08){
      if (u8Counter & mask){
        // The j < 3 is since there are 3 colours RG and B for each LED
        for (u8 j = 0; j < 3; j++){
          if (u8ColorArray[u8ColourIndex][j] != 0xff){
            // If the colour is actually a colour, turn on that colour. Use the offset to move from the LED0 in the array
            // to the respective LEDs for each bit.
            LedNameType test = (u8ColorArray[u8ColourIndex][j]) + currentLEDOffset;
            LedOn((u8ColorArray[u8ColourIndex][j]) + currentLEDOffset);
          }
        }
      }

      // Bitwise shift the mask by one. So it goes 0x01 -> 0x02 -> 0x04 -> 0x08 Since 0001 -> 0010 -> 0100 -> 1000
      mask = mask << 1;

      // Will increment current led to the next LED until it rolls over to the next colour at the first LED.
      // The loop should be done before this occurs though
      currentLEDOffset--;
      u8ColourIndex++;

      if (u8ColourIndex > 6){
        u8ColourIndex = 0;
      }
    }
  }

} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
