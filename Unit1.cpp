//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
#include <IdHashMessageDigest.hpp>
#include <System.JSON.hpp>
#include <Data.DBXJSON.hpp>
#include <memory>
#include <system.hpp>
#include <Shellapi.hpp>
#include <IniFiles.hpp>
#include <Registry.hpp>
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
UnicodeString Redirect = "", secret = "", access_token ="";
TIniFile *ini = new TIniFile(GetCurrentDir()+"save.ini");
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
Image1->Visible = false;
Label4->Visible = false;
Label5->Visible = false;
IdHTTP1->HandleRedirects = true;
IdHTTP1->Request->Connection = "Keep-Alive";
IdHTTP1->Request->Host = "oauth.vk.com";
IdHTTP1->Request->UserAgent = "VKAndroidApp/4.4.2-982 (Android 6.0.1; SDK 23; armeabi-v7a; samsung SM-G930F; ru)";
UnicodeString answer_url= "";
UnicodeString request_url = "";
if (access_token == "" || secret == "")
{
	request_url = "https://oauth.vk.com:443/token?scope=nohttps%2Call&client_id=2274003&client_secret=hHbZxrka2uZ6jB1inYsH&2fa_supported=1&lang=ru&grant_type=password&username="+Edit1->Text+"&password="+Edit2->Text+"&libverify_support=1";
	answer_url = IdHTTP1->Get(request_url);
    TJSONObject *JSON = new TJSONObject;
	JSON = (TJSONObject*)TJSONObject::ParseJSONValue(answer_url);
	access_token = JSON->Get("access_token")->JsonValue->ToString();
	secret = JSON->Get("secret")->JsonValue->ToString();
	access_token.Delete(1,1);
	access_token.Delete(access_token.Length(),1);
	secret.Delete(1,1);
	secret.Delete(secret.Length(),1);
	ini->WriteString("access_tokens", "access_token", access_token) ;
	ini->WriteString("secrets", "secret", secret) ;
	ini->Free();
if (access_token == "" || secret =="") {
	Edit1->Enabled = true;
	Edit2->Enabled = true;
} else {
	Edit1->Enabled = false;
	Edit2->Enabled = false;
}
}

TStringList* data = new TStringList();
data->Clear();
data->Add("v=5.55");
data->Add("lang=ru");
data->Add("https=1");
data->Add("contacts="+Edit3->Text);
data->Add("service=phone");
data->Add("fields=photo_100,photo_50,photo_200,common_count");
data->Add("access_token="+access_token);
TIdHashMessageDigest5* md5 = new TIdHashMessageDigest5();
UnicodeString sig = md5->HashStringAsHex("\/method\/account.lookupContacts?v=5.55&lang=ru&https=1&contacts="+Edit3->Text+"&service=phone&fields=photo_100,photo_50,photo_200,common_count&access_token="+access_token+secret);
delete md5;
data->Add("sig="+sig.LowerCase());
request_url = "https://api.vk.com:443/method/account.lookupContacts";
answer_url = IdHTTP1->Post(request_url, data);
Memo1->Lines->Add(answer_url);
/*
std::auto_ptr<TJSONObject> object(static_cast<TJSONObject*>(TJSONObject::ParseJSONValue(answer_url)));
TJSONObject* response = static_cast<TJSONObject*>(object->Get("response")->JsonValue);
TJSONArray* items = static_cast<TJSONArray*>(response->Get("found")->JsonValue);
TJSONObject* id = static_cast<TJSONObject*>(items->Get(0));

UnicodeString find_id = id->GetValue("id")->ToString();
UnicodeString first_name = id->GetValue("first_name")->ToString();
UnicodeString last_name = id->GetValue("last_name")->ToString();
UnicodeString photo = id->GetValue("photo_200")->ToString();
//UnicodeString contact = id->GetValue("contact")->ToString();
Memo1->Lines->Add(find_id+" | "+first_name+" | "+last_name+" | "+photo);
TMemoryStream* photo_stream = new TMemoryStream;
photo.Delete(1,1);
photo.Delete(photo.Length(),1);
IdHTTP1->Get(photo, photo_stream);
photo_stream->Position = 0;
Image1->Bitmap->LoadFromStream(photo_stream);
delete photo_stream;
Redirect = L"https://vk.com/id"+find_id;
Image1->Visible=true;
Label4->Text = first_name;
Label4->Visible = true;
Label5->Text = last_name;
Label5->Visible = true;
*/
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1Click(TObject *Sender)
{
ShellExecuteW(NULL, L"open", Redirect.t_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Timer1->Interval = 3000;
	Timer1->Enabled = true;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	Form2->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	AniIndicator1->Enabled = true;
	Label6->Visible = true;
	Label6->Text = "Reading saved accounts...";
	try
	{
		access_token = ini->ReadString("access_tokens","access_token", "");
		secret = ini->ReadString("secrets","secret", "");
	}catch (...)
	{
		ShowMessage ("Saving accounts are not read!");
	}
	AniIndicator1->Enabled = false;
	Label6->Visible = false;
	if (access_token == "" || secret =="") {
		Edit1->Enabled = true;
		Edit2->Enabled = true;
	} else {
		Edit1->Enabled = false;
		Edit2->Enabled = false;
	}
	/*	try
	{
		TRegistry* registry_key = new TRegistry();
		registry_key->RootKey = HKEY_CURRENT_USER;
		registry_key->OpenKey("VK_Utilities", true);
	} catch (...)
	{
		ShowMessage (L"�� �������� ������ � ��������, ��������� ��������� �� ����� ��������������");
	} */
}
//---------------------------------------------------------------------------

