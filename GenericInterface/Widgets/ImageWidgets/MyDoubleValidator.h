/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of ImageINSA.
 *
 * ImageINSA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ImageINSA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ImageINSA.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MYDOUBLEVALIDATOR_H
#define MYDOUBLEVALIDATOR_H

#include <QDoubleValidator>

namespace genericinterface
{

class MyDoubleValidator : public QDoubleValidator {
public:
    MyDoubleValidator (QObject * parent = 0) : QDoubleValidator(parent) {}
    MyDoubleValidator (double bottom, double top, int decimals, QObject * parent) : QDoubleValidator(bottom, top, decimals, parent) {}

    QValidator::State validate(QString & s, int & pos) const {
        // Empty field and incompletely entered value are enabled
        if (s.isEmpty() || s == QString::number(bottom()).left(s.length())) {
            return QValidator::Intermediate;
        }

        // Decimal position checking
        QChar point = locale().decimalPoint();
        if(s.indexOf(point) != -1) {
            int lengthDecimal = s.length() - s.indexOf(point) - 1;
            if (lengthDecimal > decimals()) {
                return QValidator::Invalid;
            }
        }

        // Ensure the input range is guaranteed
        double value = s.toDouble();
        if (value >= bottom() && value <= top()) {
            return QValidator::Acceptable;
        }
        return QValidator::Invalid;
    }
};

}
#endif // MYDOUBLEVALIDATOR_H
