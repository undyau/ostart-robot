; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCountdownDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Countdown.h"

ClassCount=5
Class1=CCountdownApp
Class2=CCountdownDlg
Class3=CAboutDlg

ResourceCount=8
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_COUNTDOWN_DIALOG
Resource4=IDD_DIALOG_MAINTAIN
Resource5=IDD_DIALOG_RECORDTIMES
Class4=CRecordTimesDlg
Resource6=IDD_COUNTDOWN_DIALOG (English (U.S.))
Class5=CMaintenanceDlg
Resource7=IDD_ABOUTBOX (English (U.S.))
Resource8=IDR_MAIN_MENU

[CLS:CCountdownApp]
Type=0
HeaderFile=Countdown.h
ImplementationFile=Countdown.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CCountdownApp

[CLS:CCountdownDlg]
Type=0
HeaderFile=CountdownDlg.h
ImplementationFile=CountdownDlg.cpp
Filter=D
LastObject=ID_CANCEL_BTN
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=CountdownDlg.h
ImplementationFile=CountdownDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg


[DLG:IDD_COUNTDOWN_DIALOG]
Type=1
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Class=CCountdownDlg

[DLG:IDD_COUNTDOWN_DIALOG (English (U.S.))]
Type=1
Class=CCountdownDlg
ControlCount=50
Control1=IDC_EDIT_FIRST_HH,edit,1350639746
Control2=IDC_EDIT_FIRST_MM,edit,1350639746
Control3=IDC_EDIT_LAST_HH,edit,1350639746
Control4=IDC_EDIT_LAST_MM,edit,1350639746
Control5=IDC_EDIT_FREQ_MM,edit,1350639746
Control6=IDC_EDIT_FREQ_SS,edit,1350639746
Control7=IDC_EDIT_FILENAME,edit,1350631552
Control8=IDC_BUTTON_BROWSE,button,1342242816
Control9=IDC_LIST_MESSAGES,SysListView32,1350631497
Control10=IDC_BUTTON_DELETE,button,1342242816
Control11=IDC_BUTTON_MODIFY,button,1342242816
Control12=IDC_BUTTON_ADDAFTER,button,1342242816
Control13=IDC_BUTTON_CREATE,button,1342242816
Control14=IDC_BUTTON_PLAYREC,button,1342242816
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287
Control17=IDC_RADIO_CONTENT_MSG,button,1342373897
Control18=IDC_COMBO_CONTENT_MSG,combobox,1344339971
Control19=IDC_RADIO_CONTENT_TIME,button,1342242825
Control20=IDC_EDIT_NEXT_PLUS_MM,edit,1350631554
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,button,1342177287
Control23=IDC_RADIO_FREQ_EVERY,button,1342373897
Control24=IDC_COMBO_RELATIVE,combobox,1344339971
Control25=IDC_EDIT_TIMING_FINISH_MM,edit,1350639746
Control26=IDC_EDIT_TIMING_FINISH_SS,edit,1350639746
Control27=IDC_EDIT_TIMING_FINISH_TT,edit,1350639746
Control28=IDC_RADIO_FREQ_AFTER_MSG,button,1342242825
Control29=IDC_COMBO_FREQ_MSG,combobox,1344340227
Control30=IDC_BUTTON_ADD,button,1342242817
Control31=ID_CANCEL_BTN,button,1342242816
Control32=IDC_STATIC,button,1342177287
Control33=IDC_DYN_LED,button,1342177287
Control34=IDC_COMBO_RECORD_MSG,combobox,1344340226
Control35=IDC_BUTTON_RECORD,button,1342242816
Control36=IDC_BUTTON_PLAY,button,1342242816
Control37=IDC_BUTTON_AMPLIFY,button,1342242816
Control38=IDC_BUTTON_DAMPEN,button,1342242816
Control39=IDC_STATIC,button,1342177287
Control40=IDC_STATIC,static,1342308352
Control41=IDC_STATIC,static,1342308352
Control42=IDC_STATIC,static,1342308352
Control43=IDC_STATIC_PUNC1,static,1342308352
Control44=IDC_STATIC_PUNC2,static,1342308352
Control45=IDC_STATIC,static,1342308352
Control46=IDC_STATIC_PUNC5,static,1342308352
Control47=IDC_STATIC,static,1342308352
Control48=IDC_STATIC,static,1342308352
Control49=IDC_STATIC_FREQ_SECONDS,static,1342308352
Control50=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDOK,button,1342373889
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC_GNU,static,1342308352
Control7=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_RECORDTIMES]
Type=1
Class=CRecordTimesDlg
ControlCount=7
Control1=IDC_STATIC_NUMBER,static,1350696961
Control2=IDC_DYN_LED,button,1342177287
Control3=IDC_SPIN_NUMBER,msctls_updown32,1342177376
Control4=IDC_BUTTON_PLAY,button,1342242816
Control5=IDC_BUTTON_RECORD,button,1342242816
Control6=IDC_CHECK_SEQUENTIAL,button,1342242819
Control7=IDCANCEL,button,1342242817

[CLS:CRecordTimesDlg]
Type=0
HeaderFile=RecordTimesDlg.h
ImplementationFile=RecordTimesDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CRecordTimesDlg

[DLG:IDD_DIALOG_MAINTAIN]
Type=1
Class=CMaintenanceDlg
ControlCount=6
Control1=ID_BTN_DELETE,button,1342242816
Control2=IDCANCEL,button,1342242817
Control3=IDC_LIST_RECS,SysListView32,1350631425
Control4=IDC_RADIO_MESSAGES,button,1342373897
Control5=IDC_RADIO_TIMES,button,1342242825
Control6=IDC_STATIC,button,1342177287

[CLS:CMaintenanceDlg]
Type=0
HeaderFile=MaintenanceDlg.h
ImplementationFile=MaintenanceDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CMaintenanceDlg

[MNU:IDR_MAIN_MENU]
Type=1
Class=?
Command1=ID_MENU_CREATE
Command2=ID_MENU_EXPORT
Command3=ID_MENU_IMPORT
Command4=ID_MENU_EXIT
Command5=ID_MENU_MAINTENANCE
Command6=ID_MENU_CLOCK
Command7=ID_MENU_HELP_CONTENTS
Command8=ID_MENU_ABOUT
CommandCount=8

