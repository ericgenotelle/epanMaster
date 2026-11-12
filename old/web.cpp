#include "netman.h"
// #include "SPIFFS.h"

// https://www.scriptol.fr/html5/progress.php
// https://developer.mozilla.org/fr/docs/Web/HTML/Element/progress
// 
// https://medium.com/@predragdavidovic10/native-dual-range-slider-html-css-javascript-91e778134816
// https://w3collective.com/double-range-slider-html-css-js/

#include "config.h"
#include "sound.h"
#include "device.h"
#include "web.h"

NetMan nm;
char msg[200];


//const char P_VERSION_DATE[] = __DATE__ " - " __TIME__ ;


//------------------------------------------------------------------------------------------------------------------------
/// @brief  Handle ROOT HTML page
/// @param request 
void _handleRoot(AsyncWebServerRequest *request) 
{
  nm._request = request;
  String ligne;

  DEBUG1("----------------------");
  DEBUG1("_handleRoot()");

  // --------------------------
  // Affichage de la page HTML
  // --------------------------
  nm.clear();
  nm.head( String(APPLI_NAME) );
  nm.add("<script>");
  nm.add("var idChange=0;");
  nm.add("function doAction(action,arg1) {");
//  nm.add( "  alert(action);");
  nm.add( "  var xhttp = new XMLHttpRequest();");
  nm.add( "  xhttp.onreadystatechange = function() {");
  nm.add( "    if (this.readyState == 4 && this.status == 200) {");
  nm.add( "    }" );
  nm.add( "  };" );
  nm.add( "  var tmp = '&idChange='+idChange;" );
  nm.add( "  if (action=='bpm') {");
  nm.add( "    tmp = '&arg1=' + document.getElementById('bpmtxt').value;");
  nm.add( "  } else if ( action=='volgen') {");
  nm.add( "    tmp = '&arg1=' + document.getElementById('volgentxt').value;");
  nm.add( "  } else {");
  nm.add( "    tmp = '&arg1=' + arg1");
  nm.add( "  }" );
  nm.add( "  var uri = 'rest?action=' + action + tmp;" );
  nm.add( "  xhttp.open('GET', uri, true);");
  nm.add( "  xhttp.send(); ");
  nm.add( "}" );
  nm.add( "" );
  nm.add( "function checkActivation() {");
  nm.add( "  if (document.getElementById('nbnotes').innerHTML=='0' ) {");
  nm.add( "   document.getElementById('learn').disabled=true;");
  nm.add( "   document.getElementById('play').disabled=true;");
  nm.add( "   document.getElementById('bpminput').disabled=true;");
  nm.add( "   document.getElementById('bpmtxt').disabled=true;");
  nm.add( "  } else {" );
  nm.add( "   document.getElementById('learn').disabled=false;");
  nm.add( "   document.getElementById('play').disabled=false;");
  nm.add( "   document.getElementById('bpminput').disabled=false;");
  nm.add( "   document.getElementById('bpmtxt').disabled=false;");
  nm.add( "  }" );
  nm.add( "  if (document.getElementById('etat').innerHTML=='stop' ) {");
  nm.add( "   document.getElementById('stop').disabled=true;");
  nm.add( "  } else {" );
  nm.add( "   document.getElementById('stop').disabled=false;");
  nm.add( "  }" );
  nm.add( "}" );
  nm.add( "function getMusicData() {");
  nm.add( "  var xhttp = new XMLHttpRequest();");
  nm.add( "  xhttp.onreadystatechange = function() {");
  nm.add( "    if (this.readyState == 4 && this.status == 200) {");
  nm.add( "      const obj = JSON.parse(this.responseText);");
  nm.add( "      idChangeNew = obj.idChange;");
  nm.add( "      if (idChange != idChangeNew) {");
  nm.add( "        document.getElementById('filename').innerHTML = obj.filename;");
  nm.add( "        document.getElementById('nbnotes').innerHTML = obj.nbnotes;");
  nm.add( "        document.getElementById('curnote').innerHTML = obj.curnote;");
  nm.add( "        document.getElementById('bpmbase').innerHTML = obj.bpmbase;");
  nm.add( "        document.getElementById('etat').innerHTML = obj.etat;");
  nm.add( "        document.getElementById('msg').innerHTML = obj.msg;");
  nm.add( "        document.getElementById('bpmtxt').textContent = obj.bpm;");
  nm.add( "        document.getElementById('bpminput').value = obj.bpm;");
  nm.add( "        document.getElementById('volgentxt').textContent = obj.volgen;");
  nm.add( "        document.getElementById('volgeninput').value = obj.volgen;");
  nm.add( "        document.getElementById('version').value = obj.version;");
  nm.add( "        checkActivation();");
  nm.add( "      }" );
  nm.add( "    }" );
  nm.add( "  };" );
  nm.add( "  xhttp.open('GET', '/rest?action=info', true);");
  nm.add( "  xhttp.send(); ");
  nm.add( " }" );
   nm.add("</script>");

  nm.add("<body>");
  nm.add( "<h2>" + String(APPLI_NAME) + "</h2>");
  nm.add( "<p><b><span id='msg'></b><p>");
  
  nm.add( "<table><tr><th>Volume</th>");
  nm.add( "  <td><input id='volgeninput' type='range' min='0' max='100' step=1 value='0'/><output id='volgentxt'></output></td></tr>"); 
  nm.add( "<tr><th>Fichier MIDI</th>"); 
  nm.add("     <td><form method='POST' action='/upload' enctype='multipart/form-data'><input type='file' id='filemidi' accept='.mid,.midi' name='data' onchange='form.submit()' /> </form></td></tr>");
  nm.add("<tr><th> &nbsp;</th>  <td><button onclick=\" doAction('info','');  \"  id='refresh' >Refresh</button></td></tr>");
  nm.add( "</table><p>");

  nm.add( "<table>"); 
  nm.add( "<tr><th>Commandes</th><td>"); 
  nm.add("  <button onclick=\" doAction('learn','');  \"  id='learn' >Apprentissage</button>");
  nm.add("  <button onclick=\" doAction('play','');  \"  id='play' >Séquenceur</button>");
  nm.add("  <button onclick=\" doAction('stop','');  \"  id='stop' >Stop</button>");
  nm.add("</td></tr>");
  nm.add( "<tr><th>BPM</th>"); 
  nm.add("  <td><input id='bpminput' type='range' min='30' max='200' step=1 value='30'/><output id='bpmtxt'></output></td></tr>");

  nm.row( "<th> Fichier </th>   <td> <span id='filename'></td>" );
  nm.row( "<th> Nb notes</th>   <td> <span id='nbnotes'></td>" );
  nm.row( "<th> BPM d'origine</th>   <td> <span id='bpmbase'></td>" );
  nm.row( "<th> Etat </th>   <td> <span id='etat'></td>" );
  nm.row( "<th> N° note en court</th>   <td> <span id='curnote'></td>" );
  nm.add( "</table>");

  // ----------------------------------------
  // list of notes/sounds 
  nm.add( "<p><table><tr>"); 
  for (int i=0; i < NB_SOUNDS; i++) {
    nm.add("<td><button onclick=\" doAction('playnote','" + String(i) + "');  \"  id='playnote' >" + String(sounds[i].noteName) + "</button></td>");
  }
  nm.add( "</tr></table>");

  // ----------------------------------------
  // Keyboard
  // ----------------------------------------
  nm.add( "<p><table class='kb'><tr>"); 
  nm.add( "<tr>"); 
  nm.add ( "  <td class='vide'> &nbsp; </td>");
  nm.add ( "  <td colspan=2 class='noire'> <button onclick=\"doAction('playnote','1');\"  id='playnote'> A# </button> </td>" );
  nm.add ( "  <td class='vide' colspan=2> &nbsp; </td>");
  nm.add ( "  <td colspan=2 class='noire'> <button onclick=\"doAction('playnote','4');\"  id='playnote'> C# </button> </td>" );
  nm.add ( "  <td colspan=2 class='noire'> <button onclick=\"doAction('playnote','6');\"  id='playnote'> D# </button> </td>" );
  nm.add ( "  <td class='vide' colspan=2> &nbsp; </td>");
  nm.add ( "  <td colspan=2 class='noire'> <button onclick=\"doAction('playnote','9');\"  id='playnote'> F# </button> </td>" );
  nm.add ( "  <td colspan=2 class='noire'> <button onclick=\"doAction('playnote','11');\" id='playnote'> G# </button> </td>" );
  nm.add ( "  <td colspan=2 class='noire'> <button onclick=\"doAction('playnote','13');\" id='playnote'> A# </button> </td>" );
  nm.add( "</tr>"); 
  nm.add( "<tr>"); 
  nm.add ( "  <td colspan=2 class='blanche'> <button onclick=\"doAction('playnote','0');\"  id='playnote'> A </button> </td>" );
  nm.add ( "  <td colspan=2 class='blanche'> <button onclick=\"doAction('playnote','2');\"  id='playnote'> B </button> </td>" );
  nm.add ( "  <td colspan=2 class='blanche'> <button onclick=\"doAction('playnote','3');\"  id='playnote'> C </button> </td>" );
  nm.add ( "  <td colspan=2 class='blanche'> <button onclick=\"doAction('playnote','5');\"  id='playnote'> D </button> </td>" );
  nm.add ( "  <td colspan=2 class='blanche'> <button onclick=\"doAction('playnote','7');\"  id='playnote'> E </button> </td>" );
  nm.add ( "  <td colspan=2 class='blanche'> <button onclick=\"doAction('playnote','8');\" id='playnote'> F </button> </td>" );
  nm.add ( "  <td colspan=2 class='blanche'> <button onclick=\"doAction('playnote','10');\" id='playnote'> G </button> </td>" );
  nm.add ( "  <td colspan=2 class='blanche'> <button onclick=\"doAction('playnote','12');\" id='playnote'> A </button> </td>" );
  nm.add ( "  <td class='vide'> &nbsp; </td>");
  nm.add( "</tr>"); 
  nm.add( "</tr></table>");
  nm.add( "<p><span id='version'></span>");

  // ----------------------------------------
  // Javascript - en fin de page
  // ----------------------------------------
  nm.add( "<script>");
  nm.add("");
  nm.add( "const bpmtxt = document.querySelector('#bpmtxt');"  );
  nm.add( "const bpminput = document.querySelector('#bpminput');" );
  nm.add( "bpmtxt.textContent = bpminput.value;") ;
  nm.add( "bpminput.addEventListener('input', (event) => {  bpmtxt.textContent = event.target.value; }); ") ;
  nm.add( "bpminput.addEventListener('mouseup', (event) => {  doAction('bpm',event.target.value); }); ") ;
  nm.add("");
  nm.add( "const volgentxt = document.querySelector('#volgentxt');"  );
  nm.add( "const volgeninput = document.querySelector('#volgeninput');" );
  nm.add( "volgentxt.textContent = volgeninput.value;") ;  
  nm.add( "volgeninput.addEventListener('input', (event) => {  volgentxt.textContent = event.target.value; }); ") ;
  nm.add( "volgeninput.addEventListener('mouseup', (event) => {  doAction('volgen',event.target.value); }); ") ;
  nm.add("");
  nm.add( "setInterval(function() {getMusicData();}, 2000);");
  nm.add( "" );
  nm.add( "</script>");
  nm.add("</body>");
  nm.add("</html>");
  nm.sendHTML();
}



//------------------------------------------------------------------------------------------------------------------------
/// @brief  Handle configuration HTML page
/// @param request 
void _handleConfig(AsyncWebServerRequest *request) 
{
  nm._request = request;
  String ligne;

  DEBUG1("----------------------");
  DEBUG1("_handleConfig()");

  // --------------------------
  // Affichage de la page HTML
  // --------------------------
  nm.clear();
  nm.head(String (APPLI_NAME) + " - Configuration");
  nm.add("<script>");
  nm.add("function doAction(note,threshold) {");
//  nm.add( "  alert(action);");
  nm.add( "  var xhttp = new XMLHttpRequest();");
  nm.add( "  xhttp.onreadystatechange = function() {");
  nm.add( "    if (this.readyState == 4 && this.status == 200) {");
  nm.add( "    }" );
  nm.add( "  };" );
  nm.add( "  var uri = '/rest?action=threshold&arg1=' + note + '&arg2=' + threshold" );
  nm.add( "  xhttp.open('GET', uri, true);");
  nm.add( "  xhttp.send(); ");
  nm.add( "}" );
  nm.add( "" );
   nm.add("</script>");

  nm.add("<body>");
  nm.add( "<h2>" + String(APPLI_NAME) + " Configuration</h2>");
  nm.add( "<table>");
  nm.add( "<tr><th>Note</th><th>Seuil déclenchement</th></tr>");
  
  // Display notes
  for (int i=0; i<NB_SOUNDS; i++) {
      nm.add( "<tr><th>" + String(sounds[i].noteName) + "</th>");
      nm.add( "  <td><input id='thresholdinput" + String(i) + "' type='range' min='0' max='4095' step=1 value='" + sounds[i].threshold + "'/><output id='thresholdtxt" + String(i) + "'></output></td></tr>"); 
  }
  nm.add( "</tr></table>");

  // ----------------------------------------
  // Javascript - end of page
  // ----------------------------------------
  nm.add( "<script>");
  nm.add("");
  for (int i=0; i<NB_SOUNDS; i++) {
    nm.add( "const thresholdtxt" + String(i) + " = document.querySelector('#thresholdtxt" + String(i) + "');"  );
    nm.add( "const thresholdinput" + String(i) + " = document.querySelector('#thresholdinput" + String(i) + "');" );
    nm.add( "thresholdtxt" + String(i) + ".textContent = thresholdinput" + String(i) + ".value;") ;
    nm.add( "thresholdinput" + String(i) + ".addEventListener('input', (event) => {  thresholdtxt" + String(i) + ".textContent = event.target.value; }); ") ;
    nm.add( "thresholdinput" + String(i) + ".addEventListener('mouseup', (event) => {  doAction('" + String(i) + "',event.target.value); }); ") ;
  }
  nm.add( "" );
  nm.add( "</script>");
  nm.add("</body>");
  nm.add("</html>");
  nm.sendHTML();
}



//------------------------------------------------------------------------------------------------------------------------
/// @brief HANDLE web to manage actions requested by ROOT page
/// @param request 
void _handleRest(AsyncWebServerRequest *request) 
{
  DEBUG1("----------------------");
  DEBUG1("_handleRest()");

  String ligne;

  nm._request = request;
  char value[255];
  int nbChanged=0;
  bool res=false;
  bool actionDone=false;
  int16_t idChangeClient;

  nm.getParamString("action",value,&nbChanged);
  nm.clear();

  //-----------------------------------------------
  // play a note
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"playnote") == 0 ) ) {
    actionDone = true;
    res = true;
    nm.add( "ok" );
    DEBUG1("Web.playnote");

    int16_t idxSound = 0;
    nm.getParamInt("arg1",&idxSound,&nbChanged);
    touchNote( idxSound , 100);
    nm.sendHTML();   // exception for 'playnote' -> quick return 
    return;
  }     

  //-----------------------------------------------
  // Get INFO in Json format
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"info") == 0) ) {
    actionDone = true;
    res = true;
    idChangeClient = -1;
    nm.getParamInt("idchange", &idChangeClient,&nbChanged);
    ligne = String("{");
    if ( idChangeClient != song.idChange ) {
      ligne += " \"change\":1,";
      ligne += " \"idchange\":\"" + String(song.idChange) + "\",";
      ligne += " \"filename\":\"" + String(song.filename) + "\",";
      ligne += " \"nbnotes\":" + String(song.nbNotes) + ",";
      ligne += " \"bpm\":" + String(song.bpm) + ",";
      ligne += " \"bpmbase\":" + String(song.bpmBase) + ",";
      ligne += " \"volgen\":" + String(volumeGen) + ",";
      ligne += " \"version\":\"" + String(PROGRAMME_VERSION) + " - " + String(PROGRAMME_DATE) + "\",";
      ligne += " \"msg\":\"" + String(msg) + "\",";
    } else {
      ligne += " \"change\":0,";
    }
    
    if (song.playing) {
      if ( song.modeAuto ) {
        ligne += " \"etat\":\"Mode AUTO\",";
      } else {
        ligne += " \"etat\":\"Mode JEU\",";
      }
      ligne += " \"curnote\":" + String(song.curNote+1) + ",";
    } else {
      ligne += " \"etat\":\"stop\",";
      ligne += " \"curnote\":\"\",";
    }
    ligne += " \"last\":1"; 
    ligne += "}";
    nm.add( ligne );
  }     

  //-----------------------------------------------
  // Start song in mode LEARN
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"learn") == 0 ) ) {
    actionDone = true;
    res = true;
    nm.add( "ok" );
    DEBUG1("Web.learn");
    song.modeAuto = false;
    startSong();
  }     

  //-----------------------------------------------
  // Start song in mode PLAY
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"play") == 0 ) ) {
    actionDone = true;
    res = true;
    nm.add( "ok" );
    DEBUG1("Web.play");
    song.modeAuto = true;
    startSong();
  }     

  //-----------------------------------------------
  // Stop playing stop
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"stop") == 0 ) ) {
    actionDone = true;
    res = true;
    nm.add( "ok" );
    DEBUG1("Web.stop");
    stopSong();
  }     

  //-----------------------------------------------
  // change BPM
  //-----------------------------------------------
  if ((!actionDone)  && ( strcmp(value,"bpm") == 0 ) ) {
    actionDone = true;
    res = true;
    song.idChange++;
    nm.add( "ok" );
    DEBUG1("Web.bpm");

    int16_t bpmnew = song.bpm;
    nm.getParamInt("arg1",&bpmnew,&nbChanged);
//DEBUG2("Web.bpmnew",bpmnew);
    ajustSongBPM(&song,bpmnew);
  }     

  //-----------------------------------------------
  // change general volume
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"volgen") == 0 ) ) {
    actionDone = true;
    res = true;
    song.idChange++;
    nm.add( "ok" );
    DEBUG1("Web.volgen");

    int16_t vol = volumeGen;
    nm.getParamInt("arg1",&vol,&nbChanged);
//DEBUG2("Web.bpmnew",bpmnew);
    volumeGen = vol;
//    saveConfigVolumeGen();
  }

  //-----------------------------------------------
  // Seuil
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"threshold") == 0 ) ) {
    actionDone = true;
    res = true;
    song.idChange++;
    int16_t note;
    int16_t threshold;
    nm.getParamInt("arg1",&note,&nbChanged);
    nm.getParamInt("arg2",&threshold,&nbChanged);
    nm.add( "ok" );
    
    if ( (note>=0) && (note<NB_SOUNDS) ) {
      sounds[note].threshold = threshold;
    }
  }     


  //-----------------------------------------------
  // Save config in EEPROM
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"saveconfig") == 0 ) ) {
    actionDone = true;
    res = true;
    saveConfigEEPROM();
  }     

  //-----------------------------------------------
  // Reset config in EEPROM
  //-----------------------------------------------
  if ( (!actionDone)  && (strcmp(value,"resetconfig") == 0 ) ) {
    actionDone = true;
    res = true;
    song.idChange++;
    nm.add( "ok" );
    DEBUG1("Web.resetconfig");

    resetConfigEEPROM();
  }     

  nm.sendHTML();
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief Handle FILEUPLOAD 
/// @param request   AsyncWebServerRequest
/// @param filename  uploaded filename
/// @param index 
/// @param data 
/// @param len 
/// @param final 
//  @note https://github.com/smford/esp32-asyncwebserver-fileupload-example/blob/master/example-01/example-01.ino
void _handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) 
{
  DEBUG1("_handleUpload");
  static size_t taille=0;
  static char _filename[100];
  
  if (!index) {
    DEBUG1(_filename);
    fileMidiLen = 0;
    taille = 0;
    msg[0] = 0;
    snprintf( _filename, sizeof(_filename) -1, "%s", filename.c_str() ) ;
    DEBUG2("filename",_filename)
  }

  if (len) {
    taille += len;

    // check if size is OK
    if ( taille > FILE_MIDI_MAX ) {
      fileMidiLen = 0;
    } else {
      DEBUG2("copy",len);
      memcpy( fileMidi + fileMidiLen , data, len);
      fileMidiLen += len;
    }
  }

  if (final) {
    song.idChange++;
    // check if MIDI file is OK
    if (fileMidiLen==0) {
      sprintf(msg,"Taille du fichier trop grande >=%d octets (max=%d octets)", taille, FILE_MIDI_MAX );
      DEBUG1(msg);
      initiateSong(&song);
    } else {
      // Try to load the song
      loadSong(&song,fileMidi,fileMidiLen);
      strcpy(song.filename,_filename);
      if (song.nbNotes==0) {
        sprintf(msg,"Ce n'est pas un fichier MIDI" );
        DEBUG1(msg);
        initiateSong(&song);
      }
    }

    

    request->redirect("/");
  }
}


//------------------------------------------------------------------------------------------------------------------------
/// @brief Setup Web contex : connect to WIFI + reference HTML handler
void setupWeb()
{
  DEBUG1("setupWeb ...");

/*
  // Initialize SPIFFS
  DEBUG1("  Setup SPIFFS...")
  if(!SPIFFS.begin(true)){
    DEBUG1("An Error has occurred while mounting SPIFFS");
    return;
  }
  DEBUG1("     SPIFFS OK")
*/

  DEBUG1("  Setup WIFI...")
  IPAddress ip;
  IPAddress masqueip;
  ip.fromString(WIFI_IP);
  masqueip.fromString(WIFI_MASQUE);
  //nm.connectWifiAP(WIFI_SSID,WIFI_PASSWORD,ip,ip,masqueip);
  nm.connectWifiSTA(WIFI_SSID,WIFI_PASSWORD,ip,ip,masqueip);
  DEBUG1("     WIFI OK")

  DEBUG1("  Setup Web pages...")
  // Route for root / web page
//  nm._server->on("/", HTTP_GET, [](AsyncWebServerRequest *request){
//    request->send(SPIFFS, "/index.html", "text/html");
//  });
  nm.on("/",_handleRoot);
  nm.on("/config",_handleConfig);
  nm.on("/rest",_handleRest);
  // run handleUpload function when any file is uploaded
  nm._server->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
      }, _handleUpload);
  nm.begin();
  DEBUG1("      Web pages OK.")
  DEBUG1("setupWeb OK."); 

}


