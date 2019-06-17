#include "exception.h"

ctrl::Exception::Exception()
    :QException()
{
}

ctrl::Exception::Exception(const QString& message)
    :QException()
{
    m_message = message.toStdString().data();
}

const char* ctrl::Exception::what() const noexcept {
    return m_message;
}
