//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit2.h"
#include "Registry.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::FormShow(TObject *Sender)
{
	try
	{
		TRegistry* registry_key = new TRegistry();
		registry_key->RootKey = HKEY_CURRENT_USER;
		registry_key->OpenKey("SOFTWARE\\VK_Utilities\\Accounts", true);
	} catch (...)
	{
		ShowMessage (L"�� �������� ������ � ��������, ��������� ��������� �� ����� ��������������");
	}
}
//---------------------------------------------------------------------------
