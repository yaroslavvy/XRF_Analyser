#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H

#include <QListView>
class QMouseEvent;

namespace ui {
    class SpectrumListView : public QListView {
        Q_OBJECT
    public:
        SpectrumListView(QWidget *parent = nullptr);
    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H
