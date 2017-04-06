#ifndef SUNRISESETMODEL_H
#define SUNRISESETMODEL_H

#include <QAbstractListModel>

#include <QDate>
#include <QTime>

class SunRiseSetLine
{
public:
    explicit SunRiseSetLine(const QDate &date);

    const QDate date() const {return m_date; }
    const QTime lengthOfDay() const {return m_lengthOfDay; }
    const QTime riseTime() const {return m_riseTime; }
    const QTime setTime() const {return m_setTime; }
    const QTime riseCivilTime() const {return m_riseCivilTime; }
    const QTime setCivilTime() const {return m_setCivilTime; }
    const QTime riseNauticalTime() const {return m_riseNauticalTime; }
    const QTime setNauticalTime() const {return m_setNauticalTime; }
    const QTime riseAstronomicalTime() const {return m_riseAstronomicalTime; }
    const QTime setAstronomicalTime() const {return m_setAstronomicalTime; }
    const QTime noonTime() const {return m_noonTime; }

private:
    QDate m_date;
    QTime m_lengthOfDay;
    QTime m_riseTime;
    QTime m_setTime;
    QTime m_riseCivilTime;
    QTime m_setCivilTime;
    QTime m_riseNauticalTime;
    QTime m_setNauticalTime;
    QTime m_riseAstronomicalTime;
    QTime m_setAstronomicalTime;
    QTime m_noonTime;
};

class SunRiseSetModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(Type)
    Q_PROPERTY(double latitude READ latitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY longitudeChanged)
    Q_PROPERTY(double timeZone READ timeZone NOTIFY timeZoneChanged)
    Q_PROPERTY(Type type READ type NOTIFY typeChanged)
public:
    enum Type {
        Offical,
        Civil,
        Nautical,
        Astronomical
    };

    enum Roles {
        DateRole = Qt::UserRole + 1,
        IsTodayRole,
        LengthOfDay,
        RiseTime,
        SetTime,
        RiseCivilTime,
        SetCivilTime,
        RiseNauticalTime,
        SetNauticalTime,
        RiseAstronomicalTime,
        SetAstronomicalTime,
        NoonTime,
        EquationOfTime
    };

    explicit SunRiseSetModel(QObject *parent = 0);

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    static double latitude() {return m_latitude; }
    static double longitude() {return m_longitude; }
    static double timeZone() {return m_timeZone; }
    Type type() const {return m_type; }

    Q_INVOKABLE int localTimeZone() const;

signals:
    void latitudeChanged();
    void longitudeChanged();
    void timeZoneChanged();
    void typeChanged();

public slots:
    void addMoreAtBeginning();
    void addMoreAtEnd();
    void setCoordinates(double lat, double lon, double tz);
    void setType(Type t);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    void init();
    void reinit();


private:
    Type m_type;
    QList<SunRiseSetLine> m_list;
    QDate m_dateBegin;
    QDate m_dateEnd;
    QHash<int, QByteArray> m_roleNames;
    static double m_latitude;
    static double m_longitude;
    static double m_timeZone;
};

#endif // SUNRISESETMODEL_H
