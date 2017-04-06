#include "sunrisesetmodel.h"

#include <cmath>

#include <QSettings>
#include <QDebug>


//offical      = 90 degrees 50'
//civil        = 96 degrees
//nautical     = 102 degrees
//astronomical = 108 degrees
const double Zenith = 90.83333;
const double ZenithCivil = 96.0;
const double ZenithNautical = 102.0;
const double ZenithAstronomical = 108.0;

static double equationOfTime(int n);
static void calculateSunRiseSet(double zenith, const QDate &dt, QTime &riseTime, QTime &setTime, double latitude, double longitude);

//double SunRiseSetModel::m_latitude = 51.507222;
//double SunRiseSetModel::m_longitude = -0.1275;

double SunRiseSetModel::m_latitude = 53.356666676667;
double SunRiseSetModel::m_longitude = 83.787222232222;
double SunRiseSetModel::m_timeZone = +7.0;

SunRiseSetLine::SunRiseSetLine(const QDate &date)
{
    m_date = date;
    calculateSunRiseSet(0.0, m_date, m_riseTime, m_setTime, SunRiseSetModel::latitude(), SunRiseSetModel::longitude());
    calculateSunRiseSet(Zenith, m_date, m_riseTime, m_setTime, SunRiseSetModel::latitude(), SunRiseSetModel::longitude());
    calculateSunRiseSet(ZenithCivil, m_date, m_riseCivilTime, m_setCivilTime, SunRiseSetModel::latitude(), SunRiseSetModel::longitude());
    calculateSunRiseSet(ZenithNautical, m_date, m_riseNauticalTime, m_setNauticalTime, SunRiseSetModel::latitude(), SunRiseSetModel::longitude());
    calculateSunRiseSet(ZenithAstronomical, m_date, m_riseAstronomicalTime, m_setAstronomicalTime, SunRiseSetModel::latitude(), SunRiseSetModel::longitude());
    int s = m_riseTime.secsTo(m_setTime);
    if (s < 0)
        s += 86400;
    int h = s / 3600;
    s %= 3600;
    int m = s / 60;
    s %= 60;
    m_lengthOfDay = QTime(h, m, s);

    QTime t(12, 0);
    s = floor(SunRiseSetModel::longitude() * 240.0 + equationOfTime(m_date.dayOfYear()) * 60.0 + 0.5 - SunRiseSetModel::timeZone() * 3600.0);
    t = t.addSecs(-s);
    m_noonTime = t;
}

//////////////////////////////////

SunRiseSetModel::SunRiseSetModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_type = Offical;

    m_roleNames[DateRole] = "date";
    m_roleNames[IsTodayRole] = "isToday";
    m_roleNames[LengthOfDay] = "lengthOfDay";
    m_roleNames[RiseTime] = "riseTime";
    m_roleNames[SetTime] = "setTime";
    m_roleNames[RiseCivilTime] = "riseCivilTime";
    m_roleNames[SetCivilTime] = "setCivilTime";
    m_roleNames[RiseNauticalTime] = "riseNauticalTime";
    m_roleNames[SetNauticalTime] = "setNauticalTime";
    m_roleNames[RiseAstronomicalTime] = "riseAstronomicalTime";
    m_roleNames[SetAstronomicalTime] = "setAstronomicalTime";
    m_roleNames[NoonTime] = "noonTime";
    m_roleNames[EquationOfTime] = "equationOfTime";

    QDateTime now = QDateTime::currentDateTime();
    QDateTime now1 = now.toUTC();
    now1.setTimeSpec(Qt::LocalTime);
    int minutes = now1.secsTo(now) / 60;
    qDebug() << minutes;
    //qDebug() << QDateTime::currentDateTimeUtc();

    QSettings settings("Avocado", "SunRiseSet");
    settings.beginGroup("Config");
    m_latitude = settings.value("Latitude", m_latitude).toDouble();
    m_longitude = settings.value("Longitude", m_longitude).toDouble();
    m_timeZone = settings.value("TimeZone", m_timeZone).toDouble();
    m_type = (SunRiseSetModel::Type)settings.value("Type", m_type).toInt();
    settings.endGroup();

    init();
}
int SunRiseSetModel::rowCount(const QModelIndex &parent) const
{
        Q_UNUSED(parent);
        return m_list.count();
}

int SunRiseSetModel::columnCount(const QModelIndex &parent) const
{
        Q_UNUSED(parent);
        return 1;
}

QVariant SunRiseSetModel::data(const QModelIndex &index, int role) const
{
    static QDate curDate = QDate::currentDate();
    //if (index.row() < 0 || index.row() >= Database->m_lists.count())
    //    return QVariant();
    if (role == DateRole)
    {
        return m_list[index.row()].date();
    }
    else if (role == IsTodayRole)
    {
        return m_list[index.row()].date() == curDate;

    }
    else if (role == LengthOfDay)
    {
        return m_list[index.row()].lengthOfDay();
    }
    else if (role == RiseTime)
    {
        return m_list[index.row()].riseTime();
    }
    else if (role == SetTime)
    {
        return m_list[index.row()].setTime();
    }
    else if (role == RiseCivilTime)
    {
        return m_list[index.row()].riseCivilTime();
    }
    else if (role == SetCivilTime)
    {
        return m_list[index.row()].setCivilTime();
    }
    else if (role == RiseNauticalTime)
    {
        return m_list[index.row()].riseNauticalTime();
    }
    else if (role == SetNauticalTime)
    {
        return m_list[index.row()].setNauticalTime();
    }
    else if (role == RiseAstronomicalTime)
    {
        return m_list[index.row()].riseAstronomicalTime();
    }
    else if (role == SetAstronomicalTime)
    {
        return m_list[index.row()].setAstronomicalTime();
    }
    else if (role == NoonTime)
    {
        return m_list[index.row()].noonTime();
    }
    else if (role == EquationOfTime)
        return equationOfTime(m_list[index.row()].date().dayOfYear());
    return QVariant();
}

int SunRiseSetModel::localTimeZone() const
{
    QDateTime now = QDateTime::currentDateTime();
    QDateTime now1 = now.toUTC();
    now1.setTimeSpec(Qt::LocalTime);
    int h = now1.secsTo(now) / 3600;
    return h;
}


void SunRiseSetModel::addMoreAtBeginning()
{
    beginInsertRows(QModelIndex(), 0, 9);
    for (int d = 0; d < 10; d++)
    {
        m_dateBegin = m_dateBegin.addDays(-1);
        SunRiseSetLine line(m_dateBegin);
        m_list.prepend(line);
    }
    endInsertRows();
}

void SunRiseSetModel::addMoreAtEnd()
{
    beginInsertRows(QModelIndex(), m_list.count(), m_list.count() + 9);
    for (int d = 0; d < 10; d++)
    {
        m_dateEnd = m_dateEnd.addDays(1);
        SunRiseSetLine line(m_dateEnd);
        m_list.append(line);
    }
    endInsertRows();
}

QHash<int, QByteArray> SunRiseSetModel::roleNames() const
{
    return m_roleNames;
}

void SunRiseSetModel::setCoordinates(double lat, double lon, double tz)
{
    m_latitude = lat;
    m_longitude = lon;
    m_timeZone = tz;
    // save
    QSettings settings("Avocado", "SunRiseSet");
    settings.beginGroup("Config");
    settings.setValue("Latitude", m_latitude);
    settings.setValue("Longitude", m_longitude);
    settings.setValue("TimeZone", m_timeZone);
    settings.setValue("Type", m_type);
    settings.endGroup();

    reinit();
}

void SunRiseSetModel::setType(Type t)
{
    m_type = t;
    QSettings settings("Avocado", "SunRiseSet");
    settings.beginGroup("Config");
    settings.setValue("Type", m_type);
    settings.endGroup();
    emit typeChanged();
}

void SunRiseSetModel::init()
{
    beginResetModel();
    m_list.clear();
    QDate date = QDate::currentDate();
    int initn = 10;
    date = date.addDays(- initn);
    m_dateBegin = date;
    for (int d = 0; d < initn * 2 + 1; d++)
    {
        SunRiseSetLine line(date);
        m_list.append(line);
        date = date.addDays(1);
    }
    m_dateEnd = date.addDays(-1);
    endResetModel();
}

void SunRiseSetModel::reinit()
{
    beginResetModel();
    for (int i = 0; i < m_list.count(); i++)
    {
        QDate date = m_list[i].date();
        m_list[i] = SunRiseSetLine(date);
    }
    endResetModel();
}

// Calculation code

static const double pi = 3.14159265359;

static double normAngle(double a)
{
    while(a >= 360.0)
        a -= 360.0;
    while(a <= -360.0)
        a += 360.0;
    return a;
}

static QTime calculateTime(double coszenith, double latitude, double longitude, int N, bool rise)
{
    double lngHour = longitude / 15.0;
    double t;
    if (rise)
        t = double(N) + ((6.0 - lngHour) / 24.0);
    else
        t = double(N) + ((18.0 - lngHour) / 24.0);
    //qDebug() << t;
    double M = (0.9856 * t) - 3.289;
    //qDebug() << M;
    double L = M + (1.916 * sin(M * pi / 180.0)) + (0.020 * sin(M * pi / 90.0)) + 282.634;
    L = normAngle(L);
    //qDebug() << L;
    double RA = atan(0.91764 * tan(L * pi / 180.0));
    RA = normAngle(RA * 180.0 / pi);
    //qDebug() << RA;
    //double Lquadrant = (floor(L / 90.0 + 0.5)) * 90.0;
    //double RAquadrant = (floor(RA / 90.0 + 0.5)) * 90.0;
    double Lquadrant = (floor(L / 90.0)) * 90.0;
    double RAquadrant = (floor(RA / 90.0)) * 90.0;
    //qDebug() << "-- " << Lquadrant << " - " << RAquadrant;
    RA = RA + (Lquadrant - RAquadrant);
    //qDebug() << RA;
    RA /= 15.0;
    //qDebug() << RA;
    double sinDec = 0.39782 * sin(L * pi / 180.0);
    double cosDec = cos(asin(sinDec));
    //qDebug() << sinDec;
    //qDebug() << cosDec;
    double cosH = (coszenith - (sinDec * sin(latitude * pi / 180.0))) / (cosDec * cos(latitude * pi / 180.0));
    //qDebug() << cosH;
    if (cosH < -1.0 || cosH > 1.0)
    {
        return QTime();
    }
    if (rise)
    {
        double H = 360.0 - acos(cosH) * 180.0 / pi;
        H /= 15.0;
        double T = H + RA - (0.06571 * t) - 6.622;
        //qDebug() << T;
        double UT = T - lngHour;
        while(UT >= 24.0)
            UT -= 24.0;
        while(UT < 0.0)
            UT += 24.0;
        //NOTE: UT potentially needs to be adjusted into the range [0,24) by adding/subtracting 24
        //qDebug() << UT;
        UT += SunRiseSetModel::timeZone();
        double hour = floor(UT);
        double min = 60.0 * (UT - hour);
        while(hour >= 24.0)
            hour -= 24.0;
        while(hour < 0.0)
            hour += 24.0;
        return QTime(hour, min);
    }
    else
    {
        double H = acos(cosH) * 180.0 / pi;
        H /= 15.0;
        double T = H + RA - (0.06571 * t) - 6.622;
        //qDebug() << T;
        double UT = T - lngHour;
        while(UT >= 24.0)
            UT -= 24.0;
        while(UT < 0.0)
            UT += 24.0;
        //NOTE: UT potentially needs to be adjusted into the range [0,24) by adding/subtracting 24
        //qDebug() << UT;
        UT += SunRiseSetModel::timeZone();
        double hour = floor(UT);
        double min = 60.0 * (UT - hour);
        while(hour >= 24.0)
            hour -= 24.0;
        while(hour < 0.0)
            hour += 24.0;
        return QTime(hour, min);
    }
}

static void calculateSunRiseSet(double zenith, const QDate &dt, QTime &riseTime, QTime &setTime, double latitude, double longitude)
{
    double coszenith = cos(zenith  * pi / 180.0);
    int N = dt.dayOfYear();
    riseTime = calculateTime(coszenith, latitude, longitude, N, true);
    setTime = calculateTime(coszenith, latitude, longitude, N, false);
}

static double equationOfTime(int n)
{
    double b = 2.0 * pi * double(n - 81) / 365.0;
    return 9.87 * sin(2.0 * b) - 7.53 * cos(b) - 1.5 * sin(b);
}
