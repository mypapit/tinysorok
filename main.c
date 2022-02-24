#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include "wrapper.h"
#include "resource.h"
HWND hMain;
char *byteArray;

int WINAPI DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow)
{
    InitCommonControls();

    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), 0, DlgProc,
            (LPARAM) lpCmdLine);

    return 0;
}




int WINAPI DlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rc;
	static HWND hwndTemp;
	static OPENFILENAME ofn;
    static char szFileName[MAX_PATH] = "";
	static int txtlength;
	int status1;

	FILE *fp;
	
	   
	hwndTemp = GetDlgItem(hWnd,IDC_EDITTXT);
    
	switch(uMsg)
    {
        case WM_INITDIALOG:
            {

               // hMain = hWnd;
				/* center dialog */
			
			GetWindowRect(hWnd, &rc);
			SetWindowPos(hWnd, NULL, (  ( GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left) ) / 2  ), (  ( GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top) ) / 2  ), 0, 0, SWP_NOSIZE | SWP_NOACTIVATE);

                SendMessage(hWnd, WM_SETICON, ICON_SMALL,
                        (LPARAM) LoadIcon(GetModuleHandle(0),
                                          MAKEINTRESOURCE(IDI_ICON1)));



            }
			break;
        case WM_CLOSE:
			PostQuitMessage(0);
            
            return TRUE;
        case WM_ACTIVATE:
            {
            
            }
			break;
        case WM_DROPFILES:
            {
            
                return TRUE;
            }
			break;


        case WM_COMMAND:
            {
				switch (LOWORD(wParam)){

					case IDC_OK:
						PostQuitMessage(0);
						break;

					case IDC_BROWSE:
						ZeroMemory(&ofn, sizeof(ofn));
					    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
					    ofn.hwndOwner = hWnd;
						ofn.lpstrFilter = "BMP Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
						ofn.lpstrFile = szFileName;
						ofn.nMaxFile = MAX_PATH;
						ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
						ofn.lpstrDefExt = "bmp";

						if(GetOpenFileName(&ofn)){

							SetDlgItemText(hWnd,IDC_EDITBMP,szFileName);
						// Do something usefull with the filename stored in szFileName 
						}

						return 1;
						

					case IDC_SOROK:
						{

							GetDlgItemText(hWnd,IDC_EDITBMP,szFileName,255);
							if (strlen(szFileName)<2){
								MessageBox(hWnd,"Uik? Mana file","RALAT",MB_ICONEXCLAMATION);
								break;
							}

							status1=MessageBox(hWnd,"Sorokkan Mesej?","Pertanyaan",MB_ICONINFORMATION|MB_YESNO);
							if ( status1!=IDYES)
								break;


							
							/*hard-coded limit bagi tiny sorok, 32k write, 288k read*/
							byteArray = (char *) malloc(WRAPPER_SOROK_BUFFER);

							txtlength=GetDlgItemText(hWnd,IDC_EDITTXT,byteArray,WRAPPER_SOROK_WRITE);
							
							if (txtlength<1){ /*kalau takder mesej nak disorokkan, minta kepastian..*/
								status1=MessageBox(hWnd,"Anda tidak memasukkan sebarang mesej, teruskan proses sorok?","Pertanyaan",MB_ICONINFORMATION|MB_YESNO);
								if ( status1!=IDYES)
									break;
							}

							status1=wrapper_sorok(szFileName,byteArray, txtlength+1);

							if (status1==0)
								MessageBox(hWnd,"Mesej berjaya di sorok!","Makluman",MB_ICONINFORMATION);
							else
								MessageBox(hWnd,wrapper_error(status1),"Makluman",MB_ICONEXCLAMATION);

							free(byteArray);
						} /*IDC_SOROK;*/
					break;
				
				case IDC_UNSOROK:
					{
						GetDlgItemText(hWnd,IDC_EDITBMP,szFileName,255);
						if (strlen(szFileName)<2){
							MessageBox(hWnd,"Uik? Mana file","RALAT",MB_ICONEXCLAMATION);
							break;
						}

						byteArray = (char *) malloc(WRAPPER_SOROK_BUFFER);

						status1=wrapper_unsorok(szFileName,byteArray,&txtlength);
						if (status1==0)
							MessageBox(hWnd,"Mesej berjaya di keluarkan!","Makluman",MB_ICONINFORMATION);
						else{
							MessageBox(hWnd,wrapper_error(status1),"Makluman",MB_ICONEXCLAMATION);
							break;
						}

						SetDlgItemText(hWnd,IDC_EDITTXT,byteArray);
												
						SendDlgItemMessage(hWnd,IDC_EDITTXT,EM_SETREADONLY ,TRUE,0);
						free(byteArray);
					}
	
					break;

				case IDC_SIMPAN:
					{
						ZeroMemory(&ofn, sizeof(ofn));
					    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
					    ofn.hwndOwner = hWnd;
						ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
						ofn.lpstrFile = szFileName;
						ofn.nMaxFile = MAX_PATH;
						ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
						ofn.lpstrDefExt = "txt";

						if(GetSaveFileName(&ofn)){

							byteArray = (char *) malloc(WRAPPER_SOROK_BUFFER);
							txtlength=GetDlgItemText(hWnd,IDC_EDITTXT,byteArray,WRAPPER_SOROK_WRITE);
							fp=fopen(szFileName,"wb");
								
								if (!fp){
									MessageBox(hWnd,"Masalah menulis fail","Makluman",MB_ICONEXCLAMATION);
									break;
								}


							fwrite(byteArray,sizeof(char),txtlength,fp);
							fclose(fp);
							free(byteArray);
							
							MessageBox(hWnd,"Mesej ditulis kedalam fail","Makluman",MB_ICONINFORMATION);
							


						}





					}
					break;
                    
				case IDC_EDITBMP:
					{
						if (HIWORD(wParam) == EN_UPDATE){
							SendDlgItemMessage(hWnd,IDC_EDITTXT,EM_SETREADONLY,FALSE,0);
							SetDlgItemText(hWnd,IDC_EDITTXT,(char *)NULL);
						}
						

					}
					break; /*IDC_EDITBMP*/
			
				case IDC_PERIHAL:
					{
						MessageBox(hWnd,"TinySorok 0.05\nmypapit (papit58@yahoo.com)\nhttp://ppsplit.sourceforge.net\n\n\"Steganografi tatkala bosan\" - papit\n\nAwas\nIni merupakan perisian di peringkat alpha, guna atas risiko sendiri.\nPerisian ini tidak mengimplemenkan sebarang perlindungan terhadap \ndata yang di simpan dalam fail grafik. Sila baca fail \'README\' untuk\nmaklumat lanjut.\n\n","Perihal",MB_ICONINFORMATION);
					}
					break;
  
		


				} /*switch_loword*/

            
            }
			return 0;
			break;
    }
    return FALSE;
}