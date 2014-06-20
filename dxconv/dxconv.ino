// OpenCM 9.04 as USB to Dynamixel converter

// Serial device defines for dxl bus
#define DXL_BUS_SERIAL1 1  //Dynamixel on Serial1(USART1)  <-OpenCM9.04
#define DXL_BUS_SERIAL2 2  //Dynamixel on Serial2(USART2)  <-LN101,BT210
#define DXL_BUS_SERIAL3 3  //Dynamixel on Serial3(USART3)  <-OpenCM 485EXP


void setup() {
  // using serial 1
  Serial.begin( 1000000 );
  
  // init USB
  // check if USB is ready
  if( !SerialUSB.isConnected() ) {
    pinMode( BOARD_LED_PIN, OUTPUT );

    while( 1 ) {
      digitalWrite( BOARD_LED_PIN, 1 );
      delay( 100 );
    }
  }  
}

void loop() {
  int i, t;
  t = Serial.availableI()
  if( t > 0 )
    for( i=0; i<t; i++ )
      SerialUSB.write( Serial.read() )
      
  t = SerialUSB.available()
  if( t > 0 )
    for( i=0; i<t; i++ )
      Serial.write( SerialUSB.read() )
}

