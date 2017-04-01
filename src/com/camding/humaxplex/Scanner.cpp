/*
 * Scanner.cpp
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#include "Options.h"
#include "Scanner.h"
#include <string>

using com::camding::humaxplex::Scanner;

std::vector<std::string> Scanner::DEVICE_CATEGORY_STR = std::vector<std::string>({
  std::string("urn:schemas-upnp-org:device:MediaServer:1"),
  std::string("urn:schemas-upnp-org:device:MediaRenderer:1")
});

enum DEVICETYPE{
  SERVER = 0,
  RENDERER
};

Scanner::Scanner(const Options& options) {
  /* Not used */
}

Scanner::~Scanner() {
  UpnpUnRegisterClient(client_);
}

void Scanner::Init() {
  std::weak_ptr<Scanner>* selfPtr = new std::weak_ptr<Scanner>(shared_from_this());
  auto callback = [](Upnp_EventType EventType, void* Event, void* Cookie) -> int{
    auto self = static_cast<std::weak_ptr<Scanner>*>(Cookie);
    auto selfLocked = self->lock();
    if (selfLocked) {
      return selfLocked->UpnpCallback(EventType, Event);
    }
    return UPNP_E_FINISH;
  };
  if (UpnpRegisterClient(callback, selfPtr, &client_) != UPNP_E_SUCCESS) {
    throw std::runtime_error("Failed to register upnp client");
  }
  initialized_ = true;
}

int Scanner::UpnpCallback(Upnp_EventType EventType, void *Event) {
  //FIXME code this
  return UPNP_E_SUCCESS;
}

void Scanner::Scan() {
  if (!initialized_)
    Init();
  if (UpnpSearchAsync(client_, 15, DEVICE_CATEGORY_STR.at(DEVICETYPE::SERVER).c_str(), nullptr) != UPNP_E_SUCCESS) {
    throw std::runtime_error("Failed to search for upnp");
  }
  //FIXME code this
}
