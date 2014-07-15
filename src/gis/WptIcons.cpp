/**********************************************************************************************
    Copyright (C) 2014 Oliver Eichler oliver.eichler@gmx.de

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

**********************************************************************************************/


#include "WptIcons.h"
#include "config.h"

#include <QtGui>

static const char * wptDefault = "://icons/waypoints/32x32/Default.png";

struct icon_t
{
    icon_t(): path(wptDefault), focus(16,16){}
    icon_t(const QString& path, int x, int y) : path(path), focus(x,y) {}
    QString path;
    QPoint  focus;
};

static QMap<QString, icon_t> wptIcons;

void initWptIcons()
{

    wptIcons["Default"]            = icon_t(wptDefault, 16, 16);
//    wptIcons["City (Capitol)"]      = ":/icons/wpt/capitol_city15x15.png";
//    wptIcons["City (Large)"]        = ":/icons/wpt/large_city15x15.png";
//    wptIcons["City (Medium)"]       = ":/icons/wpt/medium_city15x15.png";
//    wptIcons["City (Small)"]        = ":/icons/wpt/small_city15x15.png";
//    wptIcons["Small City"]          = ":/icons/wpt/small_city15x15.png";
//    wptIcons["Geocache"]            = ":/icons/wpt/geocache15x15.png";
//    wptIcons["Geocache Found"]      = ":/icons/wpt/geocache_fnd15x15.png";
    wptIcons["Flag, Red"]           = icon_t("://icons/waypoints/32x32/FlagRed.png", 0, 32);
    wptIcons["Flag, Blue"]          = icon_t("://icons/waypoints/32x32/FlagBlue.png", 0, 32);
    wptIcons["Flag, Green"]         = icon_t("://icons/waypoints/32x32/FlagGreen.png", 0, 32);
    wptIcons["Pin, Red"]            = icon_t("://icons/waypoints/32x32/PinRed.png", 0, 32);
    wptIcons["Pin, Blue"]           = icon_t("://icons/waypoints/32x32/PinBlue.png", 0, 32);
    wptIcons["Pin, Green"]          = icon_t("://icons/waypoints/32x32/PinGreen.png", 0, 32);
//    wptIcons["Block, Red"]          = ":/icons/wpt/box_red15x15.png";
//    wptIcons["Block, Blue"]         = ":/icons/wpt/box_blue15x15.png";
//    wptIcons["Block, Green"]        = ":/icons/wpt/box_green15x15.png";
//    wptIcons["Blue Diamond"]        = ":/icons/wpt/diamond_blue15x15.png";
//    wptIcons["Green Diamond"]       = ":/icons/wpt/diamond_green15x15.png";
//    wptIcons["Red Diamond"]         = ":/icons/wpt/diamond_red15x15.png";


    setWptIconByName("Traditional Cache", "://icons/cache/32x32/traditional.png");
    setWptIconByName("Multi-cache", "://icons/cache/32x32/multi.png");
    setWptIconByName("Unknown Cache", "://icons/cache/32x32/unknown.png");
    setWptIconByName("Wherigo Cache", "://icons/cache/32x32/wherigo.png");
    setWptIconByName("Event Cache", "://icons/cache/32x32/event.png");
    setWptIconByName("Earthcache", "://icons/cache/32x32/earth.png");
    setWptIconByName("Letterbox Hybrid", "://icons/cache/32x32/letterbox.png");
    setWptIconByName("Virtual Cache", "://icons/cache/32x32/virtual.png");
    setWptIconByName("Webcam Cache", "://icons/cache/32x32/webcam.png");

    QDir dirIcon(QDir::home().filePath(CONFIGDIR));
    dirIcon.mkdir("WaypointIcons");
    dirIcon.cd("WaypointIcons");

    QString filename;
    QStringList filenames = dirIcon.entryList(QDir::Files);

    foreach(filename, filenames)
    {
        QFileInfo fi(filename);
        QString name = fi.baseName();
        setWptIconByName(name, dirIcon.filePath(filename));
    }
}

void setWptIconByName(const QString& name, const QString& filename)
{
    QPixmap icon(filename);
    wptIcons[name] = icon_t(filename, icon.width()>>1, icon.height()>>1);
}


void setWptIconByName(const QString& name, const QPixmap& icon)
{
    QDir dirIcon(QDir::home().filePath(CONFIGDIR "WaypointIcons"));
    QString filename = dirIcon.filePath(name + ".png");

    icon.save(filename);
    wptIcons[name] = icon_t(filename, icon.width()>>1, icon.height()>>1);
}

QPixmap loadIcon(const QString& path)
{
    QFileInfo finfo(path);
    if(finfo.completeSuffix() != "bmp")
    {
        return QPixmap(path);
    }
    else
    {
        QImage img = QPixmap(path).toImage().convertToFormat(QImage::Format_Indexed8);
        img.setColor(0,qRgba(0,0,0,0));
        return QPixmap::fromImage(img);
    }

    return QPixmap();
}

QPixmap getWptIconByName(const QString& name, QPointF &focus, QString * src)
{

    if(wptIcons.contains(name))
    {
        focus               = wptIcons[name].focus;
        const QString& icon = wptIcons[name].path;
        if(src) *src = icon;

        return loadIcon(icon);
    }

    focus = wptIcons["Dafault"].focus;
    return loadIcon(wptIcons[name].path);
}
