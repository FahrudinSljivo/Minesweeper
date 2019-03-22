/*
Biblioteke iostream i stdlib.h su standardne biblioteke, a winbgim.h koristimo jer nam omogucuje graficki prikaz igrice
*/
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<winbgim.h>
#include<conio.h>

using namespace std;
/*
U poeetnom dijelu definišemo varijable i postavljamo vrijednosti konstanti. Vrijednostima maxdimX i maxdimY definišemo broj polja u igrici, a konstante su jer je obavezno da budu kada se definiše polje.
Dalje, imamo enumeracije koje imaju vrijednosti integera i koje ae oznaeavati odre?ene stadije u igrici. Onda, imamo 3 strukture: za taeku, pravougaonik i polje. Unutar strukture Tacka definisali smo integere x i y kao koordinate taeke.
U strukturi Pravougaonik smo definisali taeke lg i dd koje ae nam kasnije trebati prilikom crtanja pravougaonika u menijima, a u strukturi Polje smo kao boolove definisali otkriveno, mina i zastava tj. da vraaaju vrijednosti true ili false, brojMinaUSusjedstvu koji ae služiti prilikom ispisa broja susjednih polja sa minama.U Prozor upisujemo razlieite vrijednosti polja (da li je mina, zastava i sl.)
Kao strukturu odre?ujemo i polje sa dužinom i širinom maxdimX i maxdimY kao konstantim vrijednostima, a kao pravougaonike opcije koje ae se pojavljivati u pravougaonicima u menijima za pobjedu, poraz i glavnom meniju.
*/
const int maxdimX=38, maxdimY=28;
int dimX = 10, dimY=10;   //koliko ima kvadratica vertikalno i horizontalno

enum eTipStatusa{
    eNedefinisano=0,
    eGlavniMeni,
    eIgra,
    ePobjeda,
    ePobjedaSrednji,
    ePobjedaTeski,
    ePoraz,
    eKraj
};

struct Tacka {
       int x,y;  //koorinate tacke
};

struct Pravougaonik {
       Tacka lg, dd;    //lg - lijevi gornji vrh, dd - donji desni vrh
};

struct Polje{
	bool otkriveno, mina;
	bool zastava;
	int brojMinaUSusjedstvu; // >=0 -> broj mina u susjednim poljima
	Pravougaonik prozor;
};

Polje polje[maxdimX][maxdimY];

Pravougaonik naslov, laganiMod, srednjiMod, teskiMod, ponovo, da, ne;
/*
Funkcija testira da li je tacka na koju smo kliknuli unutar pravougaonika. U slueaju da kliknemo bilo gdje unutar pravougaonika za lagani, srednji ili teški mod vratit ae odgovarajuae težine.
U suprotnom, funkcija vraaa vrijednost false i neae ništa uraditi.
*/
bool TackaUnutarPravougaonika(Tacka t, Pravougaonik p) {
     if (t.x>p.lg.x && t.x<p.dd.x && t.y>p.lg.y && t.y<p.dd.y) return true;

     return false;
}
/*
Funkcija crta pravougaonik sa koordinatama lg i dd i ispisuje tekst sa odgovarajuaom velieinom i fontom koji ima lijevo i centralno poravnanje unutar tih pravouganonika
*/
void CrtajPravougaonik(Pravougaonik p, char tekst[], int velicinaFonta) {
     rectangle(p.lg.x,p.lg.y,p.dd.x,p.dd.y);

     settextstyle(3, HORIZ_DIR , velicinaFonta);
     settextjustify(LEFT_TEXT, CENTER_TEXT);
     outtextxy(p.lg.x+5, p.lg.y+velicinaFonta*8, tekst);
}
/*
Funkcija vraaa slueajni broj u rasponu od 0 do max koji ae nam trebati prilikom postavljanja mina na slueajnim mjestima na polju
*/
int GenerisiSlucajniBroj(int max) {
    int v1;
    v1 = rand() % max;
    return v1;
}
/*
Pomocna funkcija koja provjerava da li polje i,j ima minu uz provjeru vrijednosti i,j - da se ne nalaze izvan definisanog raspona. U slueaju da i,j nije u definisano rasponu vratiti false.
*/
bool PoljeImaMinu(int i, int j) {
     if (i<0 || i>=dimX || j<0 || j>=dimY) return false;

     if (polje[i][j].mina) {
        return true;
     }
     return false;
}
/*
Funkcija generiše polja u zavisnosti od vrijednosti dimX i dimY koji opet zavise od odabrane težine i to predstavlja inicijalizaciju.
offsetX i offsetY su integeri koji odre?uju gdje ae se polje crtati tj. koliko je odmaknuto od koordinatnog poeetka, a duzina i visina su varijable koje odre?uju površinu polja tj. pojedinaenih kvadratiaa.
Za poeetne postavke se uzima da nijedno polje nije otkriveno, da nijedna mina ili zastava nije postavljena kao ivarijabla brojMinaUSusjedstvu da je nula.
Posljednje dvije naredbe definišu lijevi gornji vrh odnosno desni donji vrh koji su prethodno odre?eni.
Naredni dio funkcije se odnosi na postavljanje mina na polja. Vrti se while petlja dok je broj postavljenih mina veai od nule tj. dok nisu sve postavljene. Generiše slueajne brojeve x i y od 0 do dimX i dimY (pozivamo funkciju GenerisiSlucajniBroj sa argumentima dimX i dimY).
Dalje provjerava da li polje vea ima minu, ako ne, postavi 'mina' na true i umanji broj mina za postaviti.
U sljedeaem dijelu funkcije se izraeunava broj mina u susjedstvu. Za svako polje provjerava se za svako susjedno polje da li ima minu. Ako susjedno polje ima minu poveaavamo elan brojMinaUSusjedstvu koji ae nam trebati kasnije prilikom otkrivanja susjednih polja kao i štampanja broja susjednih mina na odre?enom kvadratiau/polju koji kliknemo.
*/
void GenerisiPolja(int brojMina) {
     int x,y;

     for (int i=0;i<dimX;i++) {
         for (int j=0;j<dimY;j++) {
             int offsetX=10,offsetY=10,duzina = 20, visina =20;
             Tacka lg = {offsetX+i*duzina, offsetY+j*visina}, dd = {offsetX+(i+1)*duzina, offsetY+(j+1)*visina};
             polje[i][j].otkriveno=false;
             polje[i][j].mina=false;
             polje[i][j].zastava=false;
             polje[i][j].brojMinaUSusjedstvu=0;
             polje[i][j].prozor.lg=lg;
             polje[i][j].prozor.dd=dd;
         }
     }

   while (brojMina>0) {
           //Generisi slucajne brojeve x i y od 0 do dimX i dimY
         x=GenerisiSlucajniBroj(dimX);
         y=GenerisiSlucajniBroj(dimY);
           //Provjeri da li polje vec ima minu, ako ne, postavi 'mina' na true i umanji brojMina za postaviti
         if (polje[x][y].mina==false) {
            polje[x][y].mina=true;
            brojMina--;
         }
   }

   for (int i=0;i<dimX;i++) {
       for (int j=0;j<dimY;j++) {
           if (PoljeImaMinu(i-1,j-1)) {
              polje[i][j].brojMinaUSusjedstvu++;
           }
           if (PoljeImaMinu(i-1,j)) {
              polje[i][j].brojMinaUSusjedstvu++;
           }
           if (PoljeImaMinu(i-1,j+1)) {
              polje[i][j].brojMinaUSusjedstvu++;
           }
           if (PoljeImaMinu(i,j+1)) {
              polje[i][j].brojMinaUSusjedstvu++;
           }
           if (PoljeImaMinu(i+1,j+1)) {
              polje[i][j].brojMinaUSusjedstvu++;
           }
           if (PoljeImaMinu(i+1,j)) {
              polje[i][j].brojMinaUSusjedstvu++;
           }
           if (PoljeImaMinu(i+1,j-1)) {
              polje[i][j].brojMinaUSusjedstvu++;
           }
           if (PoljeImaMinu(i,j-1)) {
              polje[i][j].brojMinaUSusjedstvu++;
           }
       }
   }
}
/*
U prvom dijelu funkcije ispitujemo da li smo kliknuli na neku taeku izvan igrajuaeg polja. U slueaju da smo kliknuli na istu, ne vraaamo ništa i ništa se neae ni desiti.
Dalje ispitujemo da li je polje na koje kliknemo vea ranije otkriveno. U slueaju da nije, otkriveno postaje true i otkrivamo polje. Dalje, u tom slueaju ispitujemo da li je varijabla brojMinaUSusjedstvu koju smo izraeunali u prethodnoj funkciji 0. Ako jeste 0, onda otkrivamo sva susjedna, ako nije otkrivamo samo to jedno polje koje smo kliknuli.
*/
void OtkrijPolje(int x, int y) {

     if (x<0 || x>=dimX) return;
     if (y<0 || y>=dimY) return;

     if (!polje[x][y].otkriveno) {
        polje[x][y].otkriveno = true;
        if (polje[x][y].brojMinaUSusjedstvu==0) {
           OtkrijPolje(x-1, y-1);
           OtkrijPolje(x-0, y-1);
           OtkrijPolje(x+1, y-1);
           OtkrijPolje(x+1, y-0);
           OtkrijPolje(x+1, y+1);
           OtkrijPolje(x-0, y+1);
           OtkrijPolje(x-1, y+1);
           OtkrijPolje(x-1, y-0);
        }
     }
}
/*
Funkcija PrikazGlavnogMenia je funkcija koju pozivamo prilikom samog pokretanja programa. U poeetku definišemo taeke naslovPoc i opcijePoc kao i vrijednosti visinaOpcije i sirinaOpcije koje odre?uju visinu odnosno širinu pravougaonika u koji je smješten tekst.
Lijevi gornji vrh pravougaonika za naslov je ustvari taeka naslovPoc a koordinate donjeg desnog vrha pravimo tako što dodajemo vrijednosti na x odnosno y koordinatu taeke naslovPoc.
Lijevi gornji vrh pravougaonika za lagani mod je taeka opcijePoc, a koordinate donjeg desnog vrha su dobijene dodavajuai vrijednosti na x i y koordinatu taeke laganiMod.lg.
Za srednji i teški nivo je sve isto samo što se pomjeraju po y osi za vrijednost 4 u odnosu na pravougaonik za lagani mod.
U završnom dijelu crtamo pravougaonike nakon što smo im definisali dimenzije i ispisujemo odgovarajuai tekst unutar njih sa odgovarajuaim velieinama.
*/
void PrikazGlavnogMenia() {
     Tacka naslovPoc = {10,10}, opcijePoc = {20,64};
     int visinaOpcije = 30, sirinaOpcije = 200;

    naslov.lg = naslovPoc;
    naslov.dd.x = naslovPoc.x + 300;
    naslov.dd.y = naslovPoc.y + 40;

    laganiMod.lg = opcijePoc;
    laganiMod.dd.x = laganiMod.lg.x + sirinaOpcije;
    laganiMod.dd.y = laganiMod.lg.y + visinaOpcije;

    opcijePoc.y += visinaOpcije + 4;
    srednjiMod.lg = opcijePoc;
    srednjiMod.dd.x = srednjiMod.lg.x + sirinaOpcije;
    srednjiMod.dd.y = srednjiMod.lg.y + visinaOpcije;

    opcijePoc.y += visinaOpcije + 4;
    teskiMod.lg = opcijePoc;
    teskiMod.dd.x = teskiMod.lg.x + sirinaOpcije;
    teskiMod.dd.y = teskiMod.lg.y + visinaOpcije;

    cleardevice();

    CrtajPravougaonik(naslov, (char *)"Minesweeper",4);
    CrtajPravougaonik(laganiMod, (char *)"Lagano",3);
    CrtajPravougaonik(srednjiMod, (char *)"Srednje",3);
    CrtajPravougaonik(teskiMod, (char *)"Tesko",3);

}
/*
Funkcija PrikazTrenutnogStanja je void što znaei da samo ispisuje na ekran, a ono što ona ispisuje jest polje. Petlja ide po svim poljima od i=0 odnosno j=0 do dimX odnosno dimY tj. do zadnjeg polja u redu odnosno koloni.
U slueaju da je polje otkriveno postoje dva slueaja. Ako je to kliknuto polje mina, štampat aemo X u polje. Inaee, štampat ae odre?eni broj u prozor. brojmina[2] je u suštini string sa samo jednim znakom. Treba nam jedan znak jer broj mina ide od 0 do 8 pa je uvijek jednocifren broj. Taj broj mina moramo pretvoriti u ASCII kod tako što dodajemo 48 jer je u ASCII 48 ustvari nula.
Nakon toga imamo null char koji oznaeava da je to kraj. To je sve zbog funkcije CrtajPravougaonik koja dolazi poslije i koja uzima niz char-ova na tom mjestu.
Ako pak polje nije otkriveno nego je zastava tj. kliknuli smo desnim klikom na njega onda štampa Z na to polje. U krajnjem slueaju ako ništa od navedenog nije ueinjeno prazno ae ostati nepopunjeno.
*/
void PrikazTrenutnogStanja() {
    cleardevice();
     for (int i=0;i<dimX;i++) {
         for (int j=0;j<dimY;j++) {
             if (polje[i][j].otkriveno) {
                if (polje[i][j].mina) {
                   CrtajPravougaonik(polje[i][j].prozor, (char *)"X",2);
                } else {
                  char brojMina[2];
                  brojMina[0]=48+polje[i][j].brojMinaUSusjedstvu;
                  brojMina[1]='\0';
                  CrtajPravougaonik(polje[i][j].prozor,brojMina,2);
                }
             } else {
                 if (polje[i][j].zastava) {
                    CrtajPravougaonik(polje[i][j].prozor, (char *)"Z",2);
                 } else {
                      CrtajPravougaonik(polje[i][j].prozor, (char *)"",2);
                 }
             }
         }
     }

}
/*
U sljedeae dvije funkcije imamo sliean slueaj kao i kod funkcije PrikazGlavnogMenia. Umjesto tri opcije koje vraaaju neku vrijednost, ovdje imamo dvije da ili ne. Razlika je u velieini pravougaonika za naslov buduai da je tekst predvi?en za njega duži.
Funkciju PrikazPobjeda pozivamo nakon što status programa postane ePobjeda ili ePobjedaSrednji ili ePobjedaTeski u zavisnosti od izabranog nivoa,a funkciju PrikazPoraz pozivamo odmah nakon što kliknemo na polje sa minom.
*/
void PrikazPobjeda() {
Tacka naslovPoc = {10,10}, opcijePoc = {20,64};
     int visinaOpcije = 30, sirinaOpcije = 200;

    ponovo.lg = naslovPoc;
    ponovo.dd.x = naslovPoc.x + 600;
    ponovo.dd.y = naslovPoc.y + 40;

    da.lg = opcijePoc;
    da.dd.x = da.lg.x + sirinaOpcije;
    da.dd.y = da.lg.y + visinaOpcije;

    opcijePoc.y += visinaOpcije + 4;
    ne.lg = opcijePoc;
    ne.dd.x = ne.lg.x + sirinaOpcije;
    ne.dd.y = ne.lg.y + visinaOpcije;

    cleardevice();

    CrtajPravougaonik(ponovo, (char *)"Pobijedili ste. Zelite li igrati ponovo?",4);
    CrtajPravougaonik(da, (char *)"Da",3);
    CrtajPravougaonik(ne, (char *)"Ne",3);

}

void PrikazPoraz() {
Tacka naslovPoc = {10,10}, opcijePoc = {20,64};
     int visinaOpcije = 30, sirinaOpcije = 200;

    ponovo.lg = naslovPoc;
    ponovo.dd.x = naslovPoc.x + 600;
    ponovo.dd.y = naslovPoc.y + 40;

    da.lg = opcijePoc;
    da.dd.x = da.lg.x + sirinaOpcije;
    da.dd.y = da.lg.y + visinaOpcije;

    opcijePoc.y += visinaOpcije + 4;
    ne.lg = opcijePoc;
    ne.dd.x = ne.lg.x + sirinaOpcije;
    ne.dd.y = ne.lg.y + visinaOpcije;

    cleardevice();
    //clearviewport();

    CrtajPravougaonik(ponovo, (char *)"Izgubili ste. Zelite li igrati ponovo?",4);
    CrtajPravougaonik(da, (char *)"Da",3);
    CrtajPravougaonik(ne, (char *)"Ne",3);

}
/*
U startu ove funkcije definišemo klik kao varijablu eije ae se koordinate mijenajati u zavisnosti od našeg klika. Ukoliko kliknemo kursorom na neku taeku unutar pravougaonika za lagani, srednji ili teški mod vratit ae nam odgovarajuae vrijednosti integera.
Ako kliknemo na taeku unutar pravougaonika za lagani mod vratit ae vrijednost 12 i to je ustvari broj mina.
*/
int KlikMisemGlavniMeni() {
    Tacka klik;

    if (ismouseclick(WM_LBUTTONDOWN)) {
       getmouseclick(WM_LBUTTONDOWN, klik.x,klik.y);
       clearmouseclick(WM_LBUTTONDOWN);

       if (TackaUnutarPravougaonika(klik, laganiMod)) return 12;
       if (TackaUnutarPravougaonika(klik, srednjiMod)) return 70;
       if (TackaUnutarPravougaonika(klik, teskiMod)) return 200;
    }

	return 0;
}
/*
Definišemo taeke klik i desniKlik u poeetku i postavljamo rezultat na eIgra.
Dalje, za svako polje provjerava da li je taeka klik unutar kvadratiaa sa koordinatama i i j.  Ako jeste i ako nije vea postavljena zastava na taj kvadratia onda otkriva to polje i postavlja rezultat na 15 koji je uzet kao proizvoljna vrijednost veaa od nule i koji se vraaa kao vrijednost funkcije. U slueaju da je klik na minu vraaa vrijednost ePoraz što ae kasnije inicirati funkciju PrikazPoraz.
Dakle, kao i u prethodnom slueaju za lijevi klik i ovdje provjerava da li je taeka desniKlik unutar kvadratiaa sa koordinatama i i j. Ako jeste i ako nije otkriveno onda se postavlja zastavica i ako drugi put kliknemo na to isto polje uklonit aemo zastavicu. Rezultat opet postavljamo na 15 i vraaamo ga kao vrijednost funkcije.
*/
int KlikMisemIgra() {
    Tacka klik, desniKlik;
    int res = eIgra;
    int brojac=0;

    if (ismouseclick(WM_LBUTTONDOWN)) {
       getmouseclick(WM_LBUTTONDOWN, klik.x,klik.y);
       clearmouseclick(WM_LBUTTONDOWN);

       for (int i=0;i<dimX;i++) {
           for (int j=0;j<dimY;j++) {
               if (polje[i][j].zastava==false) {
                    if (TackaUnutarPravougaonika(klik, polje[i][j].prozor)) {
                        OtkrijPolje(i, j);
                        brojac++;
                        res = 15;
                        if (polje[i][j].mina) return ePoraz;
                        }
               }
           }
       }
    }
    if (ismouseclick(WM_RBUTTONDOWN)) {
       getmouseclick(WM_RBUTTONDOWN, desniKlik.x,desniKlik.y);
       clearmouseclick(WM_RBUTTONDOWN);

    cout << "x: " << desniKlik.x << ", y: " << desniKlik.y << endl;

       for (int i=0;i<dimX;i++) {
           for (int j=0;j<dimY;j++) {
                if (polje[i][j].otkriveno==false)
                    if (TackaUnutarPravougaonika(desniKlik, polje[i][j].prozor)) {
                        polje[i][j].zastava=!polje[i][j].zastava;
                        res = 15;
                    }
           }
        }
    }
	return res;
}
/*
Kao i kod funkcije KlikMisemGlavniMeni definišemo klik kao varijablu eije ae se koordinate mijenjati u zavisnosti od našeg klika. Dalje, provjeravamo da li je klik unutar pravougaonika "da" ili "ne" te vraaa odgovarajuae vrijednosti u slueaju da jeste.
Ako kliknemo na taeku unutar pravougaonika za opciju "da" onda vraaamo vrijednost eGlavniMeni što znaei da ae nas vratiti na glavni meni tj. na ponovno biranje težina. Ako, ipak, kliknemo na taeku unutar pravougaonika za opciju "ne" onda vraaa vrijednost eKraj i izlazi iz programa.
*/
int KlikMisemKraj() {
    Tacka klik;

    if (ismouseclick(WM_LBUTTONDOWN)) {
       getmouseclick(WM_LBUTTONDOWN, klik.x,klik.y);
       clearmouseclick(WM_LBUTTONDOWN);

       if (TackaUnutarPravougaonika(klik, da)) return eGlavniMeni;
       if (TackaUnutarPravougaonika(klik, ne)) return eKraj;
    }

	return 0;
}
/*
Sljedeaa funkcija provjerava da li je zadovoljen uslov pobjede, tj. da li su sva polja cista. Nakon svakog klika provjerava svako polje i ispituje da li je otkriveno i da li ima minu. Ako je negativan odgovor na oba ova pitanja onda uslov pobjede definitivno nije zadovoljen. U suprotnom ae vratiti vrijednost true i pozvat ae se kasnije funkcija PrikazPobjeda.
*/
bool Pobjeda() {
    for (int i=0;i<dimX;i++) {
         for (int j=0;j<dimY;j++) {
             if ((!polje[i][j].otkriveno) && (!polje[i][j].mina)) return false;
         }
    }
	return true;
}
/*
U poeetku main funkcije postavljamo status na eGlavniMeni koji ae raditi eim se program pokrene i dok mu kasnije ne promijenimo vrijednost tj. dok ne kliknemo na neki od 3 nivoa. Definišemo i varijablu tezina kao integer i postavljamo joj poeetnu vrijednost na 0 jer ae od nje zavisiti ostali statusi.
Naredba initgraph inicijalizira grafieki prikaz i pojavljuje se uvijek sa svojim parametrima gdriver i gmode koji su integeri. Prozor u kojem to radimo ima velieinu 800 sa 600 kao što smo odredili kod naredbe initwindow.
Cleardevice() je funkcija koja eisti ekran i postavlja koordinate na (0,0), a srand(time(NULL)) je generator slueajnih brojeva koji ae nam trebati prilikom postavljanja mina.
Sljedeai i kljueni dio main funkcije jeste do while petlja koja se vrti dok status nije eKraj. Uzeta je do while jer ona svakako izvršava jednu iteraciju i onda imamo moguanost da je ponovimo ako kliknemo na opciju da u prikazu poraza ili pobjede.
Dok je status eGlavniMeni, težina je na nuli i poziva se funkcija PrikazGlavnogMenia i trajat ae sve dok funkcija KlikMisemGlavniMeni ne vrati neku vrijednost osim nule. Kada vrati neku vrijednost tj. težinu, a to mogu biti samo brojevi 12, 70 i 200 odnosno brojevi mina za razlieite nivoe.
Svako polje se formira u zavisnosti od izabrane težine pa tako za lagani mod tj. polje od 12 mina imamo polje 10x10 itd. Nakon toga pozivamo funkciju GenerisiPolja eiji je argument bio brojMina, a koju sada pozivamo sa novim argumentom tezina koji u sustini ima isto znaeenje. Nakon toga se status mijenja u eIgra.
Dok je status eIgra integer rezultat koji je pomoana varijabla definišemo kao vrijednost koju vraaa funkcija KlikMisemIgra, a ta funkcija vraaa ili 15 ili poraz. Ako je rezultat 15 tj. ako ta funkcija vrati vrijednost 15 (ako kliknemo na eisto polje ili postavimo zastavicu), pozivamo funkciju PrikazTrenutnogStanja() sa novim izgledom. Inaee, ako nije rezultat 15 onda vraaa da je status ustvari rezultat samo što promijeni integer u enumerator.
Ako je rezultat nula to znaei da smo izgubili i status dobija vrijednost ePoraz, a ako rezultat ima vrijednost veau od nula onda to znaei da smo pobijedili i status ae poprimiti vrijednost ePobjeda.
Nakon toga se poziva funkcija PrikazPobjeda kao što je vea ranije navedeno ili ako je status ePoraz pozvat ae se funkcija PrikazPoraz sa svojim opcijama.
Dok je status bilo koja od navedene dvije opcije tj. dok gledamo u PrikazPobjeda ili PrikazPoraz definišemo opet pomoanu varijablu pomRez koja poprima vrijednost funkcije KlikMisemKraj koja inaee vraaa vrijednosti eGlavniMeni ili eKraj ili 0. Ako nije nula tj. ako je neka od ovih dvije prethodne pomRez poprima njihove vrijednosti te vraaamo u glavni meni ili pak izlazimo iz prozora i time se završava igra.

*/
int main()
{
    cout << "1" << endl;
	eTipStatusa status=eGlavniMeni;
	int tezina=0;

    cout << "1" << endl;
    int gdriver=6, gmode=2;
    initgraph (&gdriver,&gmode,"");
    initwindow(800, 600);
    cout << "1" << endl;
    cleardevice();
    cout << "1" << endl;
    srand (time(NULL));

    cout << "1" << endl;
    do {
        if (status==eGlavniMeni) {
            tezina=0;
           PrikazGlavnogMenia();

           while(tezina==0) {
               delay(50);
               tezina=KlikMisemGlavniMeni();
           }
            if (tezina==12) {
                dimX=10;
                dimY=10;
            }
            if (tezina==70) {
                dimX=20;
                dimY=20;
            }
            if (tezina==200) {
                dimX=maxdimX;
                dimY=maxdimY;
            }
            srand(time(0));
            GenerisiPolja(tezina);

            status=eIgra;
        }
        std::cout << "Tezina izabrana je: " << tezina << std::endl;

        PrikazTrenutnogStanja();
        while(status==eIgra) {
            delay(50);
            int rezultat=KlikMisemIgra();
            if (rezultat==15) {
                PrikazTrenutnogStanja();
            } else {status=(eTipStatusa)rezultat;}

            if (rezultat==0) {
                PrikazTrenutnogStanja();
                status = ePoraz;
            } else if (rezultat>0) {
                if (Pobjeda()) {
                    status = ePobjeda;
                }

            }
        }

        if (status==ePobjeda) {
            PrikazPobjeda();
            }
        if (status==ePoraz) {
            PrikazPoraz();
        }

        while((status==ePobjeda) || (status==ePoraz)) {
            delay(50);
            int pomRez = KlikMisemKraj();
            if (pomRez!=0) {
                status = (eTipStatusa) pomRez;
            }
        }
    } while (status!=eKraj);

    return 0;
}

