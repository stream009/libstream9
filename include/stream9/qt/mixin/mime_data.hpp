#ifndef MIXIN_MIME_TYPE_HPP
#define MIXIN_MIME_TYPE_HPP

#include <QtCore/QByteArray>
#include <QtCore/QMimeData>
#if(QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

namespace stream9 { namespace qt { namespace mixin {

inline void
setAction(QMimeData &data, const QAction &action)
{
    const auto &bytes = QByteArray::number(reinterpret_cast<size_t>(&action));
    data.setData("application/x-action-id", bytes);
}

inline QAction *
getAction(const QMimeData &data)
{
    const auto &bytes = data.data("application/x-action-id");
    auto* const action = reinterpret_cast<QAction*>(bytes.toUInt());

    return action;
}

}}} // namespace stream9::qt::mixin

#endif // MIXIN_MIME_TYPE_HPP
