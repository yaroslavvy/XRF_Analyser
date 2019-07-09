#ifndef QUALITATIVE_ANALYSIS_EXCEPTION_H
#define QUALITATIVE_ANALYSIS_EXCEPTION_H

#include <QException>

namespace ctrl {
    class Exception : public QException {
    public:
        Exception();
        Exception(const QString& message);

        const char* what() const noexcept override;

    private:
        const char* m_message;
    };
}

#endif // QUALITATIVE_ANALYSIS_EXCEPTION_H
