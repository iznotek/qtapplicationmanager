/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Pelagicore Application Manager.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#pragma once

#include <exception>
#include <QObject>

#if defined(Q_CC_MSVC)
#  pragma warning(disable: 4290)
#endif

QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QWindow)

class StartupInterface
{
public:
    virtual ~StartupInterface();

    virtual void initialize(const QVariantMap &systemProperties) Q_DECL_NOEXCEPT_EXPR(false) = 0;

    virtual void afterRuntimeRegistration() Q_DECL_NOEXCEPT_EXPR(false) = 0;
    virtual void beforeQmlEngineLoad(QQmlEngine *engine) Q_DECL_NOEXCEPT_EXPR(false) = 0;
    virtual void afterQmlEngineLoad(QQmlEngine *engine) Q_DECL_NOEXCEPT_EXPR(false) = 0;

    virtual void beforeWindowShow(QWindow *window) Q_DECL_NOEXCEPT_EXPR(false) = 0;
    virtual void afterWindowShow(QWindow *window) Q_DECL_NOEXCEPT_EXPR(false) = 0;
};

#define AM_StartupInterface_iid "io.qt.ApplicationManager.StartupInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(StartupInterface, AM_StartupInterface_iid)
QT_END_NAMESPACE
