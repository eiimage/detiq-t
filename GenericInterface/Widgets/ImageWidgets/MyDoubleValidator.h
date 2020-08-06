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
