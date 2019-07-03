#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include "tab_bar_spec_window.h"
#include "spectrum_list_mime_data.h"
#include "spectrum_list_view.h"
#include "tab_spec_window.h"

ui::TabBarSpecWindow::TabBarSpecWindow(QWidget *parent)
    :QTabBar (parent)
{
    setAcceptDrops(true);
    setChangeCurrentOnDrag(true);
}

void ui::TabBarSpecWindow::dragEnterEvent (QDragEnterEvent *event) {
    if(event->mimeData()->hasFormat(ctrl::SpectrumListMimeData::mimeType())){
        event->acceptProposedAction();
    }
}

void ui::TabBarSpecWindow::dragMoveEvent (QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void ui::TabBarSpecWindow::dropEvent (QDropEvent *event) {
    const ctrl::SpectrumListModel* sourceSpectrumListModel = qobject_cast<const ctrl::SpectrumListModel*>(ui::SpectrumListView::getSourceSpectrumListModel());
    ui::TabSpecWindow* thisTabSpecWindow = qobject_cast<ui::TabSpecWindow*>(parentWidget());
    ctrl::SpectrumListModel* thisSpectrumListModel = qobject_cast<ctrl::SpectrumListModel*>(thisTabSpecWindow->getCurrentWorkAreaView()->getSpectrumChart()->getModelSpectrums());
    if (sourceSpectrumListModel == thisSpectrumListModel) {
        return;
    }

    if (event->proposedAction() == Qt::MoveAction) {
            event->acceptProposedAction();
    } else if (event->proposedAction() == Qt::CopyAction) {
        event->acceptProposedAction();
    } else {
        return;
    }

    const ctrl::SpectrumListMimeData* spectrumListMimeData = dynamic_cast<const ctrl::SpectrumListMimeData*>(event->mimeData());
    if(spectrumListMimeData){
        QList<ctrl::SpectrumSPM> spectrumList(spectrumListMimeData->getSpectrumList());
        for(auto &spectrum : spectrumList){
            thisSpectrumListModel->addSpectrum(spectrum);
        }
    }
}
