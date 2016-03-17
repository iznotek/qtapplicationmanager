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
#include <QByteArray>
#include <QMutex>
#include <qplatformdefs.h>

#ifdef Q_OS_UNIX
#  include <sys/types.h>
#else
typedef uint uid_t;
typedef uint gid_t;
//typedef uint mode_t; // already typedef'ed in qplatformdefs.h
#endif


enum SudoDropPrivileges {
    DropPrivilegesPermanently,
    DropPrivilegesRegainable, // only use this for auto-tests
    NoPrivilegeSeparation     // use this on Windows or for debugging
};

bool forkSudoServer(SudoDropPrivileges dropPrivileges, QString *errorString);

class SudoInterface
{
public:
    virtual QString attachLoopback(const QString &imagePath, bool readonly) = 0;
    virtual bool detachLoopback(const QString &loopDev) = 0;
    virtual bool mount(const QString &device, const QString &mountPoint, bool readonly, const QString &fstype) = 0;
    virtual bool unmount(const QString &mountPoint, bool force) = 0;
    virtual bool mkfs(const QString &device, const QString &fstype, const QStringList &options) = 0;
    virtual bool removeRecursive(const QString &fileOrDir) = 0;
    virtual bool setOwnerAndPermissionsRecursive(const QString &fileOrDir, uid_t user, gid_t group, mode_t permissions) = 0;

protected:
    enum MessageType { Request, Reply };

#ifdef Q_OS_LINUX
    QByteArray receiveMessage(int socket, MessageType type, QString *errorString);
    bool sendMessage(int socket, const QByteArray &msg, MessageType type, const QString &errorString = QString());
#endif
    QByteArray receive(const QByteArray &packet);

protected:
    SudoInterface();
private:
    Q_DISABLE_COPY(SudoInterface)
};

class SudoServer;

class SudoClient : public SudoInterface
{
public:
    static bool initialize(int socketFd, SudoServer *shortCircuit = 0);

    static SudoClient *instance();

    QString attachLoopback(const QString &imagePath, bool readonly = false) override;
    bool detachLoopback(const QString &loopDev) override;
    bool mount(const QString &device, const QString &mountPoint, bool readonly = false, const QString &fstype = QStringLiteral("ext2")) override;
    bool unmount(const QString &mountPoint, bool force = false) override;
    bool mkfs(const QString &device, const QString &fstype = QStringLiteral("ext2"), const QStringList &options = QStringList()) override;
    bool removeRecursive(const QString &fileOrDir) override;
    bool setOwnerAndPermissionsRecursive(const QString &fileOrDir, uid_t user, gid_t group, mode_t permissions) override;

    void stopServer();

    QString lastError() const { return m_errorString; }

private:
    SudoClient(int socketFd);

    QByteArray call(const QByteArray &msg);

    int m_socket;
    QString m_errorString;
    QMutex m_mutex;
    SudoServer *m_shortCircuit;

    static SudoClient *s_instance;
};

class SudoServer : public SudoInterface
{
public:
    static bool initialize(int socketFd, int loopControlFd);

    static SudoServer *instance();

    QString attachLoopback(const QString &imagePath, bool readonly) override;
    bool detachLoopback(const QString &loopDev) override;
    bool mount(const QString &device, const QString &mountPoint, bool readonly, const QString &fstype) override;
    bool unmount(const QString &mountPoint, bool force) override;
    bool mkfs(const QString &device, const QString &fstype, const QStringList &options) override;
    bool removeRecursive(const QString &fileOrDir) override;
    bool setOwnerAndPermissionsRecursive(const QString &fileOrDir, uid_t user, gid_t group, mode_t permissions) override;

    QString lastError() const { return m_errorString; }

    void run();

private:
    SudoServer(int socketFd, int loopControlFd);

    QByteArray receive(const QByteArray &msg);
    friend class SudoClient;

    int m_socket;
    int m_loopControl;
    QString m_errorString;
    bool m_stop = false;

    static SudoServer *s_instance;
};

