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
static u8 UserApp1_au8Name[] = "Ayden";
static u8 UserApp1_RowList[] = {U8_LCD_SMALL_FONT_LINE0,
                       U8_LCD_SMALL_FONT_LINE1,
                       U8_LCD_SMALL_FONT_LINE2,
                       U8_LCD_SMALL_FONT_LINE3,
                       U8_LCD_SMALL_FONT_LINE4,
                       U8_LCD_SMALL_FONT_LINE5,
                       U8_LCD_SMALL_FONT_LINE6,
                       U8_LCD_SMALL_FONT_LINE7
                      };
static int UserApp1_NumRows = 7;
const u16 U16_ANIM_INTRO_PERIODMS = 75; // Period between animation updates
const u16 U16_ANIM_JUMP_PERIODMS = 75; // Period between animation updates
const u16 U16_ANIM_PERIODMS = 200; // Period between animation updates
const u16 U16_ANIM_GAMELOOP_PERIODMS = 75; // Period between animation updates


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
    UserApp1_pfStateMachine = UserApp1SM_Intro;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

  // Clear the Display initially
  LcdClearScreen();

  //PixelAddressType nameLocation = {U16_LCD_TOP_MOST_ROW, U16_LCD_LEFT_MOST_COLUMN};  
  //LcdLoadString(UserApp1_au8Name, LCD_FONT_SMALL, &nameLocation);

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

// drawAll when true will draw all the saved bitmaps at their saved positions and clears the queue for the next draw cycle.
static void UserApp1_Draw(bool drawAll, u8* bitmap, PixelBlockType* bitmapSize) {
  static u8 arrayLen = 0;
  static u8* bitmaps[100] = {};
  static PixelBlockType* bitmapSizes[100] = {};

  if (!drawAll) {
    // If not drawing to screen, add the given data to the arrays
    bitmaps[arrayLen] = bitmap;
    bitmapSizes[arrayLen] = bitmapSize;
    arrayLen++;
  } else {
    // Draw all to screen and reset arrays to empty.
    if (bitmap != NULL && bitmapSize != NULL){
      // If this is not simply a call to actually draw everything, then add the final thing to draw before drawing here.
      bitmaps[arrayLen] = bitmap;
      bitmapSizes[arrayLen] = bitmapSize;
      arrayLen++;
    } 

    LcdClearScreen();

    // Loop through the arrays and print everything to the screen.
    for (int i = 0; i < arrayLen; i++){
      LcdLoadBitmap(bitmaps[i], bitmapSizes[i]);
      bitmaps[i] = 0;
      bitmapSizes[i] = 0;
    }

    arrayLen = 0;
  }
}

static void UserApp1SM_Intro() {
  PixelBlockType sEngenuicsImage;
  static u8 u8RowPosition = 0;
  
  extern const u8 aau8EngenuicsLogoBlackQ1[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX];

  static int anim_increment = 0;
  static u16 counter = U16_ANIM_INTRO_PERIODMS;

  counter--;

  if (anim_increment <= 40 && counter == 0){
    // Reset the anim counter so that in another time period the next anim frame runs.
    counter = U16_ANIM_INTRO_PERIODMS;

    // Slide in from left
    sEngenuicsImage.u16RowStart = U16_LCD_ROWS - 25;
    sEngenuicsImage.u16ColumnStart = anim_increment;
    sEngenuicsImage.u16RowSize = 25;
    sEngenuicsImage.u16ColumnSize = 25;
    UserApp1_Draw(TRUE, aau8EngenuicsLogoBlackQ1, &sEngenuicsImage);
        
    /* Adjust the row by one every few iterations */
    if( (anim_increment % 3) == 0)
    {
      u8RowPosition++;
      /* On the last iteration set, adjust one more row to bring the icon together */
      if(u8RowPosition == 13)
      {
        u8RowPosition = 14;
      }
    }

    anim_increment++;
  } 
  
  if (anim_increment > 40) {
    UserApp1_pfStateMachine = UserApp1SM_GameLoop;
  }
}

static void UserApp1SM_GameLoop(void)
{
  /*
  Potentially wrap this whole thing in a counter like jump or intro so that it only draws once every say 50ms. Once every 50ms, it would
  call the jump function which would increment its anim counter and it would add the new frame to draw to the screen. 
  Then potentially draw all of those to the screen. See if that still violates timing

  A further fix/idea is to have a function that takes the bitmaps and the PixelBlockTypes and scales up the bitmap to the size of the screen,
  positioning it based on the PixelBlockType and setting all other bits to 0. 
  Then, treat each of these as a layer and loop through each pixel in the screen for each bitmap and if one is on, write it to the final
  bitmap that will be drawn to the screen. Could use this to say if two or more are on, should they be overlapping? That would be
  a collision so potentially they lose etc.

  Use srand to seed the random thing once I think. Then use rand to get random numbers
  */
  
  extern const u8 aau8EngenuicsLogoBlackQ1[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX];

  static u16 counter = U16_ANIM_GAMELOOP_PERIODMS;

  counter--;

  static jumping = FALSE;

  if (counter == 0) {
    // This will run once every period. In here, call the functions to draw things for example if they are jumping and then
    // draw everything at the end.
    counter = U16_ANIM_GAMELOOP_PERIODMS;

    PixelBlockType sEngenuicsImage;
    

    if (WasButtonPressed(BUTTON1)) {
      if (jumping == FALSE) {
        jumping = TRUE;
        ButtonAcknowledge(BUTTON1);
      } else {
        // They are already jumping so don't queue up a jump
        ButtonAcknowledge(BUTTON1);
      }
    }
    
    if (jumping) {
      UserApp1_Jump(&jumping);
      /*PixelBlockType sEngenuicsImage2;
      sEngenuicsImage2.u16RowStart = U16_LCD_ROWS - 25;
      sEngenuicsImage2.u16ColumnStart = 25;
      sEngenuicsImage2.u16RowSize = 25;
      sEngenuicsImage2.u16ColumnSize = 25;
      UserApp1_Draw(TRUE, &aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage2);*/
    } else {
      PixelBlockType sEngenuicsImage2;
      // If not jumping, draw the character at the default position.
      sEngenuicsImage2.u16RowStart = U16_LCD_ROWS - 25;
      sEngenuicsImage2.u16ColumnStart = 40;
      sEngenuicsImage2.u16RowSize = 25;
      sEngenuicsImage2.u16ColumnSize = 25;
      UserApp1_Draw(FALSE, &aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage2);
    }

    UserApp1_MoveBackground();

    // A Draw call to draw everything 
    UserApp1_Draw(TRUE, NULL, NULL);
  }

  /*static int curRow = 0;

  if (WasButtonPressed(BUTTON0)){
    ButtonAcknowledge(BUTTON0);

    LcdClearScreen();

    curRow++;

    if (curRow > UserApp1_NumRows){
      curRow = 0;
    } 

    PixelAddressType nameLocation = {UserApp1_RowList[curRow], U16_LCD_LEFT_MOST_COLUMN};  
    LcdLoadString(UserApp1_au8Name, LCD_FONT_SMALL, &nameLocation);
  }

    if (WasButtonPressed(BUTTON1)){
    ButtonAcknowledge(BUTTON1);

    LcdClearScreen();

    curRow--;

    if (curRow < 0){
      curRow = UserApp1_NumRows;
    } 

    PixelAddressType nameLocation = {UserApp1_RowList[curRow], U16_LCD_LEFT_MOST_COLUMN};  
    LcdLoadString(UserApp1_au8Name, LCD_FONT_SMALL, &nameLocation);*//*
  }*/
} /* end UserApp1SM_Idle() */
     
static void UserApp1_Jump(bool* jumping){
  static PixelBlockType sEngenuicsImage;
  static PixelBlockType sEngenuicsImage2;

  extern const u8 aau8EngenuicsLogoBlackQ1[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX];

  static u8 u8RowPosition = 0;

  static int anim_increment = 0;
  static int anim_increment2 = 0;

/*  static u16 counter = U16_ANIM_JUMP_PERIODMS;

  counter--;*/

  if (anim_increment <= 30 /*&& counter == 0*/){
    // Reset the counter
    //counter = U16_ANIM_JUMP_PERIODMS;

    sEngenuicsImage2.u16RowStart = U16_LCD_ROWS - 25;
    sEngenuicsImage2.u16ColumnStart = U16_LCD_COLUMNS - 25 - anim_increment;
    sEngenuicsImage2.u16RowSize = 25;
    sEngenuicsImage2.u16ColumnSize = 25;
    //UserApp1_Draw(FALSE, &aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage2);

    sEngenuicsImage.u16RowStart = U16_LCD_ROWS - 25 - anim_increment;
    sEngenuicsImage.u16ColumnStart = 40;
    sEngenuicsImage.u16RowSize = 25;
    sEngenuicsImage.u16ColumnSize = 25;
    UserApp1_Draw(FALSE, &aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage);

    anim_increment++;
  } else if (anim_increment2 <= 30 /*&& counter == 0*/){
    //counter = U16_ANIM_JUMP_PERIODMS;

    sEngenuicsImage.u16RowStart = (U16_LCD_ROWS - 25 - anim_increment) + anim_increment2;
    sEngenuicsImage.u16ColumnStart = 40;
    sEngenuicsImage.u16RowSize = 25;
    sEngenuicsImage.u16ColumnSize = 25;
    UserApp1_Draw(FALSE, &aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage);
    
    anim_increment2++;
  } /*else {
    // If in between counter cycles, draw at current position but don't change anim_increment
    if (anim_increment <= 20){
      sEngenuicsImage.u16RowStart = U16_LCD_ROWS - 25 - anim_increment;
      sEngenuicsImage.u16ColumnStart = 40;
      sEngenuicsImage.u16RowSize = 25;
      sEngenuicsImage.u16ColumnSize = 25;
      UserApp1_Draw(FALSE, &aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage);
    } else if (anim_increment2 <= 20) {
      sEngenuicsImage.u16RowStart = (U16_LCD_ROWS - 25 - anim_increment) + anim_increment2;
      sEngenuicsImage.u16ColumnStart = 40;
      sEngenuicsImage.u16RowSize = 25;
      sEngenuicsImage.u16ColumnSize = 25;
      UserApp1_Draw(FALSE, &aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage);
    }
  }*/

  if (anim_increment >= 30 && anim_increment2 >= 30){
    *jumping = FALSE;
    anim_increment = 0;
    anim_increment2 = 0;
  }
}

static u16 UserApp1_MoveBackground(void) {
  static PixelBlockType sEngenuicsImage;
  static PixelBlockType sEngenuicsImageStationary;

  extern const u8 aau8EngenuicsLogoBlackQ1[U8_LCD_IMAGE_ROW_SIZE_25PX][U8_LCD_IMAGE_COL_BYTES_25PX];

  static u8 u8RowPosition = 0;

  static int anim_increment = 0;

  if (anim_increment <= 60){
    sEngenuicsImage.u16RowStart = U16_LCD_ROWS - 25;
    sEngenuicsImage.u16ColumnStart = U16_LCD_COLUMNS - 25 - anim_increment;
    sEngenuicsImage.u16RowSize = 25;
    sEngenuicsImage.u16ColumnSize = 25;
    UserApp1_Draw(FALSE, &aau8EngenuicsLogoBlackQ1[0][0], &sEngenuicsImage);

    anim_increment++;
  } 
  
  if (anim_increment >= 60){
    anim_increment = 0;
  }

  return sEngenuicsImage.u16ColumnStart;
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
