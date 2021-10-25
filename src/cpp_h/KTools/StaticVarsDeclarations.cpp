#include "curl.h"
#include <QDir>

// Start CurlClass
QString KTools::Curl::cacertPath = QDir::temp().path() + "/KawaiClient";
QString KTools::Curl::cacertFileName = "cacert.pem";
QString KTools::Curl::fullCacertPath = cacertPath + '/' + cacertFileName;
QString KTools::Curl::pathToCacertInQrc = ":/resources/other/cacert.pem";
QString KTools::Curl::cookiePath = "";
// End CurlClass
