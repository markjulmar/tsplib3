========================================================================
       JULMAR TSP++ VERSION 3.0 CLASS LIBRARY : $$Root$$
========================================================================

TspWizard has created this $$Root$$ TAPI Service Provider for you.  
It contains all the options you requested for your service provider and serves
as the starting point for writing your TSP.

This file contains a summary of what you will find in each of the files that
make up your $$Root$$ TSP.

$$Root$$.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

$$Root$$.h
    This is the main header file for the TSP.  It includes other
    project specific headers (including Resource.h) and declares the
    required TSP++ objects.

$$Root$$.cpp
    This is the main application source file that contains the CServiceProvider
    override class $$SP_CLASS_NAME$$.

$$Root$$.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes a basic version resource which is required for
	all TAPI service provider. This file can be directly edited in Microsoft
	Visual C++.

$$Root$$.def
	This is the Windows Definition file which details all the exported 
	functions from the service provider.

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named $$Root$$.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

$$DEVICE_CPP_NAME$$
	This is the CTSPIDevice override class $$DEVICE_CLASS_NAME$$. This
	file contains all the generated device communication code.

$$IF(OVERRIDE_LINE)
$$LINE_CPP_NAME$$
	This is the CTSPILineConnection override class $$LINE_CLASS_NAME$$.
	This file contains the request map handler which details the call control
	requests which are processed by this service provider.
$$IF(OVERRIDE_ADDRESS)

$$ADDRESS_CPP_NAME$$
	This is the CTSPIAddressInfo override class $$ADDRESS_CLASS_NAME$$.
	This file contains the initialization overrides for a TSP++ address object.
$$ENDIF
$$IF(OVERRIDE_CALL)

$$CALL_CPP_NAME$$
	This is the CTSPICallAppearance override class $$CALL_CLASS_NAME$$.
	This file contains the initialization overrides for a TSP++ call object.
$$ENDIF
$$IF(OVERRIDE_CONFCALL)

$$CONFCALL_CPP_NAME$$
	This is the CTSPIConferenceCall override class $$CONFCALL_CLASS_NAME$$.
	This file contains the initialization overrides for a TSP++ conference object.
$$ENDIF

DropCall.cpp
	This file contains the code to process the TSPI_lineDrop request.

MakeCall.cpp
	This file contains the code to process the TSPI_lineMakeCall and TSPI_lineDial
	requests.
$$IF(LINEFEATURE_0)

Accept.cpp
	This file contains the code to process the TSPI_lineAccept request.
$$ENDIF
$$IF(LINEFEATURE_1)

Conference.cpp
	This file contains the code to manage conferences for the TSP. This includes
	the handlers for the TSPI_lineSetupConference, TSPI_linePrepareAddToConference,
	TSPI_lineAddToConference and possibly TSPI_lineRemoveFromConference.
$$ENDIF
$$IF(LINEFEATURE_2)

Answer.cpp
	This file contains the code to process the TSPI_lineAnswer request.
$$ENDIF
$$IF(LINEFEATURE_3 || LINEFEATURE_4)

Transfer.cpp
	This file contains the code to process transfer requests for the TSP. This
	includes TSPI_lineBlindTransfer, TSPI_lineSetupTransfer, and TSPI_lineCompleteTransfer.

$$ENDIF
$$IF(LINEFEATURE_25)

DevConfig.cpp
	This file contains the code to process the TSPI_lineGetDevConfig and 
	TSPI_lineSetDevConfig requests.
$$ENDIF
$$IF(LINEFEATURE_5)

Complete.cpp
	This file contains the code to manage call completions for the TSP. This includes
	the handlers for TSPI_lineCompleteCall and TSPI_lineUncompleteCall.
$$ENDIF
$$IF(LINEFEATURE_35)

Route.cpp
	This file contains the code to process the TSPI_lineRedirect request.
$$ENDIF
$$IF(LINEFEATURE_7)

Agent.cpp
	This file contains all the agent management code for the TSP. This includes
	the lineSetAgentGroup, lineSetAgentState and lineSetAgentActivity requests.
$$ENDIF
$$IF(LINEFEATURE_12 || LINEFEATURE_13)

Generate.cpp
	This file contains the code to process the TSPI_lineGenerateDigits and 
	TSPI_lineGenerateTone requests.
$$ENDIF
$$IF(LINEFEATURE_14)

Hold.cpp
	This file contains the code to process the TSPI_lineHold, TSPI_lineUnhold
	and possibly the TSPI_lineSwapHold functions.
$$ENDIF
$$IF(LINEFEATURE_21)

UUI.cpp
	This file contains the code to process the TSPI_lineSendUserUserInfo request.
$$ENDIF
$$IF(LINEFEATURE_18)

Park.cpp
	This file contains the code to process the park features of the hardware,
	specifically the TSPI_linePark and TSPI_lineUnpark requests.
$$ENDIF
$$IF(LINEFEATURE_19)

Pickup.cpp
	This file contains the code the process the TSPI_linePickup request.
$$ENDIF
$$IF(LINEFEATURE_9)

Forward.cpp
	This file contains the code to process the TSPI_lineForward request.
$$ENDIF
$$IF(LINEFEATURE_26)

QOS.cpp
	This file contains the code to process the TSPI_lineSetCallQualityOfService
	request.
$$ENDIF
$$IF(LINEFEATURE_27)

CallTreat.cpp
	This file contains the code to process the TSPI_lineSetCallTreatment request.
$$ENDIF
$$IF(LINEFEATURE_22)

MediaCtl.cpp
	This file contains the code to process the TSPI_lineSetMediaControl request.
$$ENDIF
$$IF(LINEFEATURE_20)

Secure.cpp
	This file contains the code to process the TSPI_lineSecureCall request.
$$ENDIF
$$IF(LINEFEATURE_23)

Terminal.cpp
	This file contains the code to process the TSPI_lineSetTerminal request.
$$ENDIF
$$ENDIF
$$IF(OVERRIDE_PHONE)

$$PHONE_CPP_NAME$$
	This file contains the CTSPIPhoneConnection override $$PHONE_CLASS_NAME$$
	which controls the phone buttons, display and lamps.
$$ENDIF
$$IF(PHONEFEATURE_4)
$$IF(BUTTON_CANWRITE || LAMP_CANWRITE)

BtnLamp.cpp
	This file contains the code to process the TSPI_phoneSetLampInfo and 
	TSPI_phoneSetButtonInfo requests.
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_1 || PHONEFEATURE_2)

PhnData.cpp
	This file contains the code to process the TSPI_phoneGetData and
	TSPI_phoneSetData requests.
$$ENDIF
$$IF(PHONEFEATURE_3)
$$IF(DISPLAY_CANWRITE)

Display.cpp
	This file contains the code to process the TSPI_phoneSetDisplay request.

$$ENDIF
$$ENDIF
$$// phoneSetVolume, phoneSetHookswitch, phoneSetGain
$$IF(PHONEFEATURE_5)
$$IF(GAIN_CANWRITE)

Gain.cpp
	This file contains the code to process the TSPI_phoneSetGain request.
$$ENDIF
$$IF(HOOKSWITCH_CANWRITE)

HSwitch.cpp
	This file contains the code to process the TSPI_phoneSetHookswitch request.
$$ENDIF
$$IF(VOLUME_CANWRITE)

Volume.cpp
	This file contains the code to process the TSPI_phoneSetVolume request.
$$ENDIF
$$ENDIF
$$IF(PHONEFEATURE_6)
$$IF(RINGER_CANWRITE)

Ringer.cpp
	This file contains the code to process the TSPI_phoneSetRing request.
$$ENDIF
$$ENDIF
$$IF(LINEFEATURE_8 || PHONEFEATURE_7)

DevSpecific.cpp
	This file contains the code to process device-specific extensions to the TSP.
	This includes the TSPI_lineDevSpecific, TSPI_lineDevSpecificFeature and
	TSPI_phoneDevSpecific requests.
$$ENDIF

Unsolicited.cpp
	This file contains the unsolicited event handler for the service provider.
$$IF(!HAS_UI)

INSTALL.cpp
	This file contains the code to process the TUISPI_providerInstall and
	TUISPI_providerRemove requests so that the provider may be installed onto
	a Windows system.
$$ENDIF

/////////////////////////////////////////////////////////////////////////////
Other notes:

TspWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

For several functions, AppWizard has included suggested code for you
to build upon. This code is simply a suggestion and in no way implies
that it is the only way to perform the required functionallity. You may
rewrite and customize any function in the created code.

Since the TSP has not been fleshed out with real code, it may not work
as expected right now or when modified. No warranty is given for the
generated code.

/////////////////////////////////////////////////////////////////////////////
