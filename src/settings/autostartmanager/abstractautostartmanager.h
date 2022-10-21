/*
 *  Copyright © 2019-2022 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Optimus Manager Qt.
 *
 *  Optimus Manager Qt is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Optimus Manager Qt is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Optimus Manager Qt. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef ABSTRACTAUTOSTARTMANAGER_H
#define ABSTRACTAUTOSTARTMANAGER_H

#include <QObject>

class AbstractAutostartManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractAutostartManager)

public:
    static AbstractAutostartManager *createAutostartManager(QObject *parent = nullptr);

    virtual bool isAutostartEnabled() const = 0;
    virtual void setAutostartEnabled(bool enabled) = 0;

protected:
    explicit AbstractAutostartManager(QObject *parent = nullptr);

    static void showError(const QString &informativeText);
};

#endif // ABSTRACTAUTOSTARTMANAGER_H
