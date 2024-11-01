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

  PWMAudioSetFrequency(BUZZER1, 500);

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
  if(IsButtonPressed(BUTTON0)){
    PWMAudioOn(BUZZER1);
  } else {
    PWMAudioOff(BUZZER1);
  }
  

  // static u16 u16Notes[] = {C4, D4, E4, F4, G4, A4, B4, C5};

  // Hedwig's Theme
  
  static u16 u16Notes[] = {D4, G4, A4S, A4, G4, D5, C5, A4, G4, A4S, A4, F4, G4S, D4, D4, G4, A4S, A4, 
  G4, D5, F5, E5, D5S, B4, D5S, D5, C5S, C4S, B4, G4, A4S, D5, A4S, D5, A4S, D5S, D5, C5S, A4, A4S, 
  D5, C5S, C4S, D4, D5, A4S, D5, A4S, F5, E5, D5S, B4, D5S, D5, C5S, C4S, A4S, G4};
  

  // Hall of the mountain king Doesn't work very well
  /*static u16 u16Notes[] = {D3, E3, F3, G3, A3, F3, A3, G3S, E3, G3S, G3, E3, G3, D3, E3, F3, 
  G3, A3, F3, A3, D4, C4, A3, F3, A3, C4, D3, E3, F3, G3, A3, F3, A3, G3S, E3, G3S, G3, E3, G3, 
  D3, E3, F3, G3, A3, F3, A3, D4, C4, A3, F3, A3, C4, A3, B3, C4S, D4, E4, C4, E4, F4, C4S, F4, E4, 
  C4, E4, A3, B3, C4S, D4, E4, C4, E4, F4, C4S, F4, E4, A3, B3, C4S, D4, E4, C4, E4, F4, C4S, F4, E4, 
  C4, E4, A3, B3, C4S, D4, E4, C4, E4, F4, C4S, F4, E4, D3, E3, F3, G3, A3, F3, A3, G3S, E3, G3S, G3, 
  E3, G3, D3, E3, F3, G3, A3, F3, A3, D4, C4, A3, F3, E3, D3};*/

  // Pink Panther
  /*
  static u16 u16Notes[] = {D4S, E4, F4S, G4, D4S, E4, F4S, G4, C5, B4, E4, G4, B4, A4S, A4, G4, E4, D4, E4, D4S, E4, F4S, G4, D4S, E4, F4S,
   G4, C5, B4, G4, B4, E5, D5S, D5, D4S, E4, F4S, G4, D4S, E4, F4S, G4, C5, B4, E4, G4, B4, A4S, A4,
    G4, E4, D4, E4, E5, D5, B4, A4, G4, E4, A4S, A4, A4S, A4, A4S, A4, A4S, A4, G4, E4, D4, E4, E4, E4};
  */

  // Star Wars Cantina
  /*
  static u16 u16Notes[] = {B4, E5, B4, E5, B4, E5, B4, A4S, B4, B4, A4S, B4, A4S, G4S, A4S, G4S, G4S, 
  E4, B4, E5, B4, E5, B4, E5, B4, A4S, B4, A4S, A4S, G4S, A4S, D5S, C5S, B4, A4S, B4, E5, B4, E5, B4, 
  E5, B4, A4S, B4, D5S, D5S, B4, A4S, G4S, E4, E4, G4S, B4, D5S, F5S, E5, A4S, A4S, B4, G4S};
  */

  // Imperial March
  /*
  static u16 u16Notes[] = {A4S, A4S, A4S, A4S, A4S, A4S, F4S, A4S, A4S, A4S, A4S, A4S,
   A4S, F4S, A4S, A4S, A4S, F4S, C5S, A4S, F4S, C5S, A4S, E5, E5, E5, F5S, C5S, A4S, F4S
   , C5S, A4S, A5S, A4S, A4S, A5S, G5S, G5S, D5S, D5S, D5S, A4S, D5S, D5S, C5S, C5S, B4, C5S, F4S, G4S, F4S, A4S, C5S, A4S
   , C5S, E5, A5S, A4S, A4S, A5S, G5S, G5S, D5S, D5S, D5S, A4S, D5S, D5S, C5S, C5S, B4, C5S, 
   F4S, G4S, F4S, A4S, A4S, F4S, C5S, A4S};
  */

  static u8 u8NoteIndex = 0;
  static bool goingUp = TRUE;

  if(WasButtonPressed(BUTTON1)){
    ButtonAcknowledge(BUTTON1);

    if (goingUp) 
      u8NoteIndex++;
    else 
      u8NoteIndex--;

    if(u8NoteIndex == (u8)(sizeof(u16Notes) / sizeof(u16))){
      //u8NoteIndex = 0;

      // If it equals the max value of index, go back down
      goingUp = FALSE;
      u8NoteIndex--;
    } else if (u8NoteIndex == 0){
      // If it is the min, go back up
      goingUp = TRUE;
    }

    PWMAudioSetFrequency(BUZZER1, u16Notes[u8NoteIndex]);
  }

  if (IsButtonHeld(BUTTON1, 2000)){
    LedOn(RED1);
    u8NoteIndex = 0;
    goingUp = TRUE;
    LedOff(RED1);
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
