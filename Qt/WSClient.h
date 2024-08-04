/* Copyright 2015 kapsule.code. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO 
 * EVENT SHALL EELI REILIN OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation 
 * are those of the authors and should not be interpreted as representing 
 * official policies, either expressed or implied, of Eeli Reilin.
 */
 
#ifndef WSCLIENTLIB_H
#define WSCLIENTLIB_H

#include <QObject>

#include <QUrl>
#include <QUrlQuery>

#include <QHash>
#include <QCryptographicHash>
#include <QScriptEngine>
#include <QScriptValueIterator>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QVariantMap>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>

#include <WSClientException.h>

struct listTypeInfo{
    bool isEntity;
    QString singular;
    QString label;
    QJsonObject information;
};

class WSClient : public QObject
{
    Q_OBJECT
public:
    explicit WSClient(QObject *parent = 0, const QString &lpzHost = "");

private slots:
    void onError(QNetworkReply::NetworkError code);
    void parseNetworkResponse( QNetworkReply *finished );

private:
    QString lpzURL;
    QString lpzExpireTime;
    QString lpzServerTime;
    QString lpzUserId;
    QString lpzUserName;
    QString lpzVersion;
    QString lpzSessionId;    
    QNetworkReply* pRespNetwork;
    QNetworkAccessManager *wsClient;

    QJsonObject jsLoginError;
    QJsonObject jsNoData;

private:
    void Get();
    void Post();
    QJsonObject doGet(const QUrl &url) const;
    QJsonObject doPost(const QUrl &url, const QByteArray &params) const;
    QJsonObject Get(const QString &operation, const QHash<QString, QString> &parameters);
    QJsonObject Post(const QString &operation, const QHash<QString, QString> &parameters);
    QJsonObject doChallenge(const QString &lpzUserName);

    QString getWebServiceURL(const QString &lpzURL);

    inline bool getCheckLogin(void){
        if (!this->lpzSessionId.isEmpty())
            return true;
        return false;
    }

public:
    QString getVersion(void);
    bool doLogin(const QString &lpzUserName, const QString &lpzKey);
    QList<listTypeInfo> doListTypes();
    QJsonObject doRetrieve(const QString id);
    QJsonObject doCreate(const QString name, QHash<QString, QString> &params);
    QJsonObject doUpdate(const QHash<QString, QString> &params);
    QJsonObject doDelete(const QString &id);
    QJsonArray doQuery(const QString &lpzQuery);
    QJsonObject doInvoke();
    QJsonObject doGetRelatedRecords(const QString &record, const QString &module, const QString &relatedmodule, const QString &queryparameters);
    QJsonObject doSetRelated(const QString &relate_this_id, const QString &with_this_ids);
};

#endif // WSClIENT_H
