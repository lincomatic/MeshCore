#if CRISPR == 1
#include "Mesh.h"
#include "MyMesh.h"
#include "Crispr.h"

//#if CRISPR_HAS_BUZZER == 1
  #include "UITask.h"
  extern UITask ui_task;
//#endif

const char *Crispr::processCmd(const char *cmdstr) {
  bool ok = false;
  strncpy(_tmp,cmdstr,sizeof(_tmp)-1);
  _tmp[sizeof(_tmp)-1] = 0;
  const char *parts[5]; 
  int num = mesh::Utils::parseTextParts(_tmp, parts, 5,' ');
  if (num >= 1) {
    sprintf(_rsp,"OK> %s ",parts[0]);

    if (!strcmp(parts[0],"repeat")) {
      // WARNING: THIS IS ONLY FOR SPECIAL CASE USES, E.G. WHEN FAR AWAY FROM REPEATERS
      // OR WHEN YOUR COMPANION IS IN A FIXED LOCATION WITH A GOOD ANTENNA AND LOS TO OTHER
      // REPEATERS
      // TURNING REPEAT ON INDISCRIMINATELY COULD SPAM THE MESH W/ JUNK PACKETS, AND
      // TURN IT INTO MESHTASTIC
      // NOTE THAT REPEATING IS VOLATILE, AND IS DISABLED AT EVERY BOOT
      if (num == 2) {
        if (!strcmp(parts[1],"on")) _repeat = true;
        else _repeat = false;
      }
      sprintf(_rsp+strlen(_rsp),"%s",_repeat ? "on" : "off");
      ok = true;
    }
#if ENV_INCLUDE_GPS == 1
    else if (!strcmp(parts[0],"gps")) {
      int mode = -1;
      if (num == 2) {
        mode = !strcmp(parts[1],"on") ? 1 : 0;
	      ui_task.crisprGetSensors()->setSettingValue("gps", mode ? "1" : "0");
      }
      sprintf(_rsp+strlen(_rsp),"%s",the_mesh.crisprGps(mode) ? "on" : "off");
      ok = true;
    }
    else if (!strcmp(parts[0],"gpsintvl")) {
      int sec = -1;
      if (num == 2) {
        sec = atoi(parts[1]);
      }
      sprintf(_rsp,"OK> GPS interval %d",the_mesh.crisprGpsInterval(sec));
      if (sec >= 0) {
	      sprintf(_tmp, "%u", sec);
	      ui_task.crisprGetSensors()->setSettingValue("gps_interval", _tmp);
      }
      ok = true;
    }
#endif // ENV_INCLUDE_GPS == 1
    /*
    else if (!strcmp(parts[0],"txdelay")) {
      NodePrefs *prefs = the_mesh.getNodePrefs();
      if (num == 2) {
        if (!strcmp(parts[1],"on")) _repeat = true;
        else _repeat = false;
      }
      sprintf(_rsp+strlen(_rsp),"%s",_repeat ? "on" : "off");
      ok = true;
    }
    */
#if CRISPR_HAS_BUZZER == 1
    else if (!strcmp(parts[0],"beep")) {
      if (num == 2) {
        ui_task.crisprSound(!strcmp(parts[1],"on") ? 1 : 0);
      }
      sprintf(_rsp+strlen(_rsp),"%s",ui_task.crisprSound() ? "on" : "off");
      ok = true;
    }
#endif // CRISPR_HAS_BUZZER
    else if (!strcmp(parts[0],"reboot")) {
      //sprintf(_rsp,"rebooting..."); 
      board.reboot(); // doesn't return
      //ok = true;
    }
    else if (!strcmp(parts[0],"shutdown")) {
      //sprintf(_rsp,"shutting down..."); 
      radio_driver.powerOff();
      board.powerOff(); // doesn't return
      //ok = true;
    }
 #ifdef SX126X_RX_BOOSTED_GAIN
    else if (!strcmp(parts[0],"rxboost")) {
      extern RADIO_CLASS radio;

      if (num == 2) {
        _rxBoostedGain = !strcmp(parts[1],"on") ? true : false;
        radio.setRxBoostedGainMode(_rxBoostedGain);
      }
      sprintf(_rsp+strlen(_rsp),"%s",_rxBoostedGain ? "on" : "off");
      ok = true;
    }
#endif // SX126X_RX_BOOSTED_GAIN
    else if (!strcmp(parts[0],"rstats")) { 
      sprintf(_rsp,"noise floor: %d", _radio.getNoiseFloor());
      sprintf(_rsp+strlen(_rsp),"\nlast SNR: %0.1f", radio_driver.getLastSNR());
      sprintf(_rsp+strlen(_rsp),"\nlast RSSI: %d", (int)radio_driver.getLastRSSI());
      ok = true;
    }
    else if (!strcmp(parts[0],"help")) { 
      sprintf(_rsp,"reboot,rstats,shutdown");
#if CRISPR_HAS_BUZZER == 1
      strcat(_rsp,",beep");
#endif
#if ENV_INCLUDE_GPS == 1
      strcat(_rsp,",gps,gpsintvl");
#endif // ENV_INCLUDE_GPS == 1
#ifdef SX126X_RX_BOOSTED_GAIN
      strcat(_rsp,",rxboost");
#endif
      if ((num == 2) && !strcmp(parts[1],"og")) strcat(_rsp,",repeat");
      ok = true;
    }
#ifdef ESP_PLATFORM_notworking
    else if (!strcmp(parts[0],"ota")) {
      board.startOTAUpdate("CC9", _rsp);
      sprintf(_rsp,"ota..."); 
      ok = true;
    }
#endif
  }

  if (!ok) {
    sprintf(_rsp,"ERROR> %s",cmdstr);
  }
  return _rsp;
}

#endif // CRISPR
