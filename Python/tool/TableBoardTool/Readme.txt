ui->py：
pyuic4 -o UiTestResultInfo.py UiTestResultInfo.ui

打exe包：
Control下执行：python pyinstaller.py -F Controller.py --hidden-import atexit

serial data：
px : 7f fe ff ff 07 ff ff ff ff
pg : ff ff ff ff 07 ff ff ff f1


name + "link error"

    pg  px
    1    1   | releaseStatIn        
    1    1   | touchStatIn          
    1    1   | encoderAIn           
    1    1   | encoderBIn           
    1    1   | serialTxIn           
    1    1   | elevHallAIn               
    1    1   | elevHallBIn          
    1    0   | footHomeIn           
                                    
    1    0   | footFuncIn           
    1    1   | relayCtrlIn          
    1    1   | petExoffIn           
    1    1   | stepPulseIn          
    1    1   | stepDirIn            
    1    1   | servoPulseIn         
    1    1   | servoDirIn           
    1    1   | stepClutchIn         
                                    
    1    1   | servoClutchIn        
    1    1   | hpotClkIn            
    1    1   | hpotNcsIn            
    1    1   | vpotClkIn            
    1    1   | vpotNcsIn            
    1    1   | elev1UppDownnIn      
    1    1   | elev1UpnDownpIn      
    1    1   | elev2UppDownnIn      
                                    
    1    1   | elev2UpnDownpIn      
    1    1   | encoderVccIn         
    1    1   | hpotVccIn            
    1    1   | vpotVccIn            
    1    1   | elev1VccIn           
    1    1   | elev2VccIn           
    1    1   | stepMotorEnIn        
    1    1   | servoDisableIn       
                                    
    1    1   | servoAlarmRstIn      
    1    1   | servoCwDisableIn     
    1    1   | servoCcwDisableIn    
    0    0   | reserved0            
                                    
    1    1   | motorPulseOut        
    1    1   | motorDirOut          
    1    1   | motorDisableOut      
    1    1   | clutchDisableOut     
    1    1   | boardRstOut          
    1    1   | elevUpOut            
    1    1   | elevDownOut          
    1    1   | stepReleaseOut       
                                    
    1    1   | stepTouchOut         
    1    1   | servoTouchOut        
    1    1   | stepEncoderAOut      
    1    1   | servoEncoderAOut     
    1    1   | stepEncoderBOut      
    1    1   | servoEncderBOut      
    1    1   | cradleInLimitOut     
    1    1   | cradleOutLimitOut    
                                    
    1    1   | cradleFixPosOut      
    1    1   | elevUpLimitOut       
    1    1   | elevDownLimitOut     
    1    1   | elev1HallAOut        
    1    1   | elev1HallBOut        
    1    1   | elev2HallAOut        
    1    1   | elev2HallBOut        
    1    1   | elev2OvcStat         
                                    
    1    1   | elev1OvcStat         
    0    1   | servoAlarmStatOut    
    0    1   | servoPrepareStatOut  
    0    1   | servoWorkStatOut     
    1    1   | serialRxOut          
    1    1   | hpotMisoOut          
    1    1   | vpotMisoOut          
    1    1   | framMemory           