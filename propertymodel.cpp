#include "propertymodel.hpp"
#include <QJsonDocument>

PropertyItem::PropertyItem(const int id, QObject *parent)
    : QObject(parent),
      m_properties()
{
    m_properties["id"] = id;
}

int PropertyItem::id() const
{
    return m_properties["id"].toInt();
}

const QVariantMap &PropertyItem::properties() const
{
    return m_properties;
}

void PropertyItem::setProperties(const QVariantMap &prop)
{
    if (id() == prop["id"].toInt()) {
        m_properties = prop;
        emit propertiesChanged();
    }
}

QVariant PropertyItem::property(const QString &key) const
{
    return m_properties[key];
}

QVariant PropertyItem::operator[](const QString &key) const
{
    return m_properties[key];
}

void PropertyItem::setProperty(const QString &key, const QVariant &value)
{
    if (value.isValid() && key != "id" && m_properties[key] != value) {
        m_properties[key] = value;
        emit propertiesChanged();
    }
}

void PropertyItem::removeProperty(const QString &key)
{
    if (key != "id" && m_properties.contains(key)) {
        m_properties.remove(key);
        emit propertiesChanged();
    }
}

QJsonObject PropertyItem::toJson() const
{
    return QJsonObject::fromVariantMap(m_properties);
}

void PropertyItem::fromJson(const QJsonObject &json)
{
    setProperties(json.toVariantMap());
}

PropertyModel::PropertyModel(QObject *parent)
    : QAbstractListModel(parent),
      m_properties()
{
}

PropertyItem* PropertyModel::append(const int proposed_id)
{
    int id = proposed_id;
    for (int i = 0; i < m_properties.size(); ++i) {
        if (id == m_properties[i]->id()) {
            ++id;
            i = 0;
        }
    }

    QSharedPointer<PropertyItem> property(makeProperty(id));

    connect(property.data(), &PropertyItem::propertiesChanged, this, [this](){
        PropertyItem *property = qobject_cast<PropertyItem*>(sender());
        QModelIndex index = createIndex(indexOf(property->id()), 0);
        emit dataChanged(index, index, {PropertiesRole}); //add item ?
    });

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_properties.append(property);
    endInsertRows();

    return property.data();
}

PropertyItem *PropertyModel::at(const int index) const
{
    return m_properties[index].data();
}

void PropertyModel::removeAt(const int index)
{
    if (0 > index || index > m_properties.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_properties.removeAt(index);
    endRemoveRows();
}

bool PropertyModel::removeOne(const int id)
{
    int index = indexOf(id);

    if (index >= 0) {
        removeAt(index);
        return true;
    }

    return false;
}

void PropertyModel::clear()
{
    beginResetModel();
    m_properties.clear();
    endResetModel();
}

int PropertyModel::indexOf(const int id) const
{
    int index = -1;

    for (int i = 0; i < m_properties.size(); ++i)
        if (m_properties[i]->id() == id)
            index = i;

    return index;
}

int PropertyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_properties.count();
}

QVariant PropertyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_properties.count())
        return QVariant();

    PropertyItem *property = at(index.row());

    if (role == ItemRole)
        return QVariant::fromValue(property);
    else if (role == IdRole)
        return property->id();
    else if (role == PropertiesRole)
        return property->properties();

    return QVariant();
}

QJsonArray PropertyModel::toJson() const
{
    QJsonArray array;

    for (const QSharedPointer<PropertyItem> pt : m_properties)
        array.append(pt->toJson());

    return array;
}

void PropertyModel::fromJson(const QJsonArray &json)
{
    clear();

    for (const QJsonValue &value : json) {
        if (value.isObject()) {
            QJsonObject property = value.toObject();

            if (property.contains("id")) {
                PropertyItem *item = append(property["id"].toInt());
                item->fromJson(property);
            }
        }
    }
}

PropertyItem *PropertyModel::makeProperty(const int id)
{
    return new PropertyItem(id, this);
}

QHash<int, QByteArray> PropertyModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ItemRole] = "item_role";
    roles[IdRole] = "id_role";
    roles[PropertiesRole] = "properties_role";

    return roles;
}
