#include <LiquidCrystal.h>
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define STATE_CONFIG 0
#define STATE_PASS 1
#define LOCK_ABIERTO 1
#define LOCK_CERRADO 0
#define CORRECTO 1
#define INCORRECTO 0
#define CHAR_OK byte(0)
#define CHAR_WRONG byte(1)
#define CHAR_CLOSED byte(2)
#define CHAR_OPEN byte(3)
#define CHAR_IZQ byte(4)
#define CHAR_DER byte(5)
#define CHAR_ABAJO byte(6)
#define CHAR_ARRIBA byte(7)


LiquidCrystal lcd(4,5,6,7,8,9);


// Variables
int adc_key_in  = 0;
int botonAnterior=btnNONE;
int boton;
int index=0;
int vectorPass[4]={btnLEFT, btnLEFT, btnRIGHT, btnRIGHT}; //Contrasenia
int vectorNuevo[4];
int correcto=CORRECTO;
int candado=LOCK_CERRADO;
int state=STATE_PASS;
long unsigned int tActual;


//----------- Caracteres Especiales ------------
byte ok[] = {
  B00000,
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B00000
};

byte wrong[] = {
  B00000,
  B00000,
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B00000
};


byte closed[] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B11111,
  B11011,
  B11011,
  B11111
};

byte open[] = {
  B01110,
  B10001,
  B10000,
  B10000,
  B11111,
  B11011,
  B11011,
  B11111
};

byte izq[] = {
    B00000,
  B00000,
  B00110,
  B01100,
  B11000,
  B01100,
  B00110,
  B00000
};
  
byte der[] = {
 B00000,
  B00000,
  B01100,
  B00110,
  B00011,
  B00110,
  B01100,
  B00000
};


byte abajo[] = {
  B00000,
  B00000,
  B00000,
  B10001,
  B11011,
  B01110,
  B00100,
  B00000
};


byte arriba[] = {
  B00000,
  B00100,
  B01110,
  B11011,
  B10001,
  B00000,
  B00000,
  B00000
};





//Funcion que lee los botones
int read_LCD_buttons()  
{ 
 adc_key_in = analogRead(A0);
 if (adc_key_in > 900) return btnNONE;
 if (adc_key_in < 50)   return btnRIGHT; 
 if (adc_key_in < 250)  return btnUP;
 if (adc_key_in < 450)  return btnDOWN;
 if (adc_key_in < 650)  return btnLEFT;
 if (adc_key_in < 850)  return btnSELECT; 
 return btnNONE;
} 



//Funcion que carga la pantalla de inicio
void pantallaInicio()
{
  lcd.begin(16,2);
  lcd.print("Ingrese codigo: ");
  lcd.setCursor(0, 1);
  lcd.print("____");
  lcd.setCursor(8, 1);
  lcd.write(CHAR_CLOSED);
}



// Funcion que chequea que el codigo ingresado sea igual a la pass
void checkCode()
{
  
  if(index==4)
       {
         for (int i = 0;  i < 4; i++)
			{
 			if( vectorPass[i] != vectorNuevo[i] ) //Si el codigo ingresado es incorrecto, vuelvo a cargar la pantalla de inicio
  				{
                  lcd.setCursor(6, 1);
                  lcd.write(CHAR_WRONG);
                  delay(1000);
                  pantallaInicio(); //Vuelve a cargar la pantalla de inicio
              	  correcto=INCORRECTO;
  				  break;
 				 }
			}
         	if(correcto==CORRECTO) // Si el codigo es correcto,prendo el led 10 segundos
            {
              tActual = millis();
              
              lcd.setCursor(8, 1);
              lcd.write(CHAR_OPEN); // candado abierto
              
              lcd.setCursor(6, 1);
              lcd.write(CHAR_OK); // signo ok
              
              digitalWrite(A5, 1); // se prende el led
              
              candado = LOCK_ABIERTO;
              
              lcd.setCursor(0, 0);
              lcd.print("Select to change"); // Imprimo el mensaje que indica que con Select puede guardar una pass nueva
            
            }
		index=0;
       }
  
  correcto=CORRECTO;
}



// Funcion del estado Pass
void pass()
{
  switch(boton){
    case btnRIGHT:
     if(botonAnterior != boton)
     {
   	   lcd.setCursor(index, 1);
       lcd.write(CHAR_DER);
       vectorNuevo[index]=btnRIGHT;
       index++;
       botonAnterior = boton;
     }
    break;
    
    case btnLEFT:
    if(botonAnterior != boton)
     {
     lcd.setCursor(index, 1);
       lcd.write(CHAR_IZQ);
       vectorNuevo[index]=btnLEFT;
       index++;
       botonAnterior = boton;
    }
    break;
    
    case btnUP:
   if(botonAnterior != boton)
     {
     lcd.setCursor(index, 1);
       lcd.write(CHAR_ARRIBA);
       vectorNuevo[index]=btnUP;
       index++;
       botonAnterior = boton;
    }
    break;
    
    case btnDOWN:
    if(botonAnterior != boton)
     {
     
      lcd.setCursor(index, 1);
       lcd.write(CHAR_ABAJO);
       vectorNuevo[index]=btnDOWN;
       index++;
       botonAnterior = boton;
     }
    break;
    
    case btnSELECT:
    if(candado==1 && botonAnterior !=boton)
    {
      botonAnterior = boton;
      lcd.setCursor(0,1);
      lcd.print("____");
      state = STATE_CONFIG; // Al presionar Select (habiendo ingresado el codigo correcto) ingreso al estado CONFIG
    }
    break;
    case btnNONE:
     botonAnterior = boton;
    break;
  }
    checkCode(); // Llama a la funcion que chequea la pass
  
  	if(candado==LOCK_ABIERTO)
 	{
   	 if(millis() > tActual +10000) //Si el candado estuvo abierto mas de 10 segundos se cierra
  	  {
   	      candado=LOCK_CERRADO;
    	  digitalWrite(A5, 0); //Se apaga el led
    	  pantallaInicio();
   		 }
  	  }
}


// Funcion del estado CONFIG, esta funcion pide y almacena en el array la nueva pass
void config()
{
  lcd.setCursor(0,0);
  lcd.print("Nuevo codigo:   ");
  lcd.setCursor(6,1);
  lcd.print(" ");
  
  
  switch(boton){
    case btnRIGHT:
     if(botonAnterior != boton)
     {
   	   lcd.setCursor(index, 1);
       lcd.write(CHAR_DER);
       vectorPass[index]=btnRIGHT;
       index++;
       botonAnterior = boton;
     }
    break;
    
    case btnLEFT:
    if(botonAnterior != boton)
     {
     lcd.setCursor(index, 1);
       lcd.write(CHAR_IZQ);
       vectorPass[index]=btnLEFT;
       index++;
       botonAnterior = boton;
    }
    break;
    
    case btnUP:
   	if(botonAnterior != boton)
     {
       lcd.setCursor(index, 1);
       lcd.write(CHAR_ARRIBA);
       vectorPass[index]=btnUP;
       index++;
       botonAnterior = boton;
      }
     break;
    
    case btnDOWN:
    if(botonAnterior != boton)
     {
     
      lcd.setCursor(index, 1);
       lcd.write(CHAR_ABAJO);
       vectorPass[index]=btnDOWN;
       index++;
       botonAnterior = boton;
     }
    break;
    case btnNONE:
     botonAnterior = boton;
    break;
  }
  
  if(index==4) //si cargue los 4 signos
  {
    index=0;
    lcd.setCursor(8, 1);
    lcd.write(CHAR_CLOSED);
    lcd.setCursor(6, 1);
    lcd.write(CHAR_OK);
    delay(2000); // muestro que se guardo correctamente durante 2 segundos
    pantallaInicio(); // vuelvo a cargar la pantalla de inicio
    digitalWrite(A5, 0); // Apago led si estaba encendido ya que se cierra el candado
    state = STATE_PASS; // Una vez ingresados los 4 botones vuelvo al estado PASS
    
  }
    
}




void setup()
{
  
  pinMode(A5, OUTPUT);
  
  // ---------- Creo los caracteres especiales --------
  lcd.createChar(0, ok);
  lcd.createChar(1, wrong);
  lcd.createChar(2, closed);
  lcd.createChar(3, open);
  lcd.createChar(4, izq);
  lcd.createChar(5, der);
  lcd.createChar(6, abajo);
  lcd.createChar(7, arriba);
  
  pantallaInicio(); //Cargo pantalla
   
  
}



void loop()
{
  boton = read_LCD_buttons();
  
if(state == STATE_PASS)
	pass();
if(state== STATE_CONFIG)
  config();
    
  
}
