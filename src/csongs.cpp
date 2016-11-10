#include "csongs.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

CSongs::CSongs(QObject *parent) : QAbstractListModel(parent)
{
    CSongs::loadJson();
    CSongs::filter("");
}

//! [3]
bool CSongs::loadJson()
{
    QFile loadFile(QStringLiteral("/opt/sdk/componentgallery/usr/capoeiralyrics.json"));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    read(loadDoc.object());

    return true;
}
//! [3]

//! [4]
bool CSongs::saveJson() const
{
    QFile saveFile(QStringLiteral("save.json"));

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject gameObject;
    write(gameObject);
    QJsonDocument saveDoc(gameObject);
    saveFile.write(saveDoc.toJson());

    return true;
}
//! [4]

//! [1]
void CSongs::read(const QJsonObject &json)
{
    QJsonArray songsArray = json["songs"].toArray();
    for (int i = 0; i < songsArray.size(); ++i) {
        QJsonObject songObject = songsArray[i].toObject();

        CSong song;
        song.read(songObject);
        cSongs.insert(song.cTitle.toLower() ,song);
    }
}
//! [1]

//! [2]
void CSongs::write(QJsonObject &json) const
{
    /*QJsonObject playerObject;
    //mPlayer.write(playerObject);
    json["player"] = playerObject;

    QJsonArray levelArray;
    /*foreach (const Level level, mLevels) {
        QJsonObject levelObject;
        level.write(levelObject);
        levelArray.append(levelObject);
    }*/
    //json["levels"] = levelArray;
}
//! [2]


QHash<int, QByteArray> CSongs::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CTitleRole] = "title";
    roles[CSubtitleRole] = "subtitle";
    roles[CUrlRole] = "url";
    roles[CYTPlayerRole] = "ytplayer";
    roles[CTextTole] = "text";

    return roles;
}

QVariant CSongs::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) {
        return QVariant();
    }
    if(role == CTitleRole) {
        //return QVariant(cSongs[index.row()].cTitle);
        return QVariant(cFilterSongs[index.row()].cTitle);
    }
    if(role == CSubtitleRole) {
        return QVariant(cFilterSongs[index.row()].cSubtitle);
    }
    if(role == CUrlRole) {
        return QVariant(cFilterSongs[index.row()].cUrl);
    }
    if(role == CYTPlayerRole) {
        return QVariant(cFilterSongs[index.row()].cYTPlayer);
    }
    if(role == CTextTole) {
        return QVariant(cFilterSongs[index.row()].cText);
    }
    return QVariant();
}

void CSongs::filter(const QString searchString) {
    if (searchString == "")
        cFilterSongs = cSongs.values();
    else {
        cFilterSongs.clear();

        QMap<QString, CSong>::iterator i;
        for (i = cSongs.begin(); i != cSongs.end(); ++i)
            if (i.key().contains(searchString))
                cFilterSongs.append(i.value());
    }
}
