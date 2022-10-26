#include <pgmspace.h>

#define SECRET
#define THINGNAME "MyNewESP32"

const char WIFI_SSID[] = "strathmore";
const char WIFI_PASSWORD[] = "5trathm0re";
const char AWS_IOT_ENDPOINT[] = "a2h9mbgqj099ur-ats.iot.us-east-1.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAMZRU/j9FeomgnEwxUUd62HAVRMGMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMjEwMjIwNjEy
NDZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCTtMj1v0J7JbLr1GC7
fCWvLD140c1T0LK8qwXW6uUF52ksAP7nd5CFFwxo8GnFIZWo8K8fNmBmImZIt/iG
iYXrR5bP+8PRz/gPp339z8qMAU3/zd115wp4si8mtLEGvbqd4dZPs4J/hutJK6pC
LzDwLm+jJuz3yz7ekL9aAx3/oUBJM4BebNsgCSiY3uSmVn6cYUF3R9o8jalCd2h7
jgvoZJEDG6fBg6wasLX2XkcScmh6qmqVAS+jDA+iSRJBciGNYRl8Y3U9drYD0H6F
NJx98Rtl9DEbGmflkxv8wz7sM2z9py3huzoThrVjfwa+pYi9fstoWYlXB868xLY1
3iB5AgMBAAGjYDBeMB8GA1UdIwQYMBaAFPq78naO77hBtGObHtKeptXBdAvNMB0G
A1UdDgQWBBQX1JDY1XvrKFw4bUlkc3k05vNlIzAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEADgdXq43S8DBW2LD6uPbFUwJw
R0yxjYNXclegBNiUwxMdviy4ZvzuvP8d+jtXAQ7LLVVB2ULwTRXzso7reYvDYi6+
L4QDmrb0/9WELl84d7RSvTsVWIqR21XGj1rwil1ODu3ftjHek4r1G/fgEJaC2Vra
hN82iZSdFzp6VHM1YmcLtCawwq/52FOe8YF8iQo8oDft4oHHc9y13+4XQbvSVlQN
85Vlovob/ASwvvV1ZO75u8PIr2M9Lzxq0YqB/FymH+2mgJtaQEEyzPr5rXYPI3Vo
YCEnCfyldVeCw7EN0/jAhA1AQT9nE9DX0RodMRmsQW25EMjKeHOw0JNDnetrkQ==
-----END CERTIFICATE-----

)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEAk7TI9b9CeyWy69Rgu3wlryw9eNHNU9CyvKsF1urlBedpLAD+
53eQhRcMaPBpxSGVqPCvHzZgZiJmSLf4homF60eWz/vD0c/4D6d9/c/KjAFN/83d
decKeLIvJrSxBr26neHWT7OCf4brSSuqQi8w8C5voybs98s+3pC/WgMd/6FASTOA
XmzbIAkomN7kplZ+nGFBd0faPI2pQndoe44L6GSRAxunwYOsGrC19l5HEnJoeqpq
lQEvowwPokkSQXIhjWEZfGN1PXa2A9B+hTScffEbZfQxGxpn5ZMb/MM+7DNs/act
4bs6E4a1Y38GvqWIvX7LaFmJVwfOvMS2Nd4geQIDAQABAoIBAFtFflykt/8C16H0
lWWlHGA3dTDgbAGb16bQA4a/lY/lZE7lup3reylSib+at7ExxJolSAGAh2b1YMEU
tYTlHBS8IwwYJfPTkBSUSj15lk6XS8yDOJLvlr/oU0yWoUFqIhQHhnevlDcbdu67
BbVqFdKEz0VA+3NPa2T6itOjU3kvCgWVhqjJ5UWRruZ9XGj3lkTUVPIc9OWPZkBu
uhcut0BPTEpvWGqZos54hbvipIqdO1eZ/ypWyP4yAeoGLTY/ANNbgQ/EnUk5YbO0
cCgiFT3ondFE61/FQaN56qgZ3FAactdR8DfVuW+eL5ca8vBuDCXGeTUP8NTC15YW
3In3KGUCgYEAxJKvOgH8Q8wpTL5pv8c7vaP7Oey/cZP2DyxTWRu5QWgl0ZvPmS0z
9fWW61WYXKye4luVvL5ps5dxZHjdY75MffwaGvyS4Z+rzjDTIdLpDLHAzwD8EuTz
70Xs0Fg+maKwBhyBXGfQJve39Mgi9hU+NAgO2pmMuegzuYg199jBPpMCgYEAwFwq
v7StcOpgPp5iFeGZ+/dAJXoMDzTp+/tLNJ1XF6qz7leNeof79LoGngPSVab7okJf
DwwnW9mrxhWtm04W9caXJCeLQvMFRAWmIhlqYfxvVnPbn4Zyn4ishXIofSVpt9id
euLpZDRUuL9ZwEtvbmzwWtWOhpFANyiEyieCQEMCgYBmE4XKqZ1F147au0ZOwDbV
k4Gx4q7HW/46gtoFO9BHlr2My0xSTpbc8lsceaZU+Y7IHKb1DCv3hnzrn5fXY8p0
4aTsvpvHbhLZT6FTvyIl5WgqFmjPpvTFG5qN1kUror+lbMgAJCiDE8AC7Kq6/uz+
N/D+sx3QrDOv9HuR6Izw4QKBgGLwxTg3yaIzm2957b+Ne7ODGGZuYTeNIvDt71/P
shVFQylLtUSaHzlgWbj7BDv0GKf79YL64F+5FUZcriAcyWh/TgxVnqa/8yxqr2oI
opRl0CqqiwOBlwGBviMhdbvw+3CJpW6ouh1FYXJpUdO7uUQWGobOI3IJ0frNB4gn
TDKbAoGAIw8NyXyLleiGh6Zyz/W9N3/hxa1nkluwfm+TSzFh9BIlDj66cyXUxByi
CTxyO8vUjizGQ7opXRMz7x5K1Ih8rs6sy6C3AoNHCho1s9rhyIpUKrsaTQxniqp0
0knUFjMmX9VzNnB9OQzOlKa+0p/fVNsYZy54CCQ+cj+O+V7OqFk=
-----END RSA PRIVATE KEY-----

)KEY";
