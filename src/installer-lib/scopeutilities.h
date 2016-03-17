/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the Pelagicore Application Manager.
**
** $QT_BEGIN_LICENSE:GPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Pelagicore Application Manager
** licenses may use this file in accordance with the commercial license
** agreement provided with the Software or, alternatively, in accordance
** with the terms contained in a written agreement between you and
** Pelagicore. For licensing terms and conditions, contact us at:
** http://www.pelagicore.com.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3 requirements will be
** met: http://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: GPL-3.0
**
****************************************************************************/

#pragma once

#include <QString>
#include <QDir>
#include <QFile>


class ScopedDirectoryCreator
{
public:
    ScopedDirectoryCreator();
    bool create(const QString &path, bool removeExisting = true);
    bool take();
    bool destroy();
    ~ScopedDirectoryCreator();

    QDir dir();

private:
    Q_DISABLE_COPY(ScopedDirectoryCreator)

    QString m_path;
    bool m_created = false;
    bool m_taken = false;
};

class ScopedFileCreator
{
public:
    ScopedFileCreator();
    bool create(const QString &path, bool removeExisting = true);
    bool take();
    ~ScopedFileCreator();

    QFile *file();

private:
    Q_DISABLE_COPY(ScopedFileCreator)

    QFile m_file;
    bool m_created = false;
    bool m_taken = false;
};

class ScopedLoopbackCreator
{
public:
    ScopedLoopbackCreator();
    bool create(const QString &imagePath);
    bool take();
    bool destroy();
    ~ScopedLoopbackCreator();

    QString device() const;
    QString errorString() const;

private:
    Q_DISABLE_COPY(ScopedLoopbackCreator)

    QString m_loopDevice;
    bool m_taken = false;
};

class ScopedMounter
{
public:
    ScopedMounter();
    bool mount(const QString &devicePath, const QString &mountPoint, bool readWrite);
    bool take();
    bool unmount();
    ~ScopedMounter();

    QString errorString() const;

private:
    Q_DISABLE_COPY(ScopedMounter)

    QString m_devicePath;
    QString m_mountPoint;
    bool m_mounted = false;
    bool m_taken = false;
};

class ScopedRenamer
{
public:
    enum Mode {
        NameToNameMinus = 1,  // create backup   : foo -> foo-
        NamePlusToName  = 2   // replace with new: foo+ -> foo
    };

    Q_DECLARE_FLAGS(Modes, Mode)

    ScopedRenamer();
    bool rename(const QString &baseName, ScopedRenamer::Modes modes);
    bool rename(const QDir &baseName, ScopedRenamer::Modes modes);
    bool take();
    bool undoRename();
    ~ScopedRenamer();

    bool isRenamed() const;
    QString baseName() const;

private:
    bool interalUndoRename();
    static bool internalRename(const QDir &dir, const QString &from, const QString &to);
    Q_DISABLE_COPY(ScopedRenamer)
    QDir m_basePath;
    QString m_name;
    Modes m_requested;
    Modes m_done;
    bool m_taken = false;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ScopedRenamer::Modes)
