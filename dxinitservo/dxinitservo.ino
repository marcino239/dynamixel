// OpenCM 9.04 sketch to init servo and test it


/* Serial device defines for dxl bus */
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP

/* Control table defines */
#define ID_Change_Address 3
#define Goal_Postion_Address 30
#define Moving_Speed 32

#define LED_RATE 100000    // in microseconds; should give 0.5Hz toggles

HardwareTimer Timer(1);
Dynamixel Dxl( DXL_BUS_SERIAL1 );

#define BUFF_MAX 16
char buff[ BUFF_MAX ];

int newServoID;

char *readFromUSB( char *buff, int nToRead ) {
  if( SerialUSB.getDTR() || SerialUSB.getRTS() ) {
    while( SerialUSB.available() < nToRead )
      delay( 100 );
     
    for( int i=0; i<nToRead; i++ )
      buff[ i ] = SerialUSB.read();
  }

  buff[ nToRead ] = 0;

  return buff;
} 

void setup() {
  
  // Set up the LED to blink
  pinMode(BOARD_LED_PIN, OUTPUT);

  // Pause the timer while we're configuring it
  Timer.pause();

  // Set up period
  Timer.setPeriod(LED_RATE); // in microseconds

  // Set up an interrupt on channel 1
  Timer.setMode(TIMER_CH1, TIMER_OUTPUT_COMPARE);
  Timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
  Timer.attachInterrupt(TIMER_CH1, handler_led);

  // Refresh the timer's count, prescale, and overflow
  Timer.refresh();

  // Start the timer counting
  Timer.resume();  
  
  // check if USB is ready
  if( !SerialUSB.isConnected() ) {
    pinMode( BOARD_LED_PIN, OUTPUT );

    while( 1 ) {
      digitalWrite( BOARD_LED_PIN, 1 );
      delay( 100 );
    }
  }

  // Dynamixel 2.0 Baudrate -> 0: 9600, 1: 57600, 2: 115200, 3: 1Mbps 
  Dxl.begin(3);

  // check current servo id
  int servoID, i;
  for( i=1; i<20; i++ )  {
    servoID = Dxl.ping( i );
    if( servoID != 0xffff )
      break;
  }

  SerialUSB.println( "Found Servo: " + servoID );
  
  // change servo id
  SerialUSB.println( "Changing servo ID.  Enter new number: " );
  newServoID = atoi( readFromUSB( buff, 1 ) );
  SerialUSB.println( "Setting servo with ID: " + newServoID );

  /*************** CAUTION ***********************
   * All dynamixels in bus will be changed to ID 2 by using broadcast ID(0xFE)
   * Please check if there is only one dynamixel that you want to change ID
   ************************************************/
  Dxl.writeByte( BROADCAST_ID, ID_Change_Address, newServoID );    //Change current id to new id
  Dxl.jointMode( newServoID ); //jointMode() to use position mode
}

void loop() {
  int cmd;
  
  cmd = readFromUSB( buff, 1 )[0];
  switch( cmd ) {
    case 'L':
    case 'l':
      /*Turn dynamixel left */
      Dxl.writeWord( newServoID, Goal_Postion_Address, 300 );
      break;
      
    case 'R':
    case 'r':
      /*Turn dynamixel right */
      Dxl.writeWord( newServoID, Goal_Postion_Address, 800 );
      break;
      
    case 'C':
    case 'c':
      /*Turn dynamixel to center*/
      Dxl.writeWord( newServoID, Goal_Postion_Address, 512 );
      break;
  }
    
  delay(1000);              
}

void handler_led(void) {
  toggleLED();
}

