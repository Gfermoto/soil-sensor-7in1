#pragma once
#include "Arduino.h"

enum HTTPMethod
{
    HTTP_GET,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_HEAD,
    HTTP_OPTIONS,
    HTTP_PATCH
};

class IPAddress
{
   public:
    String toString() const
    {
        return String("127.0.0.1");
    }
};

class MockClient
{
   public:
    IPAddress remoteIP() const
    {
        return IPAddress();
    }
};

class WebServer
{
   public:
    WebServer(int) {}
    bool hasArg(const String&) const
    {
        return false;
    }
    String arg(const String&) const
    {
        return String("");
    }
    bool hasHeader(const String&) const
    {
        return false;
    }
    String header(const String&) const
    {
        return String("");
    }
    HTTPMethod method() const
    {
        return HTTP_GET;
    }
    String uri() const
    {
        return String("/");
    }
    MockClient client() const
    {
        return MockClient();
    }
    void send(int, const char*, const String&) {}
    void sendHeader(const String&, const String&, bool = false) {}
};
