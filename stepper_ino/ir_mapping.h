//	buttonId, ir port code 
#define BUT_0 16753245 
#define BUT_1 16736925 
#define BUT_2 16769565
#define BUT_3 16720605 
#define BUT_4 16712445 
#define BUT_5 16761405
#define BUT_6 16769055 
#define BUT_7 16754775 
#define BUT_8 16748655
#define BUT_9 16738455 
#define BUT_10 16750695 
#define BUT_11 16756815
#define BUT_12 16724175 
#define BUT_13 16718055 
#define BUT_14 16743045
#define BUT_15 16716015 
#define BUT_16 16726215 
#define BUT_17 16734885
#define BUT_18 16728765 
#define BUT_19 16730805 
#define BUT_20 16732845

#define BUT(buttonId, command) case BUT_##buttonId: (command); blinkB(); break;

void onIRValue(int value)
{
 	switch(value)
    { 
    	// Button matrix 
        BUT(0,	 setMode(DirectMode)        )BUT(1, setMode(RotationTrackedMode)	)BUT(2, dock() )
        BUT(3,	 accHorizontal.backward()	)BUT(4, accHorizontal.forward()		)BUT(5, accHorizontal.stop()		)
        BUT(6,	 accRotation.backward()		)BUT(7, accRotation.forward()		)BUT(8, accRotation.stop()			)
        BUT(9,	 setSpeed(2, 5)				)BUT(10,trackedRotation.y() -= 5000	)BUT(11,trackedRotation.y() += 5000	)
        BUT(12,	 setSpeed(4, 5)				)BUT(13,setSpeed(8, 5)				)BUT(14,setSpeed(16, 5)				)
        BUT(15,	 setSpeed(32, 5)			)BUT(16,setSpeed(50, 5)				)BUT(17,setSpeed(70, 5)				)
        BUT(18,	 setSpeed(200, 4)			)BUT(19,setSpeed(500, 4)			)BUT(20,setSpeed(700, 4)			)
    }
}

/*
Button IDS
16753245 16736925 16769565
16720605 16712445 16761405
16769055 16754775 16748655
16738455 16750695 16756815
16724175 16718055 16743045
16716015 16726215 16734885
16728765 16730805 16732845
//repeat code 4294967295
*/
