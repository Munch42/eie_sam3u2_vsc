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

extern u32 G_u32AntApiCurrentMessageTimeStamp;                            // From ant_api.c
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;            // From ant_api.c
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  // From ant_api.c
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                // From ant_api.c


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static u32 UserApp1_u32DataMsgCount = 0;                  /* ANT_DATA packets received */
static u32 UserApp1_u32TickMsgCount = 0;                  /* ANT_TICK packets received */
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


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
  PixelAddressType sStringLocation;
  u8 au8WelcomeMessage[] = "ANT Slave Demo";
  AntAssignChannelInfoType sChannelInfo;

  extern PixelBlockType G_sLcdClearLine7;  /* From lcd-NHD-C12864LZ.c */

  if(AntRadioStatusChannel(U8_ANT_CHANNEL_USERAPP) == ANT_UNCONFIGURED)
  {
    sChannelInfo.AntChannel = U8_ANT_CHANNEL_PERIOD_HI_USERAPP;
    sChannelInfo.AntChannelType = CHANNEL_TYPE_SLAVE;
    sChannelInfo.AntChannelPeriodHi = U8_ANT_CHANNEL_PERIOD_HI_USERAPP;
    sChannelInfo.AntChannelPeriodLo = U8_ANT_CHANNEL_PERIOD_LO_USERAPP;
    
    sChannelInfo.AntDeviceIdHi = U8_ANT_DEVICE_HI_USERAPP;
    sChannelInfo.AntDeviceIdLo = U8_ANT_DEVICE_LO_USERAPP;
    sChannelInfo.AntDeviceType = U8_ANT_DEVICE_TYPE_USERAPP;
    sChannelInfo.AntTransmissionType = U8_ANT_TRANSMISSION_TYPE_USERAPP;
    
    sChannelInfo.AntFrequency = U8_ANT_FREQUENCY_USERAPP;
    sChannelInfo.AntTxPower = U8_ANT_TX_POWER_USERAPP;
    
    sChannelInfo.AntNetwork = ANT_NETWORK_DEFAULT;
    for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
    {
      sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
    }
    
    AntAssignChannel(&sChannelInfo);
  }  else {
    LedBlink(RED3, LED_2HZ);
  }

  // Update the led and lcd message for ant slave demo.
  LedOn(RED0);

  /* Initialize the LCD message */
  sStringLocation.u16PixelColumnAddress =
    U16_LCD_CENTER_COLUMN - ( strlen((char const*) au8WelcomeMessage) * (U8_LCD_SMALL_FONT_COLUMNS + U8_LCD_SMALL_FONT_SPACE) / 2);
  sStringLocation.u16PixelRowAddress = U8_LCD_SMALL_FONT_LINE7;
  LcdClearPixels(&G_sLcdClearLine7);
  LcdLoadString(au8WelcomeMessage, LCD_FONT_SMALL, &sStringLocation);

  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_WaitAntReady;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    LedBlink(RED0, LED_4HZ);
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
/* Wait for ANT channel to be configured */
static void UserApp1SM_WaitAntReady(void)
{
  if(AntRadioStatusChannel(U8_ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    LedOn(BLUE3);
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  } /* end if(AntOpenChannelNumber)... */
  
  else if (AntRadioStatusChannel(U8_ANT_CHANNEL_USERAPP) == ANT_UNCONFIGURED){
    LedOn(BLUE2);
  }
} /* end UserApp1SM_WaitAntReady() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Hold here until ANT confirms the channel is open */
static void UserApp1SM_WaitChannelOpen(void)
{
  if(AntRadioStatusChannel(U8_ANT_CHANNEL_USERAPP) == ANT_OPEN)
  {
    LedOn(GREEN0);
    UserApp1_pfStateMachine = UserApp1SM_ChannelOpen;
  }

  // Check for timeout
  if (IsTimeUp(&UserApp1_u32Timeout, 500)){
    AntCloseChannelNumber(U8_ANT_CHANNEL_USERAPP);
    LedOff(GREEN0);
    LedOn(BLUE1);
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }

} /* end UserApp1SM_WaitChannelOpen() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* Process messages while channel is open */
static void UserApp1SM_ChannelOpen(void)
{
  static u8 u8LastState = 0xff;
  static u8 au8TickMessage[] = "EVENT x\n\r";
  static u8 au8LastAntData[ANT_APPLICATION_MESSAGE_BYTES] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  bool bGotNewData;
  static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
  static PixelAddressType sStringLocation;
  u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";

  extern PixelBlockType G_sLcdClearLine7;  /* From lcd-NHD-C12864LZ.c */

  if(WasButtonPressed(BUTTON0)){
    ButtonAcknowledge(BUTTON0);

    AntCloseChannelNumber(U8_ANT_CHANNEL_USERAPP);
    u8LastState = 0xff;
    LedOff(BLUE0);
    LedBlink(GREEN0, LED_2HZ);

    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_pfStateMachine = UserApp1SM_WaitChannelClose;
  }

  if(AntRadioStatusChannel(U8_ANT_CHANNEL_USERAPP) != ANT_OPEN){
    u8LastState = 0xff;
    LedOff(BLUE0);
    LedBlink(GREEN0, LED_2HZ);

    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_pfStateMachine = UserApp1SM_WaitChannelClose;
  }

  if(AntReadAppMessageBuffer()){
    // New data message what is it?
    if(G_eAntApiCurrentMessageClass == ANT_DATA){
      LedOff(GREEN0);
      LedOn(BLUE0);
      u8LastState = 0xff;
      UserApp1_u32DataMsgCount++;

      // Check if the new data is the same as the old data
      bGotNewData = FALSE;
      for (u8 i = 0; i < ANT_APPLICATION_MESSAGE_BYTES; i++){
        if(G_au8AntApiCurrentMessageBytes[i] != au8LastAntData[i]){
          bGotNewData = TRUE;
          au8LastAntData[i] = G_au8AntApiCurrentMessageBytes[i];

          au8DataContent[2 * i] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i]   / 16);
          au8DataContent[2*i+1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16);
        }
      }

      if(bGotNewData){
        // Write the board string in the middle of last row 
        sStringLocation.u16PixelColumnAddress =  
          U16_LCD_CENTER_COLUMN - ( strlen((char const*)au8DataContent) * (U8_LCD_SMALL_FONT_COLUMNS + U8_LCD_SMALL_FONT_SPACE) / 2 );
        sStringLocation.u16PixelRowAddress = U8_LCD_SMALL_FONT_LINE7;
        LcdClearPixels(&G_sLcdClearLine7);
        LcdLoadString(au8DataContent, LCD_FONT_SMALL, &sStringLocation);

        au8TestMessage[7]++;
        if(au8TestMessage[7] == 0)
        {
          au8TestMessage[6]++;
          if(au8TestMessage[6] == 0)
          {
            au8TestMessage[5]++;
          }
        }
        AntQueueBroadcastMessage(U8_ANT_CHANNEL_USERAPP, au8TestMessage);

        // Check for a special packet and respond
        if(G_au8AntApiCurrentMessageBytes[0] == 0xA5){
          if(G_au8AntApiCurrentMessageBytes[1] == 1){
            LedOn(RED2);
            LedOn(RED3);
          } else {
            LedOff(RED2);
            LedOff(RED3);
          }

          if(G_au8AntApiCurrentMessageBytes[2] == 1){
            LedOn(GREEN2);
            LedOn(GREEN3);
          } else {
            LedOff(GREEN2);
            LedOff(GREEN3);
          }

          if(G_au8AntApiCurrentMessageBytes[3] == 1){
            LedOn(BLUE2);
            LedOn(BLUE3);
          } else {
            LedOff(BLUE2);
            LedOff(BLUE3);
          }
        }
      }

    } else if (G_eAntApiCurrentMessageClass == ANT_TICK){
      // A channel period has gone by
      UserApp1_u32TickMsgCount++;

      if (u8LastState != G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX]){
        u8LastState = G_au8AntApiCurrentMessageBytes[ANT_TICK_MSG_EVENT_CODE_INDEX];
        au8TickMessage[6] = HexToASCIICharUpper(u8LastState);
        DebugPrintf(au8TickMessage);

        switch (u8LastState){
          // Handle good response code that can appear when other ant commands are sent
          case RESPONSE_NO_ERROR:
          {
            break;
          }

          // If we are paired but missing messages, blue blinks
          case EVENT_RX_FAIL:
          {
            LedOff(GREEN0);
            LedBlink(BLUE0, LED_2HZ);
            break;
          }

          // If we drop to search led is green
          case EVENT_RX_FAIL_GO_TO_SEARCH:
          {
            LedOff(BLUE0);
            LedOn(GREEN0);
            break;
          }

          // If the search times out the channel should automatically close.
          case EVENT_RX_SEARCH_TIMEOUT:
          {
            DebugPrintf("Search Timeout\r\n");
            break;
          }

          default:
          {
            DebugPrintf("Unexpected Event\r\n");
            break;
          }
        }
      }
    }
  }

  /*
  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      // We got some data! Convert it to displayable ASCII characters 
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        au8DataContent[2 *i] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] / 16);
        au8DataContent[(2 * i) + 1] = HexToASCIICharUpper(G_au8AntApiCurrentMessageBytes[i] % 16);
      }

      // Write the board string in the middle of last row 
      sStringLocation.u16PixelColumnAddress =  
        U16_LCD_CENTER_COLUMN - ( strlen((char const*)au8DataContent) * (U8_LCD_SMALL_FONT_COLUMNS + U8_LCD_SMALL_FONT_SPACE) / 2 );
      sStringLocation.u16PixelRowAddress = U8_LCD_SMALL_FONT_LINE7;
      LcdClearPixels(&G_sLcdClearLine7);
      LcdLoadString(au8DataContent, LCD_FONT_SMALL, &sStringLocation);
    }
    else if (G_eAntApiCurrentMessageClass == ANT_TICK)
    {
      // Check the buttons and update corresponding slot in au8TestMessage 
      au8TestMessage[0] = 0x00;
      au8TestMessage[1] = 0x00;
      au8TestMessage[2] = 0x00;
      au8TestMessage[3] = 0x00;

      if( IsButtonPressed(BUTTON0) )
      {
        au8TestMessage[0] = 0xff;
      }
      
      if( IsButtonPressed(BUTTON1) )
      {
        au8TestMessage[1] = 0xff;
      }

      // A channel period has gone by: typically this is when new data should be queued to send
      au8TestMessage[7]++;
      if(au8TestMessage[7] == 0)
      {
        au8TestMessage[6]++;
        if(au8TestMessage[6] == 0)
        {
          au8TestMessage[5]++;
        }
      }
      AntQueueBroadcastMessage(U8_ANT_CHANNEL_USERAPP, au8TestMessage);
    } // end ANT_TICK 
  } // end AntReadAppMessageBuffer() 

  */

} /* end UserApp1SM_ChannelOpen() */

static void UserApp1SM_WaitChannelClose(){
  if(AntRadioStatusChannel(U8_ANT_CHANNEL_USERAPP) == ANT_CLOSED){
    LedOff(GREEN0);
    LedOn(BLUE1);

    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }

  if (IsTimeUp(&UserApp1_u32Timeout, 500)){
    LedOff(GREEN0);
    LedBlink(RED0, LED_4HZ);

    UserApp1_pfStateMachine = UserApp1SM_Error;
  }
}

/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{
  if (WasButtonPressed(BUTTON0)){
    ButtonAcknowledge(BUTTON0);

    // Queue open channel and change LED0 from yellow to blinking green to indicate channel is open
    AntOpenChannelNumber(U8_ANT_CHANNEL_USERAPP);

    LedBlink(GREEN0, LED_2HZ);

    // Set timer and advance states
    UserApp1_u32Timeout = G_u32SystemTime1ms;
    UserApp1_pfStateMachine = UserApp1SM_WaitChannelOpen;
  }

  LedOff(GREEN0);
  LedOff(RED0);
  LedOn(BLUE1);
} /* end UserApp1SM_Idle() */     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */




/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
