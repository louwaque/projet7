#include "gamemodel.hpp"
#include <QFileInfo>

GameItem::GameItem(QString fileName, QString name)
    : m_type(FileGame),
      m_name(name),
      m_location(fileName),
      m_port(0)
{
}

GameItem::GameItem(QString address, quint16 port, QString name)
    : m_type(NetworkGame),
      m_name(name),
      m_location(address),
      m_port(port)
{
}

GameItem::GameType GameItem::type() const
{
    return m_type;
}

QString GameItem::name() const
{
    return m_name;
}

void GameItem::setName(const QString &name)
{
    m_name = name;
}

QString GameItem::address() const
{
    return m_location;
}

quint16 GameItem::port() const
{
    return m_port;
}

QString GameItem::fileName() const
{
    return m_location;
}

QString GameItem::location() const
{
    if (m_type == FileGame)
        return QFileInfo(m_location).fileName();
    else if (m_type == NetworkGame)
        return QString("%1:%2").arg(m_location).arg(m_port);

    return "";
}

GameModel::GameModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void GameModel::append(const GameItem &game)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_games.append(game);
    endInsertRows();
}

void GameModel::removeAt(const int index)
{
    if (0 > index || index > m_games.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_games.removeAt(index);
    endRemoveRows();
}

void GameModel::clear()
{
    beginResetModel();
    m_games.clear();
    endResetModel();
}

const GameItem &GameModel::at(const int index) const
{
    return m_games.at(index);
}

GameItem &GameModel::operator[](const int index)
{
    return m_games[index];
}

int GameModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_games.count();
}

QVariant GameModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_games.count())
        return QVariant();

    const GameItem &game = m_games.at(index.row());

    if (role == TypeRole)
        return game.type();
    else if (role == NameRole)
        return game.name();
    else if (role == LocationRole)
        return game.location();
    else if (role == AddressRole)
        return game.address();
    else if (role == PortRole)
        return game.port();
    else if (role == FileNameRole)
        return game.fileName();

    return QVariant();
}

QHash<int, QByteArray> GameModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[TypeRole] = "TypeRole";
    roles[NameRole] = "NameRole";
    roles[LocationRole] = "LocationRole";
    roles[AddressRole] = "AddressRole";
    roles[PortRole] = "PortRole";
    roles[FileNameRole] = "FileNameRole";

    return roles;
}
