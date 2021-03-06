/*~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=
**
**    FILE:       BUTTON.CPP
**    DATE:       5/12/98
**    PROJ:       NT5
**    PROG:       BLJ
**    COMMENTS:   
**
**    DESCRIPTION: Window class custom buttons
**                    
**    HISTORY:
**    DATE        WHO            WHAT
**    ----        ---            ----
**    5/12/98     a-brycej     Wrote it.
**    
**
** Copyright (C) Microsoft 1998.  All Rights Reserved.
**
**~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=~=*/

#include "resrc1.h"

#include "cplsvr1.h"
extern HINSTANCE ghInst;
extern CDIGameCntrlPropSheet_X *pdiCpl;

// Colour of text for buttons!
#define TEXT_COLOUR  RGB(202,202,202)

HICON hIconArray[2];

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//    FUNCTION :  ButtonWndProc
//    REMARKS  :  The callback function for the CustomButton Window.
//                    
//    PARAMS   :  The usual callback funcs for message handling
//
//    RETURNS  :  LRESULT - Depends on the message
//    CALLS    :  
//    NOTES    :
//                

LRESULT CALLBACK ButtonWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch( iMsg )
    {
        case WM_PAINT:
            {
                PAINTSTRUCT *pps = new (PAINTSTRUCT);
                assert (pps);

                HDC hDC = BeginPaint(hWnd, pps);

                // Draw the appropriate icon                                                                                       
                DrawIconEx(hDC, 0, 0, hIconArray[GetWindowLong(hWnd, GWLP_USERDATA)], 0, 0, 0, NULL, DI_NORMAL);

                // Prepare the DC for the text
                SetBkMode   (hDC, TRANSPARENT);
                SetTextColor(hDC, TEXT_COLOUR);

                // Enforce the proper size!
                pps->rcPaint.top    = pps->rcPaint.left   = 0;
                pps->rcPaint.bottom = 33;
                pps->rcPaint.right  = 30;

              #define MAX_BUTTON_DIGITS  3
                TCHAR tsz[MAX_BUTTON_DIGITS+1]; //Maximum button number can be 999, more than enough.

                // Draw the Number                        
                DrawText (hDC, (LPCTSTR)tsz, GetWindowText(hWnd, tsz, MAX_BUTTON_DIGITS+1), &pps->rcPaint, DT_VCENTER|DT_CENTER|DT_NOPREFIX|DT_SINGLELINE|DT_NOCLIP);
                SetBkMode(hDC, OPAQUE);
                EndPaint (hWnd, pps);
             #undef MAX_BUTTON_DIGITS

                if( pps ) {
                    delete (pps);
                }
            }
            return(FALSE);

        default:
            return(DefWindowProc(hWnd, iMsg,wParam, lParam));
    }
    return(FALSE);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//    FUNCTION :  RegisterCustomButtonClass
//    REMARKS  :  Registers the Custom Button control window.
//                    
//    PARAMS   :  hInstance - Used for the call to RegisterClassEx
//
//    RETURNS  :  TRUE - if successfully registered
//                FALSE - failed to register
//    CALLS    :  RegisterClassEx
//    NOTES    :
//

extern ATOM RegisterCustomButtonClass()
{
    WNDCLASSEX CustCtrlClass;

    ZeroMemory(&CustCtrlClass, sizeof(WNDCLASSEX));

    CustCtrlClass.cbSize        = sizeof(WNDCLASSEX);
    CustCtrlClass.style         = CS_CLASSDC; 
    CustCtrlClass.lpfnWndProc   = ButtonWndProc;
    CustCtrlClass.hInstance     = ghInst;
    CustCtrlClass.lpszClassName = TEXT("TESTBUTTON");

    return(RegisterClassEx( &CustCtrlClass ));
}


