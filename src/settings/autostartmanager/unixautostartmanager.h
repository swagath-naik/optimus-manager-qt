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

#ifndef UNIXAUTOSTARTMANAGER_H
#define UNIXAUTOSTARTMANAGER_H

#include "abstractautostartmanager.h"

class UnixAutostartManager : public AbstractAutostartManager
{
    Q_OBJECT
    Q_DISABLE_COPY(UnixAutostartManager)

public:
    explicit UnixAutostartManager(QObject *parent = nullptr);

    bool isAutostartEnabled() const override;
    void setAutostartEnabled(bool enabled) override;
};

#endif // UNIXAUTOSTARTMANAGER_H
