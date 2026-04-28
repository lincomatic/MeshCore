#if defined(CRISPR)
#pragma once

class Crispr {
  char _tmp[200] = {0};
  char _rsp[200] = {0};
  mesh::Radio &_radio;

public:
  bool _repeat = false;
#ifdef SX126X_RX_BOOSTED_GAIN
  bool _rxBoostedGain =  SX126X_RX_BOOSTED_GAIN;
#endif

  Crispr(mesh::Radio &radio) : _radio(radio)
  {}

  const char* processCmd(const char *cmd);
};

#endif // CRISPR
