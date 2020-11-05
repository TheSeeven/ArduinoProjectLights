# ArduinoProjectLights

This is a project for my custom desk I work on.
I gathered the codes of my remote control.
Feel free to use my code to test it for your implementation.

Logic of the program
PC off LIGHT off AMBIENTAL off   -> do nothing  		
PC off LIGHT off AMBIENTAL on    -> do nothing   					
PC off LIGHT on  AMBIENTAL off   -> setLight off		
PC off LIGHT on  AMBIENTAL on    -> setLight off			
PC on  LIGHT off AMBIENTAL off   -> setLight on			
PC on  LIGHT off AMBIENTAL on    -> do nothing							
PC on  LIGHT on  AMBIENTAL off   -> do nothing						
PC on  LIGHT on  AMBIENTAL on    -> setLight off			
