#include "gl.h"
#include "ImGui.h"
#include "ImGuiGl.hpp"
#include <stdio.h> // for printf
#include "STRING.HPP"
#include "ARRAY.HPP"

#define XRES 640
#define YRES 480

class TraumSymbol {
public:
  String name;
  String global;
  Array<String> alternativeNamen;
  Array<String> sieheAuch;
  String sieheAuchString;
  String allgemein;
  String spirituell;
  String psychologisch;
  String psychisch;
  String frageStellung;
  String assoziation;
  String artemidoros;
  String achmet;
  String transzendenteBedeutung;
  String medizinRad;
  String weiblichkeit;
  Array<String> volkstuemlich;
  Array<String> volkstuemlich_europaeisch;
  Array<String> volkstuemlich_arabisch;
  Array<String> volkstuemlich_indisch;
  Array<String> volkstuemlich_persisch;
};

Array<TraumSymbol*> symbols;

#define FILENAMEDAT "DSYMBOLE.DAT"

void nextLine(char *arr, int &p, int s) {
  for (; p<s; p++) {
    if (arr[p]==0x0d||arr[p]==0x0a) {
      char c2 = p+1<s?arr[p+1]:0;
      if ((c2 != arr[p])&&((c2==0x0d)||(c2==0x0a))) p++;
      p++;
      return;
    }
  }
  return;
}

void importString(char *arr, int &p, int s, const char *varName, String &str) {
  bool found = true;
  int p2 = p+1;
  for (int i = 0; i < strlen(varName); i++) {
    if (p2 >= s) {found=false;break;}
    if (arr[p2]!=varName[i]) {found=false;break;}
    p2++;
  }
  if (found) {
    if (p2>=s) return;
    if (arr[p2]!=']') found = false;
    if (found) {
      int scanStart = -1;
      for (;p<s;p++) {
        if (arr[p]=='|' && scanStart<0)  scanStart = p; 
          else
        if (arr[p]=='|' && scanStart>=0)  break;
      }
      if (scanStart >= 0) {
        scanStart++;
        str.resize(p-scanStart);
        memcpy(&str[0],&arr[scanStart],str.size());
        nextLine(arr,p,s);
      }
    }
  }
  //fprintf(out,"[%s]|%s|\n",varName,str.c_str());
}

void importStringArray(char *arr, int &p, int s, const char *varName, Array<String> &arr2) {
  bool found = true;
  int p2 = p+1;
  for (int i = 0; i < strlen(varName); i++) {
    if (p2 >= s) {found=false;break;}
    if (arr[p2]!=varName[i]) {found=false;break;}
    p2++;
  }
  if (found) {
    if (p2>=s) return;
    if (arr[p2]!=']') found = false;
    if (found) {
      while(1) {
        int scanStart = -1;
        for (;p<s;p++) {
          if (arr[p]=='|' && scanStart<0)  scanStart = p; 
            else
          if (arr[p]=='|' && scanStart>=0)  break;
        }
        if (scanStart >= 0) {
          scanStart++;
          String str;
          str.resize(p-scanStart);
          memcpy(&str[0],&arr[scanStart],str.size());
          arr2.push_back(str);
          nextLine(arr,p,s);
          if (arr[p]!='|') break;
        }
      }
    }
  }
  //fprintf(out,"[%s]\n",varName);
  //for (int i = 0; i < arr.size(); i++) {
  //  fprintf(out,"|%s|\n",arr[i].c_str());
  //}
}

void importDAT() {

  FILE *in = fopen(FILENAMEDAT,"rb");
  fseek(in,0,SEEK_END);
  int fileSize = ftell(in);
  fseek(in,0,SEEK_SET);
  char *arr = (char*)malloc(fileSize);
  fread(arr,1,fileSize,in);
  fclose(in);

  int p = 0;
  nextLine(arr,p,fileSize);
  nextLine(arr,p,fileSize);
  int s = 0; sscanf(&arr[p],"[SYMBOLCOUNT:%d]",&s);  symbols.resize(s);
  nextLine(arr,p,fileSize);
  printf("Importing:%d Symbols\n",symbols.size());

  for (int i = 0; i < symbols.size(); i++) {
    symbols[i] = new TraumSymbol(); TraumSymbol *s = symbols[i];
    nextLine(arr,p,fileSize);
    importString(arr,p,fileSize,"NAME:",s->name);
    importStringArray(arr,p,fileSize,"ALTERNATIVENAME:",s->alternativeNamen);
    importStringArray(arr,p,fileSize,"SIEHEAUCH:",s->sieheAuch);
    importString(arr,p,fileSize,"SIEHEAUCHSTRING:",s->sieheAuchString);
    importString(arr,p,fileSize,"ALLGEMEIN:",s->allgemein);
    importString(arr,p,fileSize,"SPIRITUELL:",s->spirituell);
    importString(arr,p,fileSize,"PSYCHOLOGISCH:",s->psychologisch);
    importString(arr,p,fileSize,"PSYCHISCH:",s->psychisch);
    importString(arr,p,fileSize,"FRAGESTELLUNG:",s->frageStellung);
    importString(arr,p,fileSize,"ASSOZIATION:",s->assoziation);
    importString(arr,p,fileSize,"ARTEMIDOROS:",s->artemidoros);
    importString(arr,p,fileSize,"ACHMET:",s->achmet);
    importString(arr,p,fileSize,"TRANSZENDENTEBEDEUTUNG:",s->transzendenteBedeutung);
    importString(arr,p,fileSize,"MEDIZINRAD:",s->medizinRad);
    importString(arr,p,fileSize,"WEIBLICHKEIT:",s->weiblichkeit);
    importStringArray(arr,p,fileSize,"VOLKSTUEMLICH:",s->volkstuemlich);
    importStringArray(arr,p,fileSize,"VOLKSTUEMLICH_EUROPAEISCH:",s->volkstuemlich_europaeisch);
    importStringArray(arr,p,fileSize,"VOLKSTUEMLICH_ARABISCH:",s->volkstuemlich_arabisch);
    importStringArray(arr,p,fileSize,"VOLKSTUEMLICH_INDISCH:",s->volkstuemlich_indisch);
    importStringArray(arr,p,fileSize,"VOLKSTUEMLICH_PERSISCH:",s->volkstuemlich_persisch);
  }

  free(arr);
}


#define MINSYMBOL 2
int currentSymbol = MINSYMBOL;

void StringSection(const char *name, String &str) {
  if (!str.empty()) {
    ImGui::TextColored(ImVec4(0,1,1,1),"%s:",name);
    int len = str.size();
    int pos = 1024;
    char *s = str.c_str();
    while(len>0) {
      char a;
      if (pos < str.size()) {
        a = str[pos];
        str[pos]=0;
      }
      ImGui::TextWrapped("%s",s);
      if (pos < str.size()) {
        str[pos] = a;
      }
      len-=1024;
      pos+=1024;
      s+=1024;
    }
    ImGui::Separator();
  }
}

String k;
void StringArraySection(const char *name, Array<String> &arr) {
  if (!arr.empty()) {
    ImGui::TextColored(ImVec4(0,1,1,1),"%s:",name);
    k.resize(0);
    for (int i = 0; i < arr.size(); i++) {
      if (i != 0) k += "\n";
      k += arr[i];
    }
    ImGui::TextWrapped("%s",k.c_str());
    ImGui::Separator();
  }
}

#define SUCHWINDOWSIZE ImVec2(200,YRES)
#define SYMBOLWINDOWSIZE ImVec2(400,YRES)

#define SUCHTEXTLENGTH 500
char suchText[SUCHTEXTLENGTH]={0};
int triggered1 = 0;

void ShowSymbolWindow() {
  static bool first = true; if (first) {first = false; ImGui::SetNextWindowPos(ImVec2(240,0));}
  ImGui::Begin("Traum Symbol",NULL,SYMBOLWINDOWSIZE);
  if (currentSymbol>=MINSYMBOL&&currentSymbol<symbols.size()) {
    TraumSymbol *s = symbols[currentSymbol];

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddText(ImGui::GetWindowFont(), ImGui::GetWindowFontSize()*2.0f, pos, ImColor(128,0,255,255), s->name.c_str());
    ImGui::Dummy(ImVec2(0,ImGui::GetWindowFontSize()*2.0f));
    //ImGui::TextColored(ImVec4(0.5,0.0,1.0,1),"\"%s\"\n",s->name.c_str());
    ImGui::Separator();
    for (int i = 0; i < s->sieheAuch.size(); i++) {
      if (i != 0) ImGui::SameLine();
      if (ImGui::Button((s->sieheAuch[i]+"##BUTTONSIEHE"+String::fromInt(i)).c_str())) {
        String a = s->sieheAuch[i];
        memcpy(suchText,a.c_str(),a.length()+1);
        triggered1++;
      }
    }
    if (!s->sieheAuch.empty()) ImGui::Separator();
    StringSection("Siehe",s->sieheAuchString);
    StringSection("Allgemein",s->allgemein);
    StringSection("Spirituell",s->spirituell);
    StringSection("Psychologisch",s->psychologisch);
    StringSection("Psychisch",s->psychisch);
    StringSection("Fragestellung",s->frageStellung);
    StringSection("Assoziation",s->assoziation);
    StringSection("Artemidoros",s->artemidoros);
    StringSection("Achmet",s->achmet);
    StringSection("Transzendente Bedeutung",s->transzendenteBedeutung);
    StringSection("Medizinrad",s->medizinRad);
    StringSection("Weiblichkeit",s->weiblichkeit);
    StringArraySection("Volkstuemlich",s->volkstuemlich);
    StringArraySection("europaeisch",s->volkstuemlich_europaeisch);
    StringArraySection("arabisch",s->volkstuemlich_arabisch);
    StringArraySection("indisch",s->volkstuemlich_indisch);
    StringArraySection("persisch",s->volkstuemlich_persisch);
  }
  ImGui::End();
}

String similarWords;
String n1,n2;

bool wholeWord = true;
int triggeredY = -1;

void ShowSuchWindow() {
  static bool first = true; if (first) {first = false; ImGui::SetNextWindowPos(ImVec2(0,0));}
  ImGui::Begin("Suche",NULL,SUCHWINDOWSIZE);
  //ImGui::InputInt("SymbolNr",&currentSymbol);
  if (ImGui::InputText("Suchen",suchText,SUCHTEXTLENGTH)||(triggered1!=0)) {
    triggered1=0;
    int pressedOne = triggeredY/13;
    if (triggeredY<0) pressedOne=-1000;
    triggeredY = -1;
    similarWords.resize(0);
    n1 = suchText;
    n1 = toLower(n1);
    int maxSymbols = 64;
    int first = -1;
    int k = 0;
    for (int i = MINSYMBOL; i < symbols.size(); i++) {
      TraumSymbol *s = symbols[i];
      n2 = s->name;
      n2 = toLower(n2);
      bool found = n2.startsWith(n1);
      if (!wholeWord) found = n2.findFirst(n1) >= 0;
      if (found) {
        if (pressedOne==k) {memcpy(suchText,s->name.c_str(),s->name.length()+1);triggered1++;}
        if (first == -1) first = i;
        similarWords += s->name + "\n";
        maxSymbols--;
        if (maxSymbols<=0) break;
        k++;
      }
    }
    if (first!=-1) currentSymbol = first;
  }
  if (ImGui::Checkbox("Ganzes Wort",&wholeWord)) triggered1++;
  ImGui::BeginChild("AlphabetSub", ImVec2(40,400), true);
  for (int y = 'A'; y <= 'Z'; y++) {
    char buttonName[3];
    buttonName[2]=0;
    buttonName[1]=' ';
    buttonName[0]=y; 
    if (ImGui::Button(buttonName)) {
      memcpy(suchText,buttonName,1);
      suchText[1]=0;
      triggered1++;
    }
  }
  ImGui::EndChild();
  ImGui::SameLine();
  ImVec2 pos = ImGui::GetCursorScreenPos();
  ImGui::Text("%s",similarWords.c_str());
  if (ImGui::IsMouseClicked(1)) {
    triggered1++;
    triggeredY = ImGui::GetMousePos().y-pos.y;
  }
  ImGui::End();
  if (currentSymbol<MINSYMBOL) currentSymbol = MINSYMBOL;
  if (currentSymbol>=symbols.size()) currentSymbol = symbols.size()-1;
}
  
int main(int argc, const char *argv[]) {

  importDAT();

  const bool pureVGA = false;
  if (pureVGA) {
    glVGA();
  } else {
    if (!glVesa(XRES,YRES,16)) { // 16 bit is a lot faster than 32 bit on NVidia cards
      printf("No Vesa %dx%dx16 found.\n", XRES, YRES);
      if (!glVGA())
      {
        printf("No VGA device found.\n");
        exit(0);
      }
    }
  }

  InitImGui(glFrameBufferWidth, glFrameBufferHeight);

  bool quit = false;
  while(!quit) {

    if (currentKey == GL_VK_ESCAPE) quit = true;

    glClearColor(0.6,0.8,1.0,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    ImGuiNewFrame();

    ShowSymbolWindow();
    ShowSuchWindow();

    ImGui::Render();
      
    glRefresh();
  }

  glDone();
  return 0;
}
