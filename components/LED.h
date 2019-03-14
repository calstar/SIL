#pragma once

class LED {
  string name;
  bool activated;

public:
  void set(bool val);
  bool val();
  LED(string name);
};
