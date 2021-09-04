#ifndef KTOOLS_CURL_H
#define KTOOLS_CURL_H

#include <curl/curl.h>
#include <QString>
#include <QMap>

class QElapsedTimer;

namespace KTools
{
    class Curl
    {
    public:
        enum class RequestType {
            Get,
            Post
        };
        enum class CookieMode {
            None,
            GetAllTimes,
            GetAndKeep,
            Keep,
            Void
        };
        enum class HeaderMode {
            None,
            Custom
        };
        enum class SetHeaderMode {
            Add,
            New
        };
        enum class HttpVersion {
            Http2,
            Standart
        };
        enum class RequestMode {
            Old,
            New
        };
    private:
        struct CurlHandle
        {
            CurlHandle()
            {
                handle = curl_easy_init();
                cookieMode = CookieMode::Void;
                headerMode = HeaderMode::Custom;
                requestType = RequestType::Get;
                requestMode = RequestMode::New;
                httpVersion = HttpVersion::Http2;
                stdErr = NULL;
                postParam = new QString();
                cookieFileName = new QString();
                requestHeader = new QMap<QString, QString>();
                slistHeader = NULL;
                responseHeader = new QByteArray();
            }
            ~CurlHandle()
            {
                curl_easy_cleanup(handle);
                if (stdErr != NULL)
                    delete stdErr;
                if (postParam != NULL)
                    delete postParam;
                if (cookieFileName != NULL)
                    delete cookieFileName;
                if (slistHeader != NULL)
                    curl_slist_free_all(slistHeader);
                if (requestHeader != NULL)
                    delete requestHeader;
                if (responseHeader != NULL)
                    delete responseHeader;
            }
            CURL *handle;
            CURLcode res;
            struct curl_slist *slistHeader;
            FILE *stdErr;
            CookieMode cookieMode;
            HeaderMode headerMode;
            RequestType requestType;
            RequestMode requestMode;
            HttpVersion httpVersion;
            QMap<QString, QString> *requestHeader;
            QByteArray *responseHeader;
            QString *postParam;
            QString *cookieFileName;
            long responseCode; // response code for last request
            bool emitProgress = false;
            bool autoRefer = true;
        };
        struct RequestTypesStrings
        {
            std::string get = "GET";
            std::string post = "POST";
        };
        RequestTypesStrings requestTypesStrings;

    public:
        Curl();
        ~Curl();

        QByteArray performing(const char* url);
        QByteArray request(const QString &url);
        void restartSession();

        void setOptions();
        void setHttpVersion(const HttpVersion vers);
        void setCookieMode(const CookieMode mode);
        void setHeaderMode(const HeaderMode mode);
        void setAutoReferer(const bool ref);
        void setPostParam(const QString &data);
        void setHeader(QVector<QByteArray> chunk, const SetHeaderMode mode = SetHeaderMode::Add);
        void setHeader(const QMap<QString, QString> &chunk);
        void setHeaderLine(const std::pair<QString, QString> &headerLine);
        void delHeaderLine(const QString &key);
        void setRequestType(RequestType requType);
        void setErrFile(const QString &path, const QString &fileName);
        void unsetErrFile();

        const QString& getPostParam();

        void addHandle();
        void swichHandle(const qint32 handleNumber);

        //Parsers downloaderType;
        static QString cookiePath;

    private:
        //CURLcode res;
        static quint64 writeMemoryCallback(char *data, quint64 size, quint64 nmemb, QByteArray *writerData);
        //static int XFerInfoFunctionCallback(void *p, double dlTotal, double dlNow, double ulTotal, double ulNow);
        static quint64 headerCallback(char *buffer, quint64 size, quint64 nitems, QByteArray *userdata);
        void generateHeader();

        void curlEasySetopt(const CURLoption &option, std::string &parameter);
        void curlEasySetopt(const CURLoption &option, long parameter);
        void curlEasySetopt(const CURLoption &option, QByteArray &parameter);
        void curlEasySetopt(const CURLoption &option, std::nullptr_t parameter);
        void curlEasySetopt(const CURLoption &option, curl_slist *parameter);
        void curlEasySetopt(const CURLoption &option, FILE *parameter);
        void curlEasySetopt(const CURLoption &option, void *parameter);

        //void curlEasyGetinfo();

        //struct curl_slist *header;
        std::string voidString = "";
        std::string userAgent = "Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0";
        std::string acceptEncoding = "gzip, deflate, br";
        std::string stdFullCacertPath;
        static QString cacertPath;
        static QString cacertFileName;
        static QString fullCacertPath;
        static QString pathToCacertInQrc;
        QVector<CurlHandle*> *handlesList;
        qint32 numb; // number of the current hadle
        /*struct ForProggress
        {
            Curl *th;
            QElapsedTimer *timer;
            double lastDlTotal;
            double lastUlTotal;
            double lastDlNow;
            double lastUlNow;
        };*/
    };
}

#endif // KTOOLS_CURL_H
