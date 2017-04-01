/*
 * Scanner.h
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#pragma once

#include <upnp/upnp.h>
#include <memory>
#include <vector>

namespace com {
namespace camding {
namespace humaxplex {

class Scanner : public std::enable_shared_from_this<Scanner> {
public:
  Scanner();
  virtual ~Scanner();

  void Scan();

private:
  static std::vector<std::string> DEVICE_CATEGORY_STR;
  UpnpClient_Handle client_;
  int UpnpCallback(Upnp_EventType EventType, void *Event);
};

}}}
