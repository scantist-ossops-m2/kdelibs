/*
 * This file is part of the KDE Libraries
 * Copyright (C) 2011 Romain Perier (bambi@ubuntu.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef QLOCALIZEDSTRING_PORTING_H
#define QLOCALIZEDSTRING_PORTING_H

#include <QtCore/QString>
#include <QtCore/QLatin1String>

#pragma message("KDE5 TODO: Remove this file once QLocalizedString is in Qt5")

#ifndef I18N_NOOP2
#define I18N_NOOP2(comment,x) x
#endif

// This is convenience wrapper to be able to build kcoreaddons without kdecore.
// The code still uses ki18n* and KLocalizedString (renamed to QLocalizedString) and is unchanged.
class QLocalizedString
{
public:
    QLocalizedString(): _string(NULL) {}
    QLocalizedString(const char *text): _string(text) {}
    QString toString(int i = 0) const { Q_UNUSED(i); return _string; }
    bool isEmpty() const { return toString().isEmpty(); }

    QLatin1String _string;
};

static inline QLocalizedString ki18n (const char* msg)
{
    return QLocalizedString(msg);
}

static inline QLocalizedString ki18nc (const char *ctxt, const char *msg)
{
    Q_UNUSED(ctxt);
    return ki18n(msg);
}

static inline QString i18n(const char *msg, const QString &c1 = QString(), const QString &c2 = QString())
{
    Q_UNUSED(c1); Q_UNUSED(c2);
    return ki18n(msg).toString();
}

static inline QString i18nc(const char *ctx, const char *msg, const QString &c1 = QString(),
                            const QString &c2 = QString(), const QString &c3= QString(), const QString &c4 = QString())
{
    Q_UNUSED(ctx);
    Q_UNUSED(c1); Q_UNUSED(c2);
    Q_UNUSED(c3); Q_UNUSED(c4);
    return i18n(msg);
}

#endif