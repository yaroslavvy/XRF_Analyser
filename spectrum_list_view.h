#ifndef QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H
#define QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H

#include <QListView>
class QMouseEvent;
class QMainWindow;
class QMdiSubWindow;

namespace ctrl {
    class SpectrumListModel;
}

namespace ui {
    class SpectrumListView : public QListView {
        Q_OBJECT
    public:
        SpectrumListView(QWidget *parent = nullptr);
        static const ctrl::SpectrumListModel* getSourceSpectrumListModel();

    public slots:
        void deselectAll();
        void invertSelection();
        void showHideSpectrum();
        void spectrumSettings();
        void spectrumInformation();
        void deleteSpectrum();

    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;

        void mouseMoveEvent(QMouseEvent *event) override;
        void dragEnterEvent (QDragEnterEvent *event) override;
        void dragMoveEvent (QDragMoveEvent *event) override;
        void dropEvent (QDropEvent *event) override;

    private:
        void startDrag();
        QPoint m_ptDragPos;
        static ctrl::SpectrumListModel* m_sourceSpectrumListModel;
    };
}

#endif // QUALITATIVE_ANALYSIS_SPECTRUM_LIST_VIEW_H
