/*  This file is part of the KDE libraries
 *  Copyright 2013 Kevin Ottens <ervin+bluesystems@kde.org>
 *  Copyright 2013 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *  Copyright 2013 Alejandro Fiestas Olivares <afiestas@kde.org>
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License or ( at
 *  your option ) version 3 or, at the discretion of KDE e.V. ( which shall
 *  act as a proxy as in section 14 of the GPLv3 ), any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "khintssettings.h"

#include <QDir>
#include <QString>
#include <QFileInfo>
#include <QToolBar>
#include <QMainWindow>
#include <QApplication>
#include <QDialogButtonBox>

#include <kiconloader.h>
#include <kconfiggroup.h>
#include <ksharedconfig.h>
#include <kcolorscheme.h>

KHintsSettings::KHintsSettings() : QObject(0)
{
    KConfigGroup cg(KSharedConfig::openConfig("kdeglobals"), "KDE");

    m_hints[QPlatformTheme::CursorFlashTime] = qBound(200, cg.readEntry("CursorBlinkRate", 1000), 2000);
    m_hints[QPlatformTheme::MouseDoubleClickInterval] = cg.readEntry("DoubleClickInterval", 400);
    m_hints[QPlatformTheme::StartDragDistance] = cg.readEntry("StartDragDist", 10);
    m_hints[QPlatformTheme::StartDragTime] = cg.readEntry("StartDragTime", 500);

    const QString buttonStyle = cg.readEntry("ToolButtonStyle", "TextBesideIcon").toLower();
    m_hints[QPlatformTheme::ToolButtonStyle] = buttonStyle == "textbesideicon" ? Qt::ToolButtonTextBesideIcon
                             : buttonStyle == "icontextright" ? Qt::ToolButtonTextBesideIcon
                             : buttonStyle == "textundericon" ? Qt::ToolButtonTextUnderIcon
                             : buttonStyle == "icontextbottom" ? Qt::ToolButtonTextUnderIcon
                             : buttonStyle == "textonly" ? Qt::ToolButtonTextOnly
                             : Qt::ToolButtonIconOnly;

    m_hints[QPlatformTheme::ToolBarIconSize] = cg.readEntry("ToolbarIconsSize", 22);
    m_hints[QPlatformTheme::ItemViewActivateItemOnSingleClick] = cg.readEntry("SingleClick", true);
    m_hints[QPlatformTheme::SystemIconThemeName] = cg.readEntry("IconsTheme", "oxygen");
    m_hints[QPlatformTheme::SystemIconFallbackThemeName] = "hicolor";
    m_hints[QPlatformTheme::IconThemeSearchPaths] = xdgIconThemePaths();
    m_hints[QPlatformTheme::StyleNames] = (QStringList() << cg.readEntry("WidgetStyle", QString())
                                         << "oxygen"
                                         << "fusion"
                                         << "windows");
    m_hints[QPlatformTheme::DialogButtonBoxLayout] = QDialogButtonBox::KdeLayout;
    m_hints[QPlatformTheme::DialogButtonBoxButtonsHaveIcons] = cg.readEntry("ShowIconsOnPushButtons", true);
    m_hints[QPlatformTheme::UseFullScreenForPopupMenu] = true;
    m_hints[QPlatformTheme::KeyboardScheme] = QPlatformTheme::KdeKeyboardScheme;
    m_hints[QPlatformTheme::UiEffects] = cg.readEntry("GraphicEffectsLevel", 0) != 0 ? QPlatformTheme::GeneralUiEffect : 0;
    m_hints[QPlatformTheme::IconPixmapSizes] = QVariant::fromValue(QList<int>() << 512 << 256 << 128 << 64 << 32 << 22 << 16 << 8);

    QMetaObject::invokeMethod(this, "setupIconLoader", Qt::QueuedConnection);
}

QStringList KHintsSettings::xdgIconThemePaths() const
{
    QStringList paths;

    const QFileInfo homeIconDir(QDir::homePath() + QStringLiteral("/.icons"));
    if (homeIconDir.isDir())
        paths << homeIconDir.absoluteFilePath();

    QString xdgDirString = QFile::decodeName(qgetenv("XDG_DATA_DIRS"));
    if (xdgDirString.isEmpty())
        xdgDirString = QLatin1String("/usr/local/share/:/usr/share/");

    foreach (const QString &xdgDir, xdgDirString.split(QLatin1Char(':'))) {
        const QFileInfo xdgIconsDir(xdgDir + QStringLiteral("/icons"));
        if (xdgIconsDir.isDir())
            paths << xdgIconsDir.absoluteFilePath();
    }

    return paths;
}

void KHintsSettings::setupIconLoader()
{
    connect(KIconLoader::global(), &KIconLoader::iconChanged, this, &KHintsSettings::iconChanged);
}

void KHintsSettings::iconChanged(int group)
{
    KIconLoader::Group iconGroup = (KIconLoader::Group) group;
    if (iconGroup != KIconLoader::Toolbar) {
        return;
    }

    const int currentSize = KIconLoader::global()->currentSize(KIconLoader::Toolbar);
    if (m_hints[QPlatformTheme::ToolBarIconSize] == currentSize) {
        return;
    }

    m_hints[QPlatformTheme::ToolBarIconSize] = currentSize;
    QWidgetList widgets = QApplication::allWidgets();
    Q_FOREACH(QWidget* widget, widgets) {
        if (qobject_cast<QToolBar*>(widget) || qobject_cast<QMainWindow*>(widget)) {
            QEvent event(QEvent::StyleChange);
            QApplication::sendEvent(widget, &event);
        }
    }
}